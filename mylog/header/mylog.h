#ifndef _MYLOG_H_
#define _MYLOG_H_
#include "logger.hpp"
namespace mylog
{
typedef __mylog::LoggerBuilder LoggerBuilder;
typedef __mylog::GlobalLoggerBuilder GlobalLoggerBuilder;
typedef __mylog::Logger Logger;
typedef __mylog::Level Level;
typedef __mylog::ConsoleSinker ConsoleSinker;
typedef __mylog::FileSinker FileSinker;

    /// @brief 根据日志器名称获取日志器
    Logger::ptr getLogger(const std::string &name)
    {
        return __mylog::LoggerManager::getInstance().get(name);
    }
    /// @brief 获取默认日志器
    Logger::ptr defaultLogger()
    {
        return __mylog::LoggerManager::getInstance().defaultConsoleLogger();
    }

#define debug_log(fmt, ...) debug(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define info_log(fmt, ...) info(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define warn_log(fmt, ...) warn(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define error_log(fmt, ...) error(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define fatal_log(fmt, ...) fatal(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

#define DEBUG_LOG(fmt, ...) defaultLogger()->debug_log(fmt, ##__VA_ARGS__)
#define INFO_LOG(fmt, ...) defaultLogger()->info_log(fmt, ##__VA_ARGS__)
#define WARN_LOG(fmt, ...) defaultLogger()->warn_log(fmt, ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...) defaultLogger()->error_log(fmt, ##__VA_ARGS__)
#define FATAL_LOG(fmt, ...) defaultLogger()->fatal_log(fmt, ##__VA_ARGS__)
} // namespace mylog

#endif