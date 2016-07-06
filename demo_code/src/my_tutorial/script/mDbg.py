#!/usr/bin/env python
# -*- coding: utf-8 -*-
import sys

"This Module is a Dbg Lib"
def DBG_Printf(flag, x):#Blue
    if flag:
        print "\033[34;1m" + "[DBG]",x,"\033[0m"
def INF_Printf(flag, x):#Yellow
    if flag:
        print "\033[33;1m" + "[INF]", x, "\033[0m"
def NOT_Printf(flag, x):#Green
    if flag:
        print "\033[32;1m" + "[NOT]", x, "\033[0m"
def WAR_Printf(flag, x):#Purple
    if flag:
        print "\033[35;1m" + "[WAR]", x, "\033[0m"
def ERR_Printf(flag, x):#Red
    if flag:
        print "\033[31;1m" + "[ERR]", x, "\033[0m"

if __name__ == '__main__':
    DBG_Printf(True, "This is mDbg Test")
    INF_Printf(True, "This is mDbg Test")
    NOT_Printf(True, "This is mDbg Test")
    WAR_Printf(True, "This is mDbg Test")
    ERR_Printf(True, "This is mDbg Test")


