#ifndef _CZU_H_PACK_UTIL_H_
#define _CZU_H_PACK_UTIL_H_

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
                printf("pack error");
                return -1;
            }
            if(_length<PackBase::pack_head_length_){
                printf("not enough for a whole pack.");
                return 0;
            }
            _pack.userid_ = read_int32(buffer);
            _pack.cmd_ = read_int32(buffer);
            _pack.sequence_ =  read_int32(buffer);
            _pack.reserved_ = read_int32(buffer);
            _pack.length_ =  read_int32(buffer);
            if (_length>= (buffer-_ptr)+_pack.length_ ){
                std::shared_ptr<char> pbody( new char[ _pack.length_ + 1 ] );
                memcpy(pbody.get(), buffer , _pack.length_);
                _pack.body = pbody ;
                return (int) (buffer-_ptr +_pack.length_ );
            } else{
                printf("not enough buffer, continue read.");
                return 0;
            }

        }

        virtual int OnSerializePack(PackBase& _data, char *_buffer_ptr){

            int total_len = PackBase::pack_head_length_ + _data.length_ ;
            _buffer_ptr = (char*)malloc(total_len* sizeof(char));
            char *ptr = _buffer_ptr ;
            write_int32(ptr, _data.headflag_);
            write_int32(ptr,_data.cmd_);
            write_int32(ptr,_data.sequence_);
            write_int32(ptr,_data.userid_);
            write_int32(ptr,_data.reserved_);
            write_int32(ptr,_data.length_);
            write_array(ptr,_data.body.get(), _data.length_);


            return total_len;
        }

    };

}


#endif