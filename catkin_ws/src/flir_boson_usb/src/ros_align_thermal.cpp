#include <cali/align_thermal_to_rgb.h>
#include <chrono>
#include <ros/package.h>
#include <ros/ros.h>

void align_thermal_to_rgb::callback(const sensor_msgs::ImageConstPtr &in1, const sensor_msgs::ImageConstPtr &in2, const sensor_msgs::ImageConstPtr &in3)
{
  std::string path = ros::package::getPath("flir_boson_usb");
  std::string camera_model;
  std::string rgb_intrinsic_file;
  camera_model = path + "/example_calibrations/camera_model.txt";
  rgb_intrinsic_file = path + "/example_calibrations/rgb_intrinsic_file.txt";
  align_thermal_to_rgb foo(camera_model);
  Intrinsic intrinsic;
  parse_rgb_intrinsic(rgb_intrinsic_file, intrinsic);
  foo.set_rgb_intrinsic(intrinsic);

  cv_bridge::CvImagePtr cv_ptr_thermal;
  cv_bridge::CvImagePtr cv_ptr_rgb;
  cv_bridge::CvImagePtr cv_ptr_depth;
  cv_ptr_thermal =  cv_bridge::toCvCopy(in1);
  cv_ptr_rgb =  cv_bridge::toCvCopy(in2);
  cv_ptr_depth =  cv_bridge::toCvCopy(in3);
  cv::Mat rgb = cv_ptr_rgb->image;
  cv::Mat depth = cv_ptr_depth->image;
  cv::Mat thermal = cv_ptr_thermal->image;
  cv::Mat dst;
  
  foo.set_rgb_image(rgb);
  foo.set_depth_image(depth);
  foo.set_thermal_image(thermal);
  auto s_ts = std::chrono::high_resolution_clock::now();
  foo.align(dst);
  auto e_ts = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(e_ts-s_ts).count();
  // Print conversion time
  // std::cout << "Atlignemnt spend " << duration*1e-6 << " ms\n";

  cv_bridge::CvImage out_msg;
  out_msg.header   = in1->header; // Same timestamp and tf frame as input image
  out_msg.encoding = cv_ptr_thermal->encoding; // Or whatever
  out_msg.image    = dst; // Your cv::Mat
  out_msg.header.stamp = ros::Time::now();

  pub_1.publish(out_msg.toImageMsg());
}


align_thermal_to_rgb::align_thermal_to_rgb(ros::NodeHandle nh, ros::NodeHandle pnh):nh_(nh), pnh_(pnh){
	sub_1_.subscribe(nh_,"/flir_boson/image_raw", 1);
	sub_2_.subscribe(nh_,"/camera/color/image_raw", 1);
  sub_3_.subscribe(nh_,"/camera/aligned_depth_to_color/image_raw", 1);
  pub_1 = nh_.advertise<sensor_msgs::Image>("/flir_boson/align_image", 1);
  sync_.reset(new Sync(MySyncPolicy(10), sub_1_, sub_2_, sub_3_));
  sync_->registerCallback(boost::bind(&align_thermal_to_rgb::callback, this, _1, _2, _3));
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ros_align_thermal");
  ros::NodeHandle nh, pnh("~");

  align_thermal_to_rgb align_thermal_to_rgb(nh, pnh);
  ros::spin();
}