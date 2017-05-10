//cpp & py integrate sample.
//
// Created by songtzu on 5/8/17.
//

#include "clog.h"
#include "pyc_server.h"

using namespace czu;

class Server :public PycServer{
//public:
//    void OnRecv(int _sockid, PackBase &_pack) {
//        LOGD("cmd:%d, length:%d, sequence_:%d, body:%s,", _pack.cmd_, _pack.length_, _pack.sequence_, _pack.body.get());
//        send_pack(_sockid,_pack);
//    }
};




int main(int argc, const char ** argv){
    LOGI("main function, %s", argv[0]);

    Server server ;
    server.start_server("0.0.0.0", 8008,"./entry","main", "entry_py");
    while(1){
        sleep(1000);
    }

}