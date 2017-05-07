//
// Created by songtzu on 5/6/17.
// a python implemented logic cpp backend framework.
//

#ifndef CZU_PYC_SERVER_H
#define CZU_PYC_SERVER_H

#include "net_base.h"
#include <string>

namespace czu {

    class PycServer : public NetBase {

    public:
        PycServer() {}

        virtual ~PycServer() {}

        int start_server(const char *_ip, const int _port, const char *_py_path, const char *_py_module,
                         const char *_py_func);

        void OnRecv(int _sockid, PackBase &_pack);

    private:
        int load_py();
        //monitor for python file.
        void py_monitor();
        std::string py_path_;
        std::string py_module_;
        std::string py_func_ ;

        std::string ip_;
        int port_;

    };
}


#endif //CZU_PYC_SERVER_H
