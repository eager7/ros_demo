#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy,roslib
from std_msgs.msg import String
import cv2
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError


if __name__ == '__main__':
    print "camera display"
    cv2.namedWindow("Image window")
    try:
        img = cv2.imread("/home/pct/ros_demo/demo_code/src/opencv_test/src/test.jpg")
    except IOError, e:
        print e
    if img.any():
        cv2.imshow("Image window", img)
    else:
        print "img is null"

    cv2.waitKey(3000)
    cv2.destroyAllWindows()
