#include "../net/pack_base.h"

#include "../net/net_base.h"
#include "clog.h"

#include <stdio.h>
#include <Python.h>

using namespace czu;


//调用输出"Hello World"函数
void callpy() {
    Py_Initialize();              //使用python之前，要调用Py_Initialize();这个函数进行初始化
    PyRun_SimpleString("import sys");
    PyRun_SimpleString("sys.path.append('./entry')");
    PyObject * pModule = NULL;    //声明变量
    PyObject * pFunc = NULL;      //声明变量
    pModule =PyImport_ImportModule("main");              //这里是要调用的Python文件名
    pFunc= PyObject_GetAttrString(pModule, "entry_py");   //这里是要调用的函数名
    PyEval_CallObject(pFunc, NULL);                         //调用函数,NULL表示参数为空
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

