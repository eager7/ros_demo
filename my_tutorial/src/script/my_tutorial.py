#!/usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
from mDbg import *


if __name__ == '__main__':
    DBG_Printf(True, "This is My Tutorial Program")
    rospy.init_node("my_turorial", anonymous=False)
    pub = rospy.Publisher("/turtle1/cmd_vel", geometry_msgs/Twist)
