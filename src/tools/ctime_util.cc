#include "ctime_util.h"
#include <sstream>
#include <locale>
#include <iomanip>

using namespace std;

namespace czu {
    std::string timestamp_datetime(){
        std::string datetime;
        char datatimetmp[25] = "";
        time_t t;
        tm *tmp;

        t = time(nullptr);
        tmp = localtime(&t);

        strftime(datatimetmp, 24, "%Y-%m-%d %H:%M:%S", tmp);
        datetime = datatimetmp;

        return datetime;
    }
    string timestamp_str(){
        return to_string(time(nullptr));
    }

    int timestamp_int(){
        return time(nullptr);
    }




    /*******************************************
     * convert a string like 2016:04:06 11:22:30 to unix time stamp like 11202030200
     *
     */
    int datetime_to_timestamp(const std::string &_datetime){
#if __GNUC__ > 40900
        std::tm t = {};
        std::istringstream ss(_datetime.c_str());
        ss>>std::get_time(&t, "%Y-%m-%d %H:%M:%S");
         return mktime (&t);
#else
        struct tm tm;
        strptime(_datetime.c_str(),"%Y-%m-%d %H:%M:%S", &tm);
        return  mktime(&tm);
#endif
        return 0;
    }

    int tm_to_int(std::tm &_tm){

        return mktime (&_tm);
    }



}

