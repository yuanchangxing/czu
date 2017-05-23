#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys

import orm_sample
import threading
import time

#entry for python logic.

request_count = 0


def entry_py(start_flag, cmd, seq, useid, reserved, length, body):
    threading.Thread(target=test_increase, args=(start_flag, cmd, seq, useid, reserved, length, body), name=str(str(useid)+str(seq))).start()

    return 0, body


def test_increase(start_flag, cmd, seq, useid, reserved, length, body):
    time.sleep(1)
    print ("hello", start_flag, cmd, seq, useid, reserved, length, body)
    print (len(body))
    orm_sample.orm_entry()

    print (sys.path)

#
if __name__ == '__main__':
    print (sys.path)
    print ("==========>")
    entry_py(0,0,0,0,0,0,"body")