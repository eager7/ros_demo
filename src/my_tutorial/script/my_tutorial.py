#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
from geometry_msgs.msg import Twist,Vector3
from mDbg import *

if __name__ == '__main__':
    DBG_Printf(True, "This is My Tutorial Program")
    try:
        rospy.init_node("my_turorial", anonymous=False)
        pub = rospy.Publisher("/turtle1/cmd_vel", Twist, queue_size=5)

        msg = Twist()
        msg.linear  = Vector3(2.0, 0.0, 0.0)
        msg.angular = Vector3(0.0, 0.0, 1.8)

        while not rospy.is_shutdown():
            key = raw_input("Please input anykey wihtout 'q'")
            if key == 'q':
                break
            NOT_Printf(True, "Move The Tutorial")
            pub.publish(msg)
    except rospy.ROSInternalException, e:
        ERR_Printf(True, e)