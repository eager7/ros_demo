#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy,roslib
from std_msgs.msg import String
import cv2
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import numpy as np

def SubccriberHandler(data):
    print "Recv Publicer Msg"
    #img = cv2.imread("/home/pct/ros_demo/opencv_test/src/resource/test.jpg")
    #cv2.imshow("Image", img)

    bridge = CvBridge()
    try:
        #prev_gs = cv2.cvtColor(np.asarray(data[:,:]), cv2.COLOR_BGR2GRAY)
        cv_image = bridge.imgmsg_to_cv2(data, 'bgr8')
        cv2.imshow("Image", cv_image)
        cv2.waitKey(3)

    except CvBridgeError as e:
        print e

if __name__ == '__main__':
    print "camera display"
    cv2.namedWindow("Image")
    cv2.waitKey(2000)

    rospy.init_node('camera_display', anonymous=True)
    rospy.Subscriber("camera/image", Image, SubccriberHandler)
    rospy.spin()

    #cv2.destroyAllWindows()