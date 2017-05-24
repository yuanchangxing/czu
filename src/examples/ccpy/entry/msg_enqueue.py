#!/usr/bin/env python
# -*- coding: utf-8 -*-

#
import ctypes
from ctypes import cdll
#
#
# lib = cdll.LoadLibrary('../../../tools/send_msg_queue/libsend_msg_queue.so')
# lib = cdll.LoadLibrary('./libsend_msg_queue.so')
lib = cdll.LoadLibrary('../../tools/send_msg_queue/libsend_msg_queue.so')
# print (lib)
#
#
def msg_enqueue():
    # lib.msg_enqueue.restype = ctypes.c_char_p
    print ("enqueue start")
    ret = lib.msg_enqueue(ctypes.c_char_p(b'hello from python'))
    print ("enqueue over")
    # print (str(ret))