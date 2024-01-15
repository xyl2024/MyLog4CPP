#ifndef _SINKER_HPP_
#define _SINKER_HPP_

#include "util.hpp"
#include <memory>
#include <cassert>

namespace mylog
{
    /**
     * @brief 日志落地器基类，负责将日志消息输出到目标位置。
     *
     */
    class Sinker
    {
    public:
        using ptr = std::shared_ptr<Sinker>;
        virtual bool run(const char *message, size_t size) = 0;
        virtual ~Sinker() {}
    };

    /**
     * @brief 控制台日志落地器，负责将日志消息输出到标准输出
     *
     */
    class ConsoleSinker : public Sinker
    {
    public:
        virtual bool run(const char *message, size_t size)
        {
            std::cout.write(message, size);
            return std::cout.good();
        }
    };

    /**
     * @brief 文件日志落地器，负责将日志消息输出到指定文件
     *
     */
    class FileSinker : public Sinker
    {
    public:
        // "./path/to/file"
        // "/path/to/file"
        // 提取路径，如果实际没有路径的话要创建路径
        FileSinker(const std::string &file)
            : _file(file)
        {
            // 在打开文件之前，要创建目录，因为可能目录不存在
            if(!FileUtil::exists(FileUtil::getPath(_file)))
            {
                FileUtil::createDir(_file);
            }

            // 以追加方式打开文件
            _ofs.open(_file, std::ios::binary | std::ios::app);
            assert(_ofs.is_open());
        }
        ~FileSinker()
        {
            // Note that any open file is automatically closed when the ofstream object is destroyed.
            // if(_ofs.is_open())
                // _ofs.close();
        }


        virtual bool run(const char* message, size_t size)
        {
            _ofs.write(message, size);
            return _ofs.good();
        }


    private:
        std::string _file;
        std::ofstream _ofs; // 写入文件的句柄
    };

    /**
     * @brief 日期文件日志落地器，负责将日志消息输出到以日期分类的文件
     *
     */
    // class DailyFileSinker : public Sinker_Base
    // {
    // };

    /**
     * @brief 创建Sinker对象，并返回该对象的智能指针
     *
     *      采用工厂模式，以支持扩展不同日志输出目标
     */
    struct SinkerCreator
    {
        template <typename SinkerType, typename... Args>
        static Sinker::ptr run(Args &&...args)
        {
            return std::make_shared<SinkerType>(std::forward<Args>(args)...);
        }
    };
}

#endif