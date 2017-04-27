#ifndef _CZU_H_PACK_UTIL_H_
#define _CZU_H_PACK_UTIL_H_

#include "net_buffer_reader.h"

class PackUtil:protected NetBufferReader
{
public:
	PackUtil();
	~PackUtil();

	virtual int parse_pack()
	
};



#endif