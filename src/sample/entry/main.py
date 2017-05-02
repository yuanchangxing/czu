#!/usr/bin/env python
# -*- coding: utf-8 -*-

#entry for python logic.

def entry_py(start_flag, cmd, seq, useid, reserved, length, body):
    print ("hello", start_flag, cmd, seq, useid, reserved, length, body)
    print (len(body))
    return ( 0, body)