#ifndef _CZU_H_PACKBASE_H_
#define _CZU_H_PACKBASE_H_


#ifndef _CZU_HEADFLAG_
#define _CZU_HEADFLAG_ 7777777

class PackBase
{
public:
	PackBase();
	~PackBase();

	/*
	 *[0,4)
	 * flag for a pack.
	 */
	const int headflag_ = _CZU_HEADFLAG_ ;

	/*
	 *[4,8)
	 * commnad id.
	 */
	int cmd_ ;

	/*
	 *[8,12)
	 */
	int sequence_ ;


	/*
	 *[12,16)
	 *
	 */
	int userid_ ;


	/*
	 *[16,20)
	 */
	int reserved_ ;

	/*
	 *[20,24)
	 *length for body 
	 */
	int length_ ;


};

#endif


#endif