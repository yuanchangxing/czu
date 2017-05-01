#!/usr/bin/env python
# -*- coding: utf-8 -*-
import socket
import thread
import sys
from struct import pack, unpack
import time
import threading



class UnblockTcpClient(threading.Thread):
    def __init__(self, _ip,  _port):
        self.port_ = _port
        self.ip_ = _ip
        self.pdu_length_ = 24
        self.notify_count = 0
        self.callback_queue_ = {}
        self.start_flag_ = 7777777
        self.socket_ = socket.socket
        self.buffer_ = ""
        if(self.port_ == 0):
            self.port_ = 8003
        if(len(self.ip_)==0 ):
            self.ip_ = "192.168.0.42"
        threading.Thread.__init__(self)

    def start_client(self):

        self.socket_ = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket_.setblocking(1)
        self.socket_.setsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF, 10240)
        print ( "self.socket_.getsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF)",self.socket_.getsockopt(socket.SOL_SOCKET, socket.SO_RCVBUF) )
        ADDR = (self.ip_, self.port_)
        # print (sys._getframe().f_code.co_name,ADDR , "启动")

        self.socket_ = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket_.connect(ADDR)
        # time.sleep(1)
        self.start()
        # thread.start_new(self.run, ())
        #方法没有参数时需要传入空tuple

    def on_recieve(self,pdu):
        self.notify_count +=1
        # print ("-------------------------------->on_recieve", pdu["cmd"])
        print ("----------->\n", pdu)

        # raise NotImplementedError

    def send(self, pdu):
        buffer = pack(">I", self.start_flag_) + pack(">I", pdu["userid"]) + pack(">I", pdu["cmd"]) + pack(">I", pdu["seq"]) + pack(">I", 0) + pack(">I", pdu["length"]) + pdu["body"]
        # print (sys._getframe().f_code.co_name,"send buffer ", buffer)
        self.socket_.send(buffer)

    def send_proto(self, userid,cmd, seq, proto, callback):
        sendbuffer = pack(">I", self.start_flag_) + pack(">I", userid) +\
                  pack(">I", cmd) + pack(">I", seq) + pack(">I", 0) +\
                  pack(">I", proto.ByteSize()) + proto.SerializeToString()
        # print (sys._getframe().f_code.co_name,sendbuffer,"length is \n", len(sendbuffer), "proto size:", proto.ByteSize())
        if(callback!=None):
            self.callback_queue_[seq] = callback
        self.socket_.send(sendbuffer)

    def send_json(self,userid,cmd, seq, jsonstr, callback):
        sendbuffer = pack(">I", self.start_flag_)+ pack(">I", cmd) + pack(">I", seq) +\
                     pack(">I", userid) +\
                    pack(">I", 0) +\
                     pack(">I", len(jsonstr)) + jsonstr
        # print (sys._getframe().f_code.co_name,sendbuffer,"length is \n", len(sendbuffer), "proto size:", len(jsonstr))
        if(callback!=None):
            self.callback_queue_[seq] = callback
            # print ("回调函数注册,seq:", seq)
        self.socket_.send(sendbuffer)

    def run(self):
        # print "run recv thread"
        while(1):

            data = self.socket_.recv(10240)
            self.buffer_ = self.buffer_ + data

            # print (sys._getframe().f_code.co_name, "read bytes", len(data))
            while 1:
                if(len(self.buffer_)<4):
                    # print ("数据不足标记位长度，继续读")
                    break
                packet = {}
                # print ("len(data)",len(self.buffer_))
                packet["start"], = unpack(">I", self.buffer_[0:4])
                if(self.start_flag_!=packet["start"]):
                    self.buffer_ = ""
                    break
                packet["cmd"], = unpack(">I", self.buffer_[4:8])
                packet["seq"], = unpack(">I", self.buffer_[8:12])
                print ("sequence:", packet["seq"] )
                packet["userid"], = unpack(">I", self.buffer_[12:16])
                packet["reserved"],= unpack(">I", self.buffer_[16:20])
                # datatype, reserved = unpack(">hh", self._buffer[16:20])
                # print (sys._getframe().f_code.co_name,datatype,reserved)
                packet["length"], = unpack(">I", self.buffer_[20:24])
                if(packet["length"]+ self.pdu_length_ <= len(self.buffer_)):
                    # print (self.pdu_length_, self.pdu_length_ + packet["length"])
                    packbuf = self.buffer_[self.pdu_length_: self.pdu_length_ + packet["length"]]
                    packet["body"] = packbuf
                    self.buffer_ = self.buffer_[packet["length"]+ self.pdu_length_:-1]
                    if self.callback_queue_.has_key( packet["seq"]):
                        self.callback_queue_[packet["seq"] ](packet)
                        # print (sys._getframe().f_code.co_name, "[succed certain callback]", packet)
                        # print ("注册回调函数")
                        #删除回调
                        self.callback_queue_.pop(packet["seq"])
                    else:
                        self.on_recieve( packet )
                        # print (sys._getframe().f_code.co_name, len(self.callback_queue_), "[succed default callback]", packet)

                else:
                    break

                # print ("============>")

                # eee = str(data[20:20 + leng])
                # print "book list  is:", eee, "\n\n"
                # print eval(eee)

