#ifndef _CZU_H_PACK_UTIL_H_
#define _CZU_H_PACK_UTIL_H_

#include "clog.h"
#include "net_buffer_reader.h"
#include "pack_base.h"
#include <arpa/inet.h>
#include <cstring>
#include <stdint.h>




namespace czu{

    class PackUtil:protected NetBufferReader
    {
    public:
        PackUtil(){}
        virtual ~PackUtil(){}

        virtual int OnParsePack(char *_ptr, int _length, PackBase &_pack){
            if (_length <= 0) {
                return 0;
            }

            char *buffer =  _ptr ;

            int start_flag = read_int32(buffer);
            if ( start_flag != PackBase::headflag_){
                LOGE("pack error, start_flag:%d; supposed:%d", start_flag, PackBase::headflag_);
                return -1;
            }
            if(_length<PackBase::pack_head_length_){
//                LOGD("not enough for a whole pack.");
                return 0;
            }
            _pack.cmd_ = read_int32(buffer);
            _pack.sequence_ =  read_int32(buffer);
            _pack.userid_ = read_int32(buffer);
            _pack.reserved_ = read_int32(buffer);
            _pack.length_ =  read_int32(buffer);
            if (_length>= (buffer-_ptr)+_pack.length_ ){
                std::shared_ptr<char> pbody( new char[ _pack.length_ + 1 ] );
                memcpy(pbody.get(), buffer , _pack.length_);
                _pack.body = pbody ;
                return (int) (buffer-_ptr +_pack.length_ );
            } else{
//                LOGD("not enough buffer, continue read.length_:%d; (buffer-_ptr)+_pack.length_ :%d", _pack.length_, (buffer-_ptr)+_pack.length_ );
                return 0;
            }

        }

        virtual int OnSerializePack(PackBase& _data, char *&_buffer_ptr){
//            LOGD("AAA");
            int total_len = PackBase::pack_head_length_ + _data.length_ ;
//            LOGD("BBB");
            _buffer_ptr = (char*)malloc(total_len* sizeof(char));
//            LOGD("CCC");
            char *ptr = _buffer_ptr ;
//            LOGD("DDDD");
            write_int32(ptr,_data.headflag_);
//            LOGD("EEE");
            write_int32(ptr,_data.cmd_);
//            LOGD("FFF");
            write_int32(ptr,_data.sequence_);
//            LOGD("GGG");
            write_int32(ptr,_data.userid_);
//            LOGD("HHH");
            write_int32(ptr,_data.reserved_);
//            LOGD("III");
            write_int32(ptr,_data.length_);
//            LOGD("JJJ%d",_data.length_);
            write_array(ptr,_data.body.get(), _data.length_);
//            LOGD("KKK");
//            free(_buffer_ptr);

            return total_len;
        }

    };

}


#endif