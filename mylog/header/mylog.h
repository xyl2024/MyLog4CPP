#ifndef _MYLOG_H_
#define _MYLOG_H_
#include "logger.hpp"
namespace mylog
{
    Logger::ptr getLogger(const std::string &name)
    {
        return LoggerManager::getInstance().get(name);
    }
    Logger::ptr defaultLogger()
    {
        return LoggerManager::getInstance().defaultConsoleLogger();
    }

#define debug(fmt, ...) debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define info(fmt, ...) info(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define warn(fmt, ...) warn(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define error(fmt, ...) error(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define fatal(fmt, ...) fatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define DEBUG(fmt, ...) defaultLogger()->debug(fmt, ##__VA_ARGS__)
#define INFO(fmt, ...) defaultLogger()->info(fmt, ##__VA_ARGS__)
#define WARN(fmt, ...) defaultLogger()->warn(fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...) defaultLogger()->error(fmt, ##__VA_ARGS__)
#define FATAL(fmt, ...) defaultLogger()->fatal(fmt, ##__VA_ARGS__)
} // namespace mylog

#endif