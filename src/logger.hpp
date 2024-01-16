#ifndef _LOGGER_GOO_
#define _LOGGER_GOO_

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "util.hpp"
#include "level.hpp"
#include "sinker.hpp"
#include "logInfo.hpp"
#include "format.hpp"
#include "asyncLoggerCtrl.hpp"
#include <unordered_map>
#include <atomic>
#include <mutex>
#include <cstdarg>

namespace mylog
{
    /**
     * @brief 日志器基类，组织各个模块进行日志的输出
     */
    class Logger
    {
    public:
        enum class Type
        {
            SYNC = 0, // 同步日志器
            ASYNC     // 异步日志器
        };

    protected:
        std::string _name;                   // 日志器名称
        std::atomic<Level::Value> _minLevel; // 日志器最低输出等级
        Formatter::ptr _formatter;           // 格式化器
        std::vector<Sinker::ptr> _sinkers;   // 日志器的多个日志落地方向
        std::mutex _mutex;                   // 互斥锁
    protected:
        Logger(const std::string &name, Level::Value minLevel, Formatter::ptr formatter, std::vector<Sinker::ptr> sinkers)
            : _name(name), _minLevel(minLevel), _formatter(formatter), _sinkers(sinkers.begin(), sinkers.end())
        {
        }

    public:
        using ptr = std::shared_ptr<Logger>;

        /// @brief 把 DEBUG 等级的日志消息输出
        /// @param file 当前文件名
        /// @param line 当前行号
        /// @param format 日志有效载荷的格式，与printf一样
        /// @param  填充占位符
        void debug(const char *file, size_t line, const char *format, ...)
        {
            Level::Value lv = Level::Value::DEBUG;
            // 0.是否可以输出
            if (levelAvailable(lv) == false)
                return;
            // 1.组织日志消息
            va_list al;
            va_start(al, format);
            std::string message = organizeMessage(lv, file, line, format, al);
            va_end(al);
            // 2.日志消息输出
            outPut(message);
        }
        /// @brief 把 INFO 等级的日志消息输出
        /// @param file 当前文件名
        /// @param line 当前行号
        /// @param format 日志有效载荷的格式，与printf一样
        /// @param  填充占位符
        void info(const char *file, size_t line, const char *format, ...)
        {
            Level::Value lv = Level::Value::INFO;
            // 0.是否可以输出
            if (levelAvailable(lv) == false)
                return;
            // 1.组织日志消息
            va_list al;
            va_start(al, format);
            std::string message = organizeMessage(lv, file, line, format, al);
            va_end(al);
            // 2.日志消息输出
            outPut(message);
        }
        /// @brief 把 WARN 等级的日志消息输出
        /// @param file 当前文件名
        /// @param line 当前行号
        /// @param format 日志有效载荷的格式，与printf一样
        /// @param  填充占位符
        void warn(const char *file, size_t line, const char *format, ...)
        {
            Level::Value lv = Level::Value::WARN;
            // 0.是否可以输出
            if (levelAvailable(lv) == false)
                return;
            // 1.组织日志消息
            va_list al;
            va_start(al, format);
            std::string message = organizeMessage(lv, file, line, format, al);
            va_end(al);
            // 2.日志消息输出
            outPut(message);
        }
        /// @brief 把 ERROR 等级的日志消息输出
        /// @param file 当前文件名
        /// @param line 当前行号
        /// @param format 日志有效载荷的格式，与printf一样
        /// @param  填充占位符
        void error(const char *file, size_t line, const char *format, ...)
        {
            Level::Value lv = Level::Value::ERROR;
            // 0.是否可以输出
            if (levelAvailable(lv) == false)
                return;
            // 1.组织日志消息
            va_list al;
            va_start(al, format);
            std::string message = organizeMessage(lv, file, line, format, al);
            va_end(al);
            // 2.日志消息输出
            outPut(message);
        }
        /// @brief 把 FATAL 等级的日志消息输出
        /// @param file 当前文件名
        /// @param line 当前行号
        /// @param format 日志有效载荷的格式，与printf一样
        /// @param  填充占位符
        void fatal(const char *file, size_t line, const char *format, ...)
        {
            Level::Value lv = Level::Value::FATAL;
            // 0.是否可以输出
            if (levelAvailable(lv) == false)
                return;
            // 1.组织日志消息
            va_list al;
            va_start(al, format);
            std::string message = organizeMessage(lv, file, line, format, al);
            va_end(al);
            // 2.日志消息输出
            outPut(message);
        }

    protected:
        /// @brief 判断是否可以日志输出。当前等级大于或等于最小限制等级时才可以输出。
        /// @param lv 当前等级
        /// @return 是否可以日志输出
        bool levelAvailable(const Level::Value &lv)
        {
            return lv >= _minLevel;
        }

        /// @brief 提取日志有效信息并构造日志信息，返回格式化后的日志消息
        /// @param lv 当前等级
        /// @param file 当前文件名
        /// @param line 当前行号
        /// @param format 日志有效载荷的格式，与printf一样
        /// @param al 不定参数列表
        /// @return 格式化后的日志消息
        std::string organizeMessage(const Level::Value &lv, const char *file, size_t line, const char *format, va_list al)
        {
            // 1.提取出不定参中的日志有效载荷到message中
            char *buf;
            std::string message;
            int n = vasprintf(&buf, format, al);
            if (n < 0)
            {
                message = "Get log message failed.\n";
            }
            else
            {
                message.assign(buf, n);
                free(buf);
            }
            // 2.构造日志信息对象
            LogInfo info(lv, _name, file, line, message);

            // 3.调用Formatter格式化日志信息并返回
            return _formatter->run(info);
        }

        /// @brief 把日志消息输出的函数。具体如何输出，由派生的日志器类决定
        ///
        ///         同步日志器：将日志消息直接输出到指定的sinkers中
        ///         异步日志器：将日志消息放入缓冲区，缓冲区中的日志消息由其他线程处理输出
        /// @param message 格式化后的日志消息
        virtual void outPut(const std::string &message) = 0;
    };

    /**
     * @brief 同步日志器，继承Logger类
     */
    class SyncLogger : public Logger
    {
    public:
        SyncLogger(const std::string &name, Level::Value minLevel, Formatter::ptr formatter, std::vector<Sinker::ptr> sinkers)
            : Logger(name, minLevel, formatter, sinkers)
        {
        }

        /// @brief 把日志消息直接输出到指定的sinkers中。
        /// @param message 格式化后的日志消息
        virtual void outPut(const std::string &message) override
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (!_sinkers.empty())
                for (auto &sinker : _sinkers)
                    sinker->run(message.c_str(), message.size());
            else
            {
                std::cout << "sinkers为空\n";
            }
        }
    };

    /// @brief 异步日志器，继承Logger类
    class AsyncLogger : public Logger
    {
    private:
        AsyncLoggerCtrl::ptr _ctrl;

    public:
        AsyncLogger(const std::string &name, Level::Value minLevel, Formatter::ptr formatter, std::vector<Sinker::ptr> sinkers)
            : _ctrl(std::make_shared<AsyncLoggerCtrl>(std::bind(&AsyncLogger::sink, this, std::placeholders::_1))), Logger(name, minLevel, formatter, sinkers)
        {
        }

    private:
        virtual void outPut(const std::string &message) override
        {
            // 直接往缓冲区放，日志的落地由异步工作线程完成
            _ctrl->push(message);
        }
        void sink(AsyncLoggerCharBuf &logMsg)
        {
            if (!_sinkers.empty())
                for (auto &sinker : _sinkers)
                    sinker->run(logMsg.readBegin(), logMsg.readEnd());
        }
    };

    class LoggerBuilder
    {
    protected:
        Logger::Type _type;                // 日志器类型
        std::string _name;                 // 日志器名称
        Level::Value _minLevel;            // 日志器最低输出等级
        Formatter::ptr _formatter;         // 格式化器
        std::vector<Sinker::ptr> _sinkers; // 日志器的多个日志落地方向

    protected:
        LoggerBuilder()
            : _type(Logger::Type::SYNC), _minLevel(Level::Value::DEBUG)
        {
        }

    public:
        using ptr = std::shared_ptr<LoggerBuilder>;

        void setLoggerName(const std::string &name)
        {
            _name = name;
        }
        void setMinLevel(const Level::Value &lv)
        {
            _minLevel = lv;
        }
        void setLoggerType(const Logger::Type &type)
        {
            _type = type;
        }
        void setFormatter(const std::string &fmt = DEFAULT_FORMAT)
        {
            _formatter = std::make_shared<Formatter>(fmt);
        }
        template <typename SinkType, typename... Args>
        void setSinker(Args &&...args)
        {
            auto sink = SinkerCreator::run<SinkType>(std::forward<Args>(args)...);
            _sinkers.push_back(sink);
        }
        virtual Logger::ptr build() = 0;
    };

    class LocalLoggerBuilder : public LoggerBuilder
    {
    public:
        virtual Logger::ptr build()
        {
            if (_name.empty())
            {
                std::cout << "日志器名称不能为空！\n";
                exit(-1);
            }
            if (_formatter.get() == nullptr)
            {
                std::cout << "当前日志器：" << _name << " 未检测到日志格式，默认设置为[%t][%c][%n][%i][%f%S:%S%l]%T%m%N\n";
                setFormatter();
            }
            if (_sinkers.empty())
            {
                std::cout << "当前日志器：" << _name << " 未检测到落地方向，默认设置为标准输出!\n";
                setSinker<ConsoleSinker>();
            }
            Logger::ptr lp;
            if (_type == Logger::Type::ASYNC)
            {
                lp = std::make_shared<AsyncLogger>(_name, _minLevel, _formatter, _sinkers);
            }
            else
                lp = std::make_shared<SyncLogger>(_name, _minLevel, _formatter, _sinkers);
            return lp;
        }
    };

    /// @brief 管理用户创建的所有日志器
    class LoggerManager
    {
    private:
        std::mutex _mtx;
        Logger::ptr _defaultConsoleLogger;
        std::unordered_map<std::string, Logger::ptr> _loggers;

    private:
        LoggerManager()
        {
            buildDefaultConsoleLogger();
        }

        void buildDefaultConsoleLogger()
        {
            std::unique_ptr<LocalLoggerBuilder> llb(new LocalLoggerBuilder());
            llb->setLoggerName("defaultConsoleSyncLogger");
            llb->setLoggerType(Logger::Type::SYNC);
            _defaultConsoleLogger = llb->build();
            _loggers.insert(std::make_pair("defaultConsoleSyncLogger", _defaultConsoleLogger));
            std::cout << "初始化默认Console日志器成功\n";
        }
        LoggerManager(const LoggerManager &) = delete;
        LoggerManager &operator=(const LoggerManager &) = delete;

    public:
        static LoggerManager &getInstance()
        {
            static LoggerManager e; // C++11编译器实现了静态局部变量构造时的线程安全
            return e;
        }
        void add(const std::string &name, const Logger::ptr logger)
        {
            std::unique_lock<std::mutex> lock(_mtx);
            _loggers.insert(std::make_pair(name, logger));
        }
        bool exists(const std::string &name)
        {
            std::unique_lock<std::mutex> lock(_mtx);
            auto it = _loggers.find(name);
            return (it != _loggers.end());
        }
        Logger::ptr get(const std::string &name)
        {
            std::unique_lock<std::mutex> lock(_mtx);
            auto it = _loggers.find(name);
            if (it == _loggers.end())
                return defaultConsoleLogger();
            return it->second;
        }
        Logger::ptr defaultConsoleLogger()
        {
            std::unique_lock<std::mutex> lock(_mtx);
            return _defaultConsoleLogger;
        }
    };

    class GlobalLoggerBuilder : public LoggerBuilder
    {
    public:
        virtual Logger::ptr build()
        {
            if (_name.empty())
            {
                std::cout << "日志器名称不能为空！\n";
                exit(-1);
            }
            if (_formatter.get() == nullptr)
            {
                std::cout << "当前日志器：" << _name << " 未检测到日志格式，默认设置为[%t][%c][%n][%i][%f%S:%S%l]%T%m%N\n";
                setFormatter();
            }
            if (_sinkers.empty())
            {
                std::cout << "当前日志器：" << _name << " 未检测到落地方向，默认设置为标准输出!\n";
                setSinker<ConsoleSinker>();
            }
            Logger::ptr lp;
            if (_type == Logger::Type::ASYNC)
            {
                lp = std::make_shared<AsyncLogger>(_name, _minLevel, _formatter, _sinkers);
            }
            else
                lp = std::make_shared<SyncLogger>(_name, _minLevel, _formatter, _sinkers);
            LoggerManager::getInstance().add(_name, lp);
            return lp;
        }
    };

}

#endif