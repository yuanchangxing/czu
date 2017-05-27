//
// Created by rotecode on 5/23/17.
//

#include "send_msg_queue.h"
#include <thread>
#include <chrono>
using namespace std;
//using namespace std::chrono_literals;

int
main(int argc, char **argv){
    start_send_task();
    int count ;
    while (true){
        ++count;
        msg_enqueue(to_string(count));
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}
