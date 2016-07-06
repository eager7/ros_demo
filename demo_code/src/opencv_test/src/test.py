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
    img = cv2.imread("test.jpg")
    if img:
        cv2.imshow("Image window", img)
    else:
        print "img is null"
    cv2.waitKey(30)
    cv2.destroyAllWindows()
