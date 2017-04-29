//
// Created by songtzu on 4/29/17.
//

#ifndef CZU_CLOG_H
#define CZU_CLOG_H

enum LogLevel{

    Level_Debug  =0,
    Level_Trace =1,
    Level_Info =2,
    Level_Warning =3,
    Level_Error =4,
    Level_Fatal =5,

} ;


void LogImp(  LogLevel l,  const char*file, int line, const  char*msg, ...) ;


void log_set_level(LogLevel _level);



#define LOGD(x, ...) LogImp(Level_Debug, __FILE__, __LINE__,  x , ## __VA_ARGS__)

#define LOGT(x, ...) LogImp(Level_Trace, __FILE__, __LINE__,  x , ## __VA_ARGS__)

#define LOGE(x, ...) LogImp(Level_Error, __FILE__, __LINE__,  x , ## __VA_ARGS__)

#define LOGI(x, ...) LogImp(Level_Info, __FILE__, __LINE__,  x , ## __VA_ARGS__)

#define LOGW(x, ...) LogImp(Level_Warning, __FILE__, __LINE__,  x , ## __VA_ARGS__)

#define LOGF(x, ...) LogImp(Level_Fatal, __FILE__, __LINE__,  x , ## __VA_ARGS__)







#endif //CZU_CLOG_H
