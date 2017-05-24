//
// Created by rotencode on 5/22/17.
//

#include "send_msg_queue.h"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <iostream>

using namespace std;
std::queue< string > msg_queue;

// synchronization
std::mutex queue_mutex;
condition_variable condition;

int task_worker();


int msg_enqueue(std::string _msg){

    {
        std::unique_lock<mutex> lock(queue_mutex);
        cout <<"msg enqueue" << _msg <<endl;
        // don't allow enqueueing after stopping the pool
        msg_queue.push(_msg);
    }
    condition.notify_one();
    return 0;
}

int start_send_task(){
    std::thread worker(task_worker);
    worker.detach();
}

int task_worker() {
    std::unique_lock<std::mutex> lock(queue_mutex);

    while (true) {

        condition.wait(lock);

        while (!msg_queue.empty()) {
            std::cout << "consuming " << msg_queue.front() << '\n';
            msg_queue.pop();
        }

    }
}