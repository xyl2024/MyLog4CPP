#ifndef _FORMAT_HPP_
#define _FORMAT_HPP_

#include "logInfo.hpp"
#include "level.hpp"
#include <string>
#include <vector>
#include <memory>
#include <sstream>
#include <cassert>
#include <ctime>

namespace logFormatHelper
{
    using namespace mylog;

    /**
     * @struct Item
     * @brief Item抽象类代表格式化字符串中每个子项
     */
    struct Item
    {
        using ptr = std::shared_ptr<Item>;
        // 将info中特定字段写进ostream中
        virtual void getItem(std::ostream &os, const LogInfo &info) = 0;
        virtual ~Item() {}
    };

    // Item抽象类的各个派生类，代表格式化字符串中的某个子项
    // %t
    struct TimeItem : public Item
    {
        virtual void getItem(std::ostream &os, const LogInfo &info)
        {
            std::string str = "";
            time_t timestamp = info.timestamp;
            tm *timeinfo = localtime(&timestamp);

            if (timeinfo)
            {
                str += std::to_string(timeinfo->tm_year + 1900);
                str += "-";
                str += std::to_string(timeinfo->tm_mon + 1);
                str += "-";
                str += std::to_string(timeinfo->tm_mday);
                str += " ";
                str += std::to_string(timeinfo->tm_hour);
                str += ":";
                str += std::to_string(timeinfo->tm_min);
                str += ":";
                str += std::to_string(timeinfo->tm_sec);
            }
            os << str;

            // const char *format = "%D-%H:%M:%S";
            // time_t t = info.timestamp;
            // struct tm lt;
            // localtime_r(&t, &lt);
            // char tmp[128];
            // strftime(tmp, 127, format, &lt);
            // os << tmp;
        }
    };

    // %c
    struct LevelItem : public Item
    {
        virtual void getItem(std::ostream &os, const LogInfo &info)
        {
            os << Level::toString(info.level);
        }
    };

    // %n
    struct LoggerNameItem : public Item
    {
        virtual void getItem(std::ostream &os, const LogInfo &info)
        {
            os << info.loggerName;
        }
    };

    // %i
    struct ThreadIDItem : public Item
    {
        virtual void getItem(std::ostream &os, const LogInfo &info)
        {
            os << info.tid;
        }
    };

    // %f
    struct FileItem : public Item
    {
        virtual void getItem(std::ostream &os, const LogInfo &info)
        {
            os << info.file;
        }
    };

    // %l
    struct LineItem : public Item
    {
        virtual void getItem(std::ostream &os, const LogInfo &info)
        {
            os << std::to_string(info.line);
        }
    };

    // %m
    struct MessageItem : public Item
    {
        virtual void getItem(std::ostream &os, const LogInfo &info)
        {
            os << info.message;
        }
    };

    struct SpaceItem : public Item
    {
        virtual void getItem(std::ostream &os, const LogInfo &info)
        {
            std::string str = " ";
            os << str;
        }
    };

    struct TabItem : public Item
    {
        virtual void getItem(std::ostream &os, const LogInfo &info)
        {
            std::string str = "\t";
            os << str;
        }
    };

    struct NLineItem : public Item
    {
        virtual void getItem(std::ostream &os, const LogInfo &info)
        {
            std::string str = "\n";
            os << str;
        }
    };

    struct OtherItem : public Item
    {
        OtherItem(const std::string &str)
            : _str(str)
        {
        }
        virtual void getItem(std::ostream &os, const LogInfo &info)
        {
            os << _str;
        }

    private:
        std::string _str;
    };
} // namespace logFormatHelper

namespace mylog
{
    const std::string DEFAULT_FORMAT = "[%t][%c][%n][%i][%f%S:%S%l]%T%m%N";

    /**
     * @class Formatter
     * @brief 格式化器，将日志信息按照指定的格式输出
     *
     * 规则如下：
     *  %t: 时间
     *  %c: 日志等级
     *  %n: 日志器名称
     *   %i: 线程id
     *   %f: 文件名
     *   %l: 行号
     *   %m: 日志主体信息
     *   %S: 空格
     *   %T: 制表符缩进
     *   %N: 换行
     *   %%: %
     *   其他字符：原样输出
     */
    class Formatter
    {
    public:
        using ptr = std::shared_ptr<Formatter>;
        
        Formatter(const std::string &format = DEFAULT_FORMAT)
            : _format(format)
        {
            assert(fillItems());
        }

        /// @brief 运行格式化功能，提取日志信息各项要素按指定格式输出字符串
        /// @param info 日志信息
        /// @return 格式化后的字符串
        std::string run(const LogInfo &info)
        {
            std::stringstream ss;
            this->run(ss, info);
            return ss.str();
        }

        /// @brief 运行格式化功能，提取日志信息各项要素按指定格式输出到指定输出流
        /// @param os 输出流对象
        /// @param info 日志信息
        /// @return 输出流对象
        std::ostream &run(std::ostream &os, const LogInfo &info)
        {
            for (auto &it : _items)
                it->getItem(os, info);
            return os;
        }

    private:
        /// @brief 解析 _format 串，获得每个格式化Item，按顺序添加进 _items 里
        /// @return  是否解析成功
        bool fillItems();

    private:
        std::string _format;                            // 格式化规则字符串 %d %p...
        std::vector<logFormatHelper::Item::ptr> _items; // 按_format的规则按顺序存放每个项
    };

    bool Formatter::fillItems()
    {
        int i = 0, n = _format.size();
        while (i < n)
        {
            if (_format[i] != '%')
            {
                std::string tmp = "";
                while (i < n && _format[i] != '%')
                    tmp += _format[i++];
                _items.push_back(
                    logFormatHelper::Item::ptr(new logFormatHelper::OtherItem(tmp)));
            }
            else
            {
                ++i;
                if (i == n)
                    return false;
                switch (_format[i])
                {
                case 't':
                    _items.push_back(
                        logFormatHelper::Item::ptr(new logFormatHelper::TimeItem()));
                    break;
                case 'c':
                    _items.push_back(
                        logFormatHelper::Item::ptr(new logFormatHelper::LevelItem()));
                    break;
                case 'n':
                    _items.push_back(
                        logFormatHelper::Item::ptr(new logFormatHelper::LoggerNameItem()));
                    break;
                case 'i':
                    _items.push_back(
                        logFormatHelper::Item::ptr(new logFormatHelper::ThreadIDItem()));
                    break;
                case 'f':
                    _items.push_back(
                        logFormatHelper::Item::ptr(new logFormatHelper::FileItem()));
                    break;
                case 'l':
                    _items.push_back(
                        logFormatHelper::Item::ptr(new logFormatHelper::LineItem()));
                    break;
                case 'm':
                    _items.push_back(
                        logFormatHelper::Item::ptr(new logFormatHelper::MessageItem()));
                    break;
                case 'S':
                    _items.push_back(
                        logFormatHelper::Item::ptr(new logFormatHelper::SpaceItem()));
                    break;
                case 'T':
                    _items.push_back(
                        logFormatHelper::Item::ptr(new logFormatHelper::TabItem()));
                    break;
                case 'N':
                    _items.push_back(
                        logFormatHelper::Item::ptr(new logFormatHelper::NLineItem()));
                    break;
                case '%':
                    _items.push_back(
                        logFormatHelper::Item::ptr(new logFormatHelper::OtherItem("%")));
                    break;
                default:
                    return false;
                }
                ++i;
            }
        }
        return true;
    }
}

#endif