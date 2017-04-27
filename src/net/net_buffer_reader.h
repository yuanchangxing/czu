
#ifndef _CZU_H_NET_BUFFER_READER_H_
#define _CZU_H_NET_BUFFER_READER_H_

class NetBufferReader
{
public:
	NetBufferReader();
	~NetBufferReader();
	

	/*
	 * int 32 operator.
	 */
	inline int read_int32( const char *_ptr ){

	}
	inline write_int32(char *_ptr, int _value){
		
	}

	/*
	 *int 16 operator
	 */
	inline int read_int16( const char *_ptr ){

	}
	inline write_int16( const char *_ptr, int _value ){

	}

	/*
	 *double 64 operator
	 */
	inline double read_double64( const char *_ptr ){

	}
	inline write_double64( char*_ptr, double _value){

	}



	inline char* read_array(const int _length){

	}

	inline write_array(char *_ptr, const char *_value){

	}


};

#endif
