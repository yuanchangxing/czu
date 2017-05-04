#!/usr/bin/env python
# -*- coding: utf-8 -*-

import orm_sample
import os, sys
#entry for python logic.

def entry_py(start_flag, cmd, seq, useid, reserved, length, body):
    print ("hello", start_flag, cmd, seq, useid, reserved, length, body)
    print (len(body))
    orm_sample.orm_entry()
    print (sys.path)
    return ( 0, body)

#
# if __name__ == '__main__':
#     print (sys.path)
#     print ("==========>")
#     entry_py(0,0,0,0,0,0,"body")