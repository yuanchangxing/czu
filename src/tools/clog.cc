#include "clog.h"
#include "ctime_util.h"


#include <stdio.h>
#include <math.h>
#include <stdarg.h>
#include <mutex>


#define BUF_MAX 1056

enum ColorFont{
    ColorFont_Black = 30,
    ColorFont_Red = 31,
    ColorFont_Green = 32,
    ColorFont_Yellow = 33,
    ColorFont_Blue = 34,
    ColorFont_Purple = 35,
    ColorFont_White = 37,
};
enum ColorBackgroud{
    ColorBackground_Black = 40,
    ColorBackground_Red = 41,
    ColorBackground_Green = 42,
    ColorBackground_Yellow = 43,
    ColorBackground_Blue = 44,
    ColorBackground_Purple = 45,
    ColorBackground_White = 47,
};
#define SCREEN_LOG_FORMAT "\033[%d;%dm%s\033[0m\n"

#define SCREEN_LOG_ONLY_FOREGROUND_FORMAT "\033[%dm%s\033[0m\n"

extern char *__progname;



int log_level , log_type;
std::once_flag flag_log_init_ ;

void clog_init(){

}

void log_set_level(LogLevel _level){
    log_type = _level;
}

void _log_stub(LogLevel _lv,  const char* _file, int _line,  const char* _msg, va_list _ap){
    std::call_once(flag_log_init_, clog_init);


    const char *LogLevelString[] = {  "DEBUG" ,"TRACE", "INFO", "WARN", "ERROR", "FATAL"};
    int bytes = 0;
    char data[BUF_MAX] = "";

    bytes += snprintf(data+bytes, BUF_MAX-1-bytes,
                      "[%s] [%5s] [LINE]:%-4d [FILE]:%s [MSG]:",
                      czu::timestamp_datetime().c_str(),
                      LogLevelString[_lv] , _line, _file);




    if (bytes < BUF_MAX-1){
        bytes += vsnprintf(data+bytes, BUF_MAX-1-bytes, _msg, _ap);
    }
#ifdef   __linux__

    switch (_lv) {
        case Level_Info:
            printf(SCREEN_LOG_ONLY_FOREGROUND_FORMAT, ColorFont_Black, data) ;
            break;
        case Level_Debug:
            printf(SCREEN_LOG_ONLY_FOREGROUND_FORMAT, ColorFont_White, data) ;
            break;
        case Level_Trace:
            printf(SCREEN_LOG_ONLY_FOREGROUND_FORMAT, ColorFont_Blue, data) ;
            break;
        case Level_Warning:
            printf(SCREEN_LOG_ONLY_FOREGROUND_FORMAT, ColorFont_Purple, data) ;
            break;
        case Level_Error:
            printf(SCREEN_LOG_ONLY_FOREGROUND_FORMAT, ColorFont_Yellow, data) ;
            break;
        case Level_Fatal:
            printf(SCREEN_LOG_ONLY_FOREGROUND_FORMAT, ColorFont_Red, data) ;
            break ;
        default:
            printf( "%s\n", data);
            break;
    }
#else
    printf( "%s\n", data);
#endif

    return ;
}

void LogImp(LogLevel l,  const char* _file, int _line, const char* _msg, ...) {
    va_list ap;
    va_start(ap,_msg);
    _log_stub(l, _file, _line, _msg, ap);
    va_end(ap);
}

