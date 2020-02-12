#ifndef _ALIGN_DEPTH_TO_RGB_H
#define _ALIGN_DEPTH_TO_RGB_H

#include <cassert>
#include <fstream>
#include <string>
#include <opencv2/opencv.hpp>
#include <tf/tf.h>
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/Image.h>
#include <iostream>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../src/helper.hpp"

class align_thermal_to_rgb
{
public:
  align_thermal_to_rgb();
  align_thermal_to_rgb(std::string);
  align_thermal_to_rgb(cv::Mat, cv::Mat, tf::Transform);

  align_thermal_to_rgb(ros::NodeHandle nh, ros::NodeHandle pnh);


private:
  ros::NodeHandle nh_, pnh_;

  bool has_camera_model;
  bool has_rgb_intrinsic;
  bool has_rgb_image;
  bool has_depth_image;
  bool has_thermal_image;
  const int kernel_size = 7;
  cv::Mat intrinsic_;
  cv::Mat distortion_;
  cv::Mat rgb_image_;
  cv::Mat depth_image_;
  cv::Mat thermal_image_;
  Intrinsic rgb_intrinsic_;
  tf::Transform extrinsic_;

  message_filters::Subscriber<sensor_msgs::Image> sub_1_;
  message_filters::Subscriber<sensor_msgs::Image> sub_2_;
  message_filters::Subscriber<sensor_msgs::Image> sub_3_;
  ros::Publisher pub_1;


  typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image, sensor_msgs::Image> MySyncPolicy;
  typedef message_filters::Synchronizer<MySyncPolicy> Sync;
  boost::shared_ptr<Sync> sync_;

public:
  void set_camera_model(cv::Mat, cv::Mat, tf::Transform);
  void set_rgb_image(cv::Mat img)
  {
    rgb_image_ = img.clone();
    has_rgb_image = true;
  }
  void set_depth_image(cv::Mat img)
  {
    if (img.type() == CV_16UC1)
      depth_image_ = img.clone();
    has_depth_image = true;
  }
  void set_thermal_image(cv::Mat img)
  {
    thermal_image_ = img.clone();
    has_thermal_image = true;
  }
  void set_rgb_intrinsic(Intrinsic);
  void set_rgb_intrinsic(std::vector<double>);
  void set_rgb_intrinsic(double, double, double, double);
  void show_model(void);
  void align(cv::Mat &);

  void callback(const sensor_msgs::ImageConstPtr& , const sensor_msgs::ImageConstPtr& , const sensor_msgs::ImageConstPtr& );



};
#endif