#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sstream> // for converting the command line parameter to integer

int main(int argc, char** argv)
{
  // Check if video source has been passed as a parameter
//  if(argv[1] == NULL) return 1;   
  // if you do not want to input parameter when doing rosrun then you have to comment this line

  ros::init(argc, argv, "image_publisher");
  ros::NodeHandle nh("~");
  image_transport::ImageTransport it(nh);
  std::string video_source_name("a");


  image_transport::Publisher pub = it.advertise("camera/image", 1);

  // Convert the passed as command line parameter index for the video device to an integer

 //  std::istringstream video_sourceCmd("0");
  int video_source = 1;
  nh.param("video_sname", video_source, 0);
  std::cout << video_source << " lllllll" << std::endl;
//  ros::param::get("~video_sname", video_source);
  std::cout << video_source << " 22222" << std::endl;


  cv::VideoCapture cap(video_source);
  // Check if video device can be opened with the given index
  if(!cap.isOpened()) return 1;
  cv::Mat frame;
  sensor_msgs::ImagePtr msg;

///  ros::Rate loop_rate(5);
  while (nh.ok()) {
    cap >> frame;
    // Check if grabbed frame is actually full with some content
    if(!frame.empty()) {
      msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", frame).toImageMsg();
      pub.publish(msg);
//      cv::waitKey(1);
    }

    ros::spinOnce();
//   loop_rate.sleep();
  }
}
