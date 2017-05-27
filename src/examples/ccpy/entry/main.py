#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

import orm_sample
import threading,thread
import time

import msg_enqueue
#entry for python logic.

request_count = 0
request_count_lock = thread.allocate_lock()


def entry_py(start_flag, cmd, seq, useid, reserved, length, body):

    print (u"python model run")
    # threading.Thread(target=increase, args=(start_flag, cmd, seq, useid, reserved, length, body), name=str(str(useid)+str(seq))).start()
    threading.Thread(target=increase, args=(start_flag, cmd, seq, useid, reserved, length, body),
                     name=str(str(useid) + str(seq))).start()
    # increase(start_flag,cmd,seq,useid,reserved, length,body)
    print (u"11111")

    return 0, body


def increase(start_flag, cmd, seq, useid, reserved, length, body):
    print (u"2222")

    global request_count

    print (u"333")

    # time.sleep(1)
    request_count_lock.acquire()
    request_count += 1
    print ("hello", start_flag, cmd, seq, useid, reserved, length, body, "request_count",request_count)
    request_count_lock.release()
    print (len(body))
    # orm_sample.orm_entry()

    print (sys.path)
    msg_enqueue.msg_enqueue()
    print (u"python model increase over")


if __name__ == '__main__':
    print (sys.path)
    print ("==========>")
    entry_py(0,0,0,0,0,0,"body")