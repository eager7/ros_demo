#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <cv_bridge/cv_bridge.h>
#include "zhelpers.hpp"
//#include <zmq.h>

zmq::context_t context(1);
zmq::socket_t publisher(context, ZMQ_PUB);

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    //CvImageConstPtr mat_gen = cv_bridge::toCvShare(msg, "bgr8")->image;
    cv::Mat mat_gen = cv_bridge::toCvShare(msg, "bgr8")->image;
    cv::imshow("zmq_view", mat_gen);
    //cv::imshow("zmq_view", cv_bridge::toCvShare(msg, "bgr8")->image);
    size_t len = mat_gen.total() * mat_gen.channels();
    zmq::message_t message(len);
// zmq_msg_init_size(&message, len);
    memcpy(message.data(), mat_gen.data, len);
    bool rc = publisher.send(message);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "zmq_image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("zmq_view");
  publisher.bind("tcp://*:5550");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);
  ros::spin();
  cv::destroyWindow("zmq_view");
}
