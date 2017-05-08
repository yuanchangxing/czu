//
// Created by songtzu on 5/6/17.
//

#include "pyc_server.h"
#include <thread>

using namespace std;

namespace czu {

    int PycServer::load_py() {

        Py_Initialize();                                                //py parser init.
        PyRun_SimpleString("import sys");
        char path_str[512] = {'\0'};
        sprintf(path_str,"sys.path.append('%s')", py_path_.c_str());
        string append_path = path_str ;
        PyRun_SimpleString(append_path.c_str());
        PyObject * ptr_module = NULL;                                   //module for py. file name.
        PyObject * ptr_func = NULL;                                     //function name for py.
        PyObject * ptr_parameters = NULL;
        PyObject * ptr_result = NULL;
        const char* ptr_buffer = NULL;
        int code = 0;
        ptr_parameters = PyTuple_New(7);
        PyTuple_SetItem(ptr_parameters, 0, Py_BuildValue("i", 7777777));
        PyTuple_SetItem(ptr_parameters, 1, Py_BuildValue("i", 1));
        PyTuple_SetItem(ptr_parameters, 2, Py_BuildValue("i", 2));
        PyTuple_SetItem(ptr_parameters, 3, Py_BuildValue("i", 3));
        PyTuple_SetItem(ptr_parameters, 4, Py_BuildValue("i", 4));
        PyTuple_SetItem(ptr_parameters, 5, Py_BuildValue("i", 5));
        PyTuple_SetItem(ptr_parameters, 6, Py_BuildValue("s", "hi this is buffer"));



        ptr_module =PyImport_ImportModule("main");                      //python module file name.
        ptr_func= PyObject_GetAttrString(ptr_module, "entry_py");       //python function name.
        ptr_result = PyEval_CallObject(ptr_func, ptr_parameters);       //call function.
        if(ptr_result) {
            if(PyArg_ParseTuple(ptr_result, "is",  &code, &ptr_buffer)) {
                LOGD("code:%d, buffer:%s", code, ptr_buffer);
            } else{
                LOGE("parse error");
            }
        } else{
            LOGE("python return value ",ptr_result);
        }



        Py_DECREF(ptr_module);                                          //clean up.
        Py_DECREF(ptr_func);
        Py_DECREF(ptr_parameters);
        Py_DECREF(ptr_result);

        Py_Finalize();
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