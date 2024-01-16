#ifndef _UTIL_HPP_
#define _UTIL_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

namespace mylog
{
    class TimeUtil
    {
    public:
        // 获取当前系统时间（时间戳）
        static time_t getTime()
        {
            return (time_t)time(nullptr);
        }
    };
    class FileUtil
    {
    public:
        // 输入："/path/to/file.txt"
        static bool exists(const std::string &path)
        {
            std::ifstream ifs(path.c_str(), std::ifstream::in);
            bool res = ifs.good();
            ifs.close();
            return res;
        }
        // 输入：
        // "/path/to/file.txt"
        // "./path/to/file.txt"
        // "file.txt"
        // "./file.txt"
        // find_last_of("/\\"), 如果找到直接返回前面的，找不到则返回"."
        static std::string getPath(const std::string &path)
        {
            size_t pos = path.find_last_of("/\\");
            if (pos == std::string::npos)
                return ".";
            return path.substr(0, pos - 0 + 1);
        }

        // 将输入的路径字符串的路径创建出来
        // 输入：
        // "/path/to/file.txt"
        // "./path/to/file.txt"
        // "file.txt"
        // "./file.txt"
        //
        static void createDir(const std::string &file)
        {
            std::string path = getPath(file);
            if (path.empty())
                return;
            if (exists(path))
                return;
            size_t pos, idx = 0;
            while (idx < path.size())
            {
                pos = path.find_first_of("/\\", idx);
                if (pos == std::string::npos)
                {
                    mkdir(path.c_str(), 0755);
                    return;
                }
                if (pos == idx)
                {
                    idx = pos + 1;
                    continue;
                }
                std::string subdir = path.substr(0, pos);
                if (subdir == "." || subdir == "..")
                {
                    idx = pos + 1;
                    continue;
                }
                if (exists(subdir))
                {
                    idx = pos + 1;
                    continue;
                }
                mkdir(subdir.c_str(), 0755);
                idx = pos + 1;
            }
        }
    };
}

#endif