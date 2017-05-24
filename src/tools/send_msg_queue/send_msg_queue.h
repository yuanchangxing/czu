//
// Created by rotencode on 5/22/17.
//

#ifndef CCPY_SEND_MSG_QUEUE_H_H
#define CCPY_SEND_MSG_QUEUE_H_H

#include <string>

extern "C" {
    int msg_enqueue(std::string _msg);

    int start_send_task();

}

#endif //CCPY_SEND_MSG_QUEUE_H_H
