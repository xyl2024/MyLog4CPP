#ifndef _LEVEL_HPP_
#define _LEVEL_HPP_

namespace mylog
{
#define STRING(val) #val

    struct Level
    {
        enum class Value //C++11新增的枚举类
        {
            DEBUG = 0,
            INFO,
            WARNNING,
            ERROR,
            FATAL,
            OFF
        };

        static std::string toString(Level::Value lv)
        {
            switch (lv)
            {
            case Level::Value::DEBUG:
                return STRING(DEBUG);
            case Level::Value::INFO:
                return STRING(INFO);
            case Level::Value::WARNNING:
                return STRING(WARNNING);
            case Level::Value::ERROR:
                return STRING(ERROR);
            case Level::Value::FATAL:
                return STRING(FATAL);
            case Level::Value::OFF:
                return STRING(OFF);
            default:
                return STRING(UNKNOWN);
            }
        }
    };
}

#endif