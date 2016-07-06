#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy,roslib
from std_msgs.msg import String
import cv2
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

def SubccriberHandler(data):
    print "Recv Publicer Msg"
    img = cv2.imread("test.jpg")
    cv2.imshow("Image window", img)
    return
    bridge = CvBridge()
    try:
        cv_image = bridge.cv2_to_imgmsg(data, 'bgr8')
        (rows,cols,channels) = cv_image.shape
        if cols > 60 and rows > 60 :
            cv2.circle(cv_image, (50,50), 10, 255)
            cv2.imshow("Image window", cv_image)
            cv2.waitKey(3)
    except CvBridgeError as e:
        print e




if __name__ == '__main__':
    print "camera display"
    cv2.namedWindow("Image window")
    try:
        rospy.init_node('camera_display', anonymous=True)
        rospy.Subscriber("camera/image", Image, SubccriberHandler)
        rospy.spin()
    except rospy.ROSInterruptException:
        pass
    cv2.destroyAllWindows()