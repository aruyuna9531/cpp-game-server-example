#pragma once

#include "common.h"

#include <log4cplus/logger.h>   
#include <log4cplus/tstring.h>
#include <log4cplus/fileappender.h>   
#include <log4cplus/consoleappender.h>   
#include <log4cplus/loggingmacros.h>

#define LogColorRed		31
#define LogColorGreen	32
#define LogColorYellow	33
#define LogColorBlue	34
#define LogColorPurple	35
#define LogColorCyan	36

#define LogColorGrey	90	// dark grey

#define LogColorRedEx		91 // 亮色
#define LogColorGreenEx		92
#define LogColorYellowEx	93
#define LogColorBlueEx		94
#define LogColorPurpleEx	95
#define LogColorCyanEx		96

#define LOG_TRACE(...) { \
    LOG4CPLUS_TRACE( log4cplus::Logger::getRoot( ), __VA_ARGS__ ); \
}

#define LOG_DEBUG(...) { \
    LOG4CPLUS_DEBUG( log4cplus::Logger::getRoot( ),"\33[0;" << LogColorGreenEx << "m" <<  __VA_ARGS__ << "\33[0m" ); \
}

#define LOG_INFO(...)  { \
    LOG4CPLUS_INFO( log4cplus::Logger::getRoot( ), "\33[0;" << LogColorYellowEx << "m" <<  __VA_ARGS__ << "\33[0m" );  \
}

#define LOG_WARN(...)  { \
    LOG4CPLUS_WARN( log4cplus::Logger::getRoot( ),  "\33[0;" << LogColorBlueEx << "m" <<  __VA_ARGS__ << "\33[0m" );\
}

#define LOG_ERROR(...)  { \
    LOG4CPLUS_ERROR( log4cplus::Logger::getRoot( ),  "\33[0;" << LogColorRedEx << "m" <<  __VA_ARGS__ << "\33[0m" );\
}

#define LOG_FATAL(...)  { \
    LOG4CPLUS_FATAL( log4cplus::Logger::getRoot( ),  "\33[0;" << LogColorRedEx << "m" <<  __VA_ARGS__ << "\33[0m" );\
}

#define LOG_COLOR( colorEx, msg )  { \
    LOG4CPLUS_DEBUG( log4cplus::Logger::getRoot( ),  "\33[0;" << colorEx << "m" <<  msg << "\33[0m" );\
}

#define LOG_GM(...) { \
        log4cplus::Logger gmLogger = log4cplus::Logger::getInstance( LOG4CPLUS_STRING_TO_TSTRING("GM") ); \
        LOG4CPLUS_INFO( gmLogger, __VA_ARGS__ ); \
    }

#if false
#define LOG_HTTP_OPEN 1
#define LOG_HTTP(...)  { LOG_COLOR( LogColorYellowEx, __VA_ARGS__ ) }
#endif

#if false
#define LOG_MSG_OPEN 1
bool IsLogShowMsg(int msgId);
#define LOG_MSG(...)  { LOG_COLOR( LogColorGrey, __VA_ARGS__ ) }
#endif 

#if false
#define LOG_SYSOBJ_OPEN 1
#define LOG_SYSOBJ(...)  { LOG_COLOR( LogColorCyanEx, __VA_ARGS__ ) }
#endif

#if false
#define LOG_REDIS_OPEN 1
#define LOG_REDIS(...)  { LOG_COLOR( LogColorGreenEx, __VA_ARGS__ ) }
#endif

#if false
#define LOG_NETWORK_OPEN 1
#define LOG_NETWORK(...)  { LOG_COLOR( LogColorYellowEx, __VA_ARGS__ ) }
#endif

namespace Log4Help {
    std::string GetMsgIdName(Proto::MsgId msgId);
    std::string GetMsgIdName(int msgId);
}