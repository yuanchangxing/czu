#ifndef _H_TIME_UTIL_H
#define _H_TIME_UTIL_H
#include <string>
#include <ctime>

#include <time.h>
#include <sstream>


namespace czu {
    std::string timestamp_datetime();
    std::string timestamp_str();
    int timestamp_int();

    template <class T>
    inline std::string to_string (const T& t){
        std::stringstream ss;
        ss << t;
        return ss.str();
    }

    int datetime_to_timestamp(const std::string &_datetime);

    int tm_to_int(std::tm &_tm);



}





#endif // _H_TIME_UTIL_H
