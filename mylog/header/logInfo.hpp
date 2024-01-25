#ifndef _MESSAGE_HPP_
#define _MESSAGE_HPP_

#include "level.hpp"
#include "util.hpp"
#include <thread>

namespace __mylog
{
    /*
        定义日志信息的结构
        这是日志输出时的各项信息
    */
    struct LogInfo
    {
        Level::Value level;     // 日志等级
        time_t timestamp;       // 时间戳，精确到秒
        size_t line;            // 行号
        std::thread::id tid;    // 线程ID
        std::string file;       // 文件名
        std::string loggerName; // 日志器名称
        std::string message;    // 日志主体消息

        // 构造函数
        LogInfo(const Level::Value& level_, const std::string& loggerName_,
        const std::string& file_, size_t line_,const std::string& message_)
            :level(level_)
            ,timestamp(TimeUtil::getTime())
            ,line(line_)
            ,tid(std::this_thread::get_id())
            ,file(file_)
            ,loggerName(loggerName_)
            ,message(message_)
        {}
    };
}

#endif