#include "../net/pack_base.h"

#include "../net/net_base.h"
#include "clog.h"

#include <stdio.h>


using namespace czu;

class Server :public NetBase{
public:
    void OnRecv(int _sockid, PackBase &_pack) {
        LOGD("cmd:%d, length:%d, sequence_:%d, body:%s,", _pack.cmd_, _pack.length_, _pack.sequence_, _pack.body.get());
        send_pack(_sockid,_pack);
    }
};


Server server ;


int main(int argc, char **argv)
{
	/* code */
    LOGD("LOGD hello world, %ld",  sizeof(czu::PackBase));
    LOGT("LOGT hello world, %ld",  sizeof(czu::PackBase));
    LOGE("LOGE hello world, %ld",  sizeof(czu::PackBase));

    LOGI("LOGI hello world, %ld",  sizeof(czu::PackBase));
    LOGW("LOGW hello world, %ld",  sizeof(czu::PackBase));
    LOGF("LOGF hello world, %ld",  sizeof(czu::PackBase));

    server.start_server("0.0.0.0",8888);

    return 0;
}

