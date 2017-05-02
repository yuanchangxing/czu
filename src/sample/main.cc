#include "../net/pack_base.h"

#include "../net/net_base.h"
#include "clog.h"

#include <stdio.h>
#include <Python.h>
#include <iostream>

using namespace czu;


//python integrate sample.
void callpy() {
    Py_Initialize();              //使用python之前，要调用Py_Initialize();这个函数进行初始化
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./entry')");
    PyObject * ptr_module = NULL;    //声明变量
    PyObject * ptr_func = NULL;      //声明变量
    PyObject * ptr_parameters = NULL;
    PyObject * ptr_result = NULL;
    const char* ptr_buffer = NULL;
    int value = 0;
//    pParam = Py_BuildValue("(s)", "hi this is buffer");
    ptr_parameters = PyTuple_New(7);
    PyTuple_SetItem(ptr_parameters, 0, Py_BuildValue("i", 7777777));
    PyTuple_SetItem(ptr_parameters, 1, Py_BuildValue("i", 1));
    PyTuple_SetItem(ptr_parameters, 2, Py_BuildValue("i", 2));
    PyTuple_SetItem(ptr_parameters, 3, Py_BuildValue("i", 3));
    PyTuple_SetItem(ptr_parameters, 4, Py_BuildValue("i", 4));
    PyTuple_SetItem(ptr_parameters, 5, Py_BuildValue("i", 5));

    PyTuple_SetItem(ptr_parameters, 6, Py_BuildValue("s", "hi this is buffer"));






    ptr_module =PyImport_ImportModule("main");              //这里是要调用的Python文件名
    ptr_func= PyObject_GetAttrString(ptr_module, "entry_py");   //这里是要调用的函数名
    ptr_result = PyEval_CallObject(ptr_func, ptr_parameters);                         //
    if(ptr_result) {
        if(PyArg_ParseTuple(ptr_result, "is",  &value, &ptr_buffer)) {
            LOGD("value:%d, buffer:%s", value, ptr_buffer);
//            std::cout <<"value:"<<value<<"ptr_buffer:"<< ptr_buffer << std::endl;
        } else{
            LOGE("parse error");
        }
    } else{
        LOGE("python return value ",ptr_result);
    }
    Py_DECREF(ptr_module);
    Py_DECREF(ptr_func);
    Py_DECREF(ptr_parameters);
    Py_DECREF(ptr_result);


    Py_Finalize();                //调用Py_Finalize,这个和Py_Initialize相对应的.
}


class Server :public NetBase{
public:
    void OnRecv(int _sockid, PackBase &_pack) {
        LOGD("cmd:%d, length:%d, sequence_:%d, body:%s,", _pack.cmd_, _pack.length_, _pack.sequence_, _pack.body.get());
        send_pack(_sockid,_pack);
        callpy();
    }
};


Server server ;


int main(int argc, char **argv)
{
    LOGD("dir:%s", argv[0]);
	/* code */
    LOGD("LOGD hello world, %ld",  sizeof(czu::PackBase));
    callpy();

    LOGT("LOGT hello world, %ld",  sizeof(czu::PackBase));
    LOGE("LOGE hello world, %ld",  sizeof(czu::PackBase));

    LOGI("LOGI hello world, %ld",  sizeof(czu::PackBase));
    LOGW("LOGW hello world, %ld",  sizeof(czu::PackBase));
    LOGF("LOGF hello world, %ld",  sizeof(czu::PackBase));

    server.start_server("0.0.0.0",8888);

    return 0;
}

