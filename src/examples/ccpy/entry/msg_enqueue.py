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
    lib.test.restype = ctypes.c_char_p
    ret = lib.test(ctypes.c_char_p(b'hello from python'))
    print (str(ret))