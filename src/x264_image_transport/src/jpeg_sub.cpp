#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fstream>

static cv::Mat matEle;
bool init_server(int rfd, int sfd); 

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
//    cv::resizeWindow("view", 800, 100);
//    cv::moveWindow("view", 220, 300);
    matEle = cv_bridge::toCvShare(msg, "bgr8")->image;
    cv::imshow("view", matEle);
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
  }
}

void* new_thread(void*) {
  int socketfd;
  socketfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socketfd == -1) {
    printf("create socket failed.............\n");
    return NULL;
  }
//  int optval = 1;
//  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));

  struct sockaddr_in server;
  bzero(&server,sizeof(server)); 
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = htonl(INADDR_ANY);
  server.sin_port = htons(33445);
  if (bind(socketfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    fprintf(stderr, "bind Error:%s\n", strerror(errno));   
    return NULL;
  }

  int socketsfd;
  socketsfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (socketsfd == -1) {
    printf("create socket failed.............\n");
    return NULL;
  }
//  int optval = 1;
//  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));



  printf("wating............\n");
  while (true) {
    init_server(socketfd, socketsfd);
  }
  close(socketfd);
  fflush(stdout);
}

int receive_data(int rfd, char *buf, int len, struct sockaddr_in *client) {
  int length = sizeof(*client);
  int nbytes = 0;
//  while (strncmp(buf, "get image", 4)) {
    if ((nbytes = recvfrom(rfd, buf, len, 0, (struct sockaddr *)client, (socklen_t *)&length)) < 0) {
      perror("could not read datagram !");
    }
    //printf("received from %s:%d\n", inet_ntoa(client->sin_addr), ntohs(client->sin_port));
    //printf("data : %s\n", buf);
  //}
  return nbytes;
}

bool send_data(int sfd, char *buf, int len, struct sockaddr_in client) {
  struct sockaddr_in sserver;
  bzero(&sserver,sizeof(sserver)); 
  sserver.sin_family = AF_INET;
  sserver.sin_addr.s_addr = client.sin_addr.s_addr;
  sserver.sin_port = htons(33444);
    //printf("send to  %s:%d\n", inet_ntoa(sserver.sin_addr), ntohs(sserver.sin_port));
  if ((sendto(sfd, buf, len, 0, (struct sockaddr *)&sserver, sizeof(sserver))) < 0) {
    //printf("send to  %s:%d\n", inet_ntoa(sserver.sin_addr), ntohs(sserver.sin_port));
    perror("send");
    return false;  
  }
  return true;
}

bool init_server(int rfd, int sfd) {
  struct sockaddr_in client;
  char* image_buff = NULL;
  int image_len = 0;
  int times = 0;
  int len = 1024;
  while (1) {
    int recv_len = 0;
    char buf[1024] = {0};
    //printf("%s, %d\n", __func__, __LINE__);
    recv_len = receive_data(rfd, buf, len, &client); 
    //printf("%s, %d\n", __func__, __LINE__);
    if (!strncmp(buf, "get image", 4)) {
      std::vector<uchar> vimagebuff;
      cv::imencode(".jpg", matEle, vimagebuff);
      image_len = vimagebuff.size();
      if (image_buff) {
        delete [] image_buff;
        image_buff = NULL;
      }
      image_buff = new char[image_len];
      printf("image_len = %d\n", image_len);
      for (int i = 0; i < image_len; i++) {
        image_buff[i] = vimagebuff[i];
      }
#if 1
      std::ofstream of1("a.jpg", std::ofstream::binary);
      of1.write(image_buff, image_len);
      of1.close();
      printf("write done ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
#endif
      times = image_len / len;
      if (image_len % len)
              times ++;
      send_data(sfd, (char *)&image_len, sizeof(image_len), client);
    } else {
      //printf("%d, %d, %d, %d\n", buf[0], buf[1], buf[2], buf[3]);
      //printf("%x, %x, %x, %x\n", buf[0], buf[1], buf[2], buf[3]);
      int index = 0;
      index += buf[0] << 24; 
      index += buf[1] << 16;
      index += buf[2] << 8;
      index += buf[3];
     // printf("index %d, times = %d, len = %d\n", index, times, len);
      if (index == times - 1) 
        send_data(sfd, (char *)(image_buff + (index * len)), image_len - index * len, client);
      else {
        //printf("send %d\n", len);
        send_data(sfd, (char *)(image_buff + (index * len)), len, client);
//        send_data(sfd, (char *)&image_buff + (len), len, client);
      }

      //printf("index %d, times = %d, len = %d\n", index, times, len);
    }
  }
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "image_listener");
  ros::NodeHandle nh;
  cv::namedWindow("view");
  cv::startWindowThread();
  image_transport::ImageTransport it(nh);
  image_transport::Subscriber sub = it.subscribe("camera/image", 1, imageCallback);
  pthread_t send_thread;
  pthread_create(&send_thread, NULL, new_thread, NULL);
  ros::spin();
  cv::destroyWindow("view");
}
