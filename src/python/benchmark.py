#!/usr/bin/env python
# -*- coding: utf-8 -*-


# import gl
import sys, threading

import json
import httplib, urllib, urllib2
import time
from net.unblocktcpclient  import UnblockTcpClient

import json,thread


seqid_lock = thread.allocate_lock() #创建锁对象

recieve_num_lock = thread.allocate_lock() #创建锁对象
send_num_lock=thread.allocate_lock()
recieve_num= 0
send_num = 0
seqid=0
phone = "15960565013"
name = "test-name"

users=""


def get_seq():
    seqid_lock.acquire()
    global seqid
    seqid += 1
    seqid_lock.release()
    return seqid


def send_im_callback(proto):
    recieve_num_lock.acquire() #加锁
    global recieve_num
    recieve_num = recieve_num + 1
    # print ("recieve_num",recieve_num)
    recieve_num_lock.release()

def benchmark(index):

    tcp = UnblockTcpClient("127.0.0.1", 8008)

    tcp.start_client()




    msg = {}
    msg["CONTENT_TEXT"] = "hello,I am "
    msg['src_userid'] = 9999
    msg['datetime'] = str(time.localtime())
    root = [msg]
    print (json.dumps(root))

    for i in range(0, 10000):
        # send(index,tcp)
        send_num_lock.acquire()
        global send_num
        send_num = send_num + 1
        tcp.send_json(1, 2, get_seq(), json.dumps(root) , send_im_callback)

        send_num_lock.release()

        # time.sleep(0.1)

    while(1):
        time.sleep(100)  #保活，以便连接维持。


if __name__ == '__main__':
    time.sleep(1)

    for i in range(0, 1):
        threading.Thread(target=benchmark, args=(i,), name=str(i)).start()
        time.sleep(0.2)
        #threading.Thread(target=doAdd, args=(), name='thread - ' + str(i)).start()
        #benchmark()



    while(1):
        time.sleep(10)
        # print ("send_num:========================================",send_num,"recieve_num:", recieve_num)
        # print ("send_num:========================================",send_num,"recieve_num:", recieve_num)
        print ("send_num:========================================",send_num,"recieve_num:", recieve_num)
