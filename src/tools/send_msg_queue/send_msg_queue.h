//
// Created by rotencode on 5/22/17.
//

#ifndef CCPY_SEND_MSG_QUEUE_H_H
#define CCPY_SEND_MSG_QUEUE_H_H

#include <string>

//https://xidui.github.io/2015/08/23/python-%E8%B0%83%E7%94%A8C-ctypes%E5%BA%93%E5%B0%9D%E8%AF%95/
extern "C" {
    int msg_enqueue(char * _msg);

    int start_send_task();

    const char * test(char *str);

}

#endif //CCPY_SEND_MSG_QUEUE_H_H
