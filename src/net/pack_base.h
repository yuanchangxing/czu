#ifndef _CZU_H_PACKBASE_H_
#define _CZU_H_PACKBASE_H_


#ifndef _CZU_HEADFLAG_
#define _CZU_HEADFLAG_ 7777777


#include <memory>   //shared_ptr


namespace czu {

	class PackBase {
	public:
		PackBase() {}

		virtual ~PackBase() {}

        const static int pack_head_length_ = 24;
		/*
         *[0,4)
         * flag for a pack.
         */
		const static int headflag_ = _CZU_HEADFLAG_;

		/*
         *[4,8)
         * commnad id.
         */
		int cmd_;

		/*
         *[8,12)
         */
		int sequence_;


		/*
         *[12,16)
         *
         */
		int userid_;


		/*
         *[16,20)
         */
		int reserved_;

		/*
         *[20,24)
         *length for body
         */
		int length_;


        std::shared_ptr<char> body ;


    };
}
#endif


#endif