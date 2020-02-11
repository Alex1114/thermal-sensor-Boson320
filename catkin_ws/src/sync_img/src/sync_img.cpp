#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <iostream>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <image_transport/image_transport.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// using namespace message_filters;

class Node
{
private:
  ros::NodeHandle nh_;

  message_filters::Subscriber<sensor_msgs::Image> sub_1_;
  message_filters::Subscriber<sensor_msgs::Image> sub_2_;
  message_filters::Subscriber<sensor_msgs::Image> sub_3_;
  ros::Publisher pub_1;
  ros::Publisher pub_2;
  ros::Publisher pub_3;

  typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image, sensor_msgs::Image, sensor_msgs::Image> MySyncPolicy;
  typedef message_filters::Synchronizer<MySyncPolicy> Sync;
  boost::shared_ptr<Sync> sync_;

public:
  Node()
  {
    ros::NodeHandle nh_;
    sub_1_.subscribe(nh_, "/camera/color/image_raw", 1);
    sub_2_.subscribe(nh_, "/flir_boson/image_raw", 1);
    sub_3_.subscribe(nh_, "/camera/aligned_depth_to_color/image_raw", 1);
    pub_1 = nh_.advertise<sensor_msgs::Image>("/camera/color/image_raw_sync", 1);
    pub_2 = nh_.advertise<sensor_msgs::Image>("/flir_boson/image_raw_sync", 1);
    pub_3 = nh_.advertise<sensor_msgs::Image>("/camera/aligned_depth_to_color/image_raw_sync", 1);
    sync_.reset(new Sync(MySyncPolicy(10), sub_1_, sub_2_, sub_3_));
    sync_->registerCallback(boost::bind(&Node::callback, this, _1, _2, _3));
  }

  void callback(const sensor_msgs::ImageConstPtr &in1, const sensor_msgs::ImageConstPtr &in2, const sensor_msgs::ImageConstPtr &in3)
  {
    ROS_INFO("Synchronization successful");

    pub_1.publish(in1);
    pub_2.publish(in2);
    pub_3.publish(in3);
  }
};

int main(int argc, char **argv)
{
  ros::init(argc, argv, "synchronizer");

  Node synchronizer;

  ros::spin();
}