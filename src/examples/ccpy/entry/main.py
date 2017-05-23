#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

import orm_sample
import threading,thread
import time

#entry for python logic.

request_count = 0
request_count_lock = thread.allocate_lock()


def entry_py(start_flag, cmd, seq, useid, reserved, length, body):
    threading.Thread(target=test_increase, args=(start_flag, cmd, seq, useid, reserved, length, body), name=str(str(useid)+str(seq))).start()

    return 0, body


def test_increase(start_flag, cmd, seq, useid, reserved, length, body):
    global request_count


    time.sleep(1)
    request_count_lock.acquire()
    request_count += 1
    print ("hello", start_flag, cmd, seq, useid, reserved, length, body, "request_count",request_count)
    request_count_lock.release()
    print (len(body))
    # orm_sample.orm_entry()

    print (sys.path)

#
if __name__ == '__main__':
    print (sys.path)
    print ("==========>")
    entry_py(0,0,0,0,0,0,"body")