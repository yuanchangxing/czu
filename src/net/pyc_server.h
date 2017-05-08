//
// Created by songtzu on 5/6/17.
// a python implemented logic cpp backend framework.
//

#ifndef CZU_PYC_SERVER_H
#define CZU_PYC_SERVER_H

#include "net_base.h"
#include <string>
#include <Python.h>

namespace czu {

    class PycServer : public NetBase {

    public:
        PycServer() {}

        virtual ~PycServer();

        int start_server(const char *_ip, const int _port, const char *_py_path, const char *_py_module,
                         const char *_py_func);


    private:
        int py_load();
        //monitor for python file.
        void py_monitor();

        void py_release();

        void py_execute();

        void OnRecv(int _sockid, PackBase &_pack);


    private:
        PyObject * ptr_module_ = nullptr;                                   //module for py. file name.
        PyObject * ptr_func_ = nullptr;                                     //function name for py.

        std::string py_path_;
        std::string py_module_;
        std::string py_func_ ;

        std::string ip_;
        int port_;

    };
}


#endif //CZU_PYC_SERVER_H
