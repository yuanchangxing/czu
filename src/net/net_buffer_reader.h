
#ifndef _CZU_H_NET_BUFFER_READER_H_
#define _CZU_H_NET_BUFFER_READER_H_

#include <zconf.h>
#include <arpa/inet.h>
#include <string.h>


namespace czu {

	class NetBufferReader {
	public:
		NetBufferReader() {};

		virtual ~NetBufferReader() {};


		/*
         * int 32 operator.
         */
		inline int read_int32(char *&_ptr) {
            int val = ntohl(uint32_t( *((int*)_ptr)));
			_ptr += sizeof(int) ;
            return val;
        }

		inline void write_int32(char *&_ptr, int _value) {
            int bigendian_value = htonl(_value);
            memcpy(_ptr, (char*)(&(bigendian_value)), sizeof(int));
            _ptr += sizeof(int);
		}

		/*
         *int 16 operator
         */
		inline int read_int16(char *&_ptr) {

		}

		inline void write_int16(const char *_ptr, int _value) {

		}

		/*
         *double 64 operator
         */
		inline double read_double64(char *_ptr) {

		}

		inline void write_double64(char *_ptr, double _value) {

		}


		inline char *read_array(char *&_ptr, int _length) {

		}

		inline void write_array(char *&_ptr, const char *_value, const int _length) {
			memcpy(_ptr, _value, _length );
            _ptr+= _length;
		}


	};

}
#endif
