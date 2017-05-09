//
// Created by songtzu on 5/6/17.
//

#include "pyc_server.h"
#include <thread>
#include <sys/inotify.h>


using namespace std;

namespace czu {

    PycServer::~PycServer() {
        py_release();
    }


    int PycServer::py_load() {
        lock_guard<mutex> lck(mtx_py_);

        if (ptr_module_ != nullptr) {
            py_release();
        }

        Py_Initialize();                                                //py parser init.
        PyRun_SimpleString("import sys");
        char path_str[512] = {'\0'};
        sprintf(path_str, "sys.path.append('%s')", py_path_.c_str());
        string append_path = path_str;
        PyRun_SimpleString(append_path.c_str());

        ptr_module_ = PyImport_ImportModule(py_module_.c_str());                      //python module file name.
        ptr_func_ = PyObject_GetAttrString(ptr_module_, py_func_.c_str());       //python function name.

    }


    void PycServer::py_release() {
        Py_DECREF(ptr_module_);                                          //clean up.
        ptr_module_ = nullptr;
        Py_DECREF(ptr_func_);
        ptr_func_ = nullptr;
//        Py_DECREF(ptr_parameters);
//        Py_DECREF(ptr_result);

        Py_Finalize();
    }


    void PycServer::py_monitor() {

        int fd = inotify_init();
        int wd = inotify_add_watch(fd, py_path_.c_str(),
                                   IN_CREATE | IN_DELETE | IN_MOVED_FROM | IN_MOVED_TO | IN_MODIFY);

        if (wd < 0)
            perror("inotify_add_watch");

        int buffer_len = 1024;
        char buffer[1024] = {'\0'};
        int event_size = sizeof(struct inotify_event);

        while (true) {
            int i = 0;

            ssize_t len = read(fd, buffer, buffer_len);
            if (len < 0) {
                LOGE("inotify read error");
                break;
            }
            LOGD("inotify:%s", buffer);
            while (i < buffer_len) {
                struct inotify_event *event = (struct inotify_event *) &buffer[i];

                if (event->len) {
                    if (event->mask & IN_CREATE || event->mask & IN_DELETE || event->mask & IN_MOVED_FROM ||
                        event->mask & IN_MOVED_TO || event->mask & IN_MODIFY) {
                        LOGI("inotify found event for file system change. reload python.");
                        py_load();
                    }
                }
                i += event_size + event->len;
            }

        }
        int ret = inotify_rm_watch(fd, wd);
        close(fd);

    }


    void PycServer::py_execute(int _sock_fd, PackBase _pack) {

        PyObject *ptr_parameters = nullptr;
        PyObject *ptr_result = nullptr;
        const char *ptr_buffer = nullptr;
        int code = 0;
        ptr_parameters = PyTuple_New(7);
        PyTuple_SetItem(ptr_parameters, 0, Py_BuildValue("i", 7777777));
        PyTuple_SetItem(ptr_parameters, 1, Py_BuildValue("i", 1));
        PyTuple_SetItem(ptr_parameters, 2, Py_BuildValue("i", 2));
        PyTuple_SetItem(ptr_parameters, 3, Py_BuildValue("i", 3));
        PyTuple_SetItem(ptr_parameters, 4, Py_BuildValue("i", 4));
        PyTuple_SetItem(ptr_parameters, 5, Py_BuildValue("i", 5));
        PyTuple_SetItem(ptr_parameters, 6, Py_BuildValue("s", "hi this is buffer"));

        ptr_result = PyEval_CallObject(ptr_func_, ptr_parameters);       //call function.
        if (ptr_result) {
            if (PyArg_ParseTuple(ptr_result, "is", &code, &ptr_buffer)) {
                LOGD("code:%d, buffer:%s", code, ptr_buffer);
            } else {
                LOGE("parse error");
            }
        } else {
            LOGE("python return value ", ptr_result);
        }

        if (ptr_parameters != nullptr) {
            Py_DECREF(ptr_parameters);
        }
        if (ptr_result != nullptr) {
            Py_DECREF(ptr_result);
        }

    }


    int PycServer::start_server(const char *_ip, const int _port, const char *_py_path, const char *_py_module,
                                const char *_py_func) {
        py_path_ = _py_path;
        py_module_ = _py_module;
        py_func_ = _py_func;
        ip_ = _ip;
        port_ = _port;

        py_load();
//        py_execute();


        NetBase::start_server(ip_, port_);

        std::thread thread_file_monitor(&PycServer::py_monitor, this);// c11 create a thread to monitor file path.
        thread_file_monitor.detach();


    }


    void PycServer::OnRecv(int _sock_id, PackBase &_pack) {
        threadpool_.enqueue(&PycServer::py_execute,this, _sock_id, _pack);
//        py_execute();
    }


}