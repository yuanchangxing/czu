#include "../net/pack_base.h"

#include "../net/net_base.h"
#include "clog.h"

#include <stdio.h>



using namespace czu;

class Server :public NetBase{
public:
    void OnRecv(int _sockid, PackBase &_pack) {
        send(_sockid,_pack);
    }
};

int main(int argc, char **argv)
{
	/* code */
    LOGD("LOGD hello world, %ld",  sizeof(czu::PackBase));
    LOGT("LOGT hello world, %ld",  sizeof(czu::PackBase));
    LOGE("LOGE hello world, %ld",  sizeof(czu::PackBase));

    LOGI("LOGI hello world, %ld",  sizeof(czu::PackBase));
    LOGW("LOGW hello world, %ld",  sizeof(czu::PackBase));
    LOGF("LOGF hello world, %ld",  sizeof(czu::PackBase));


    return 0;
}

