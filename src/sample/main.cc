#include <stdio.h>
#include "../net/pack_base.h"

#include "../net/net_base.h"

class Server :public NetBase{
public:
    void OnRecv(int _sockid, PackBase &_pack) {
        send(_sockid,_pack);
    }
};

int main(int argc, char **argv)
{
	/* code */
    printf("hello world, %ld" , sizeof(czu::PackBase));
	return 0;
}

