//
// Created by songtzu on 5/6/17.
//

#include "pyc_server.h"
#include <thread>

using namespace std;

namespace czu {

    int PycServer::load_py() {

    }

    void PycServer::py_monitor() {

        load_py();
    }


    int PycServer::start_server(const char *_ip, const int _port, const char *_py_path, const char *_py_module,
                                const char *_py_func) {
        py_path_ = _py_path;
        py_module_ = _py_module;
        py_func_ = _py_func;
        ip_ = _ip;
        port_ = _port;

        load_py();

        NetBase::start_server(ip_, port_);

        std::thread thread_file_monitor(  &PycServer::py_monitor, this );// c11 create a thread to monitor file path.
        thread_file_monitor.detach();

    }


    void PycServer::OnRecv(int _sockid, PackBase &_pack) {

    }


}