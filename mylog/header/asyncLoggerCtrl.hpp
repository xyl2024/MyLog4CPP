#ifndef _ASYNCLOGGERCTRL_HPP_
#define _ASYNCLOGGERCTRL_HPP_
#include "asyncLoggerCharBuf.hpp"
#include <mutex>
#include <condition_variable>
#include <thread>
#include <functional>
#include <unistd.h>
namespace mylog
{
    class AsyncLoggerCtrl
    {
    public:
        using ptr = std::shared_ptr<AsyncLoggerCtrl>;
        using sink_func_t = std::function<void(AsyncLoggerCharBuf &buf)>;
    private:
        AsyncLoggerCharBuf _consumeBuf; // 消费者缓冲区
        AsyncLoggerCharBuf _produceBuf; // 生产者缓冲区

        std::mutex _mtx;                      // 控制生产者与消费者互斥
        std::condition_variable _consumeCond; // 控制生产者与消费者同步
        std::condition_variable _produceCond; // 控制生产者与消费者同步

        bool _stoped;  // 退出标志，异步日志控制器要退出时设置为true
        sink_func_t _sink;          // 实际日志落地的函数，由AsyncLogger传入
        std::thread _consumer; // 消费者线程

    public:
        AsyncLoggerCtrl(sink_func_t sink)
            : _stoped(false), _sink(sink), _consumer(std::thread(&AsyncLoggerCtrl::threadRunning, this))
        {
            std::cout << "异步日志器控制器创建成功.\n";
        }
        ~AsyncLoggerCtrl()
        {
            _stoped = true;
            _consumeCond.notify_all();
            _consumer.join();
        }
        // 生产者生产数据
        void push(const std::string &logMsg)
        {
            if(_stoped) 
                return;
            // 1.生产时，加锁，且条件为：生产缓冲区有空间可写
            {
                std::unique_lock<std::mutex> lock(_mtx);
                _produceCond.wait(lock, [&]{
                    return _produceBuf.writableSize() >= logMsg.size();
                });
                _produceBuf.push(logMsg.c_str(), logMsg.size());
            }
            // 2.唤醒消费者线程
            _consumeCond.notify_all();
        }

    private:
        // 消费者线程工作函数
        void threadRunning()
        {
            std::cout << "异步日志工作线程创建成功.\n";
            while(true)
            {
                sleep(1);
                if(_stoped && _produceBuf.empty())
                    return;
                // 1.消费时，加锁，且条件为：生产缓冲区有数据
                {
                    std::unique_lock<std::mutex> lock(_mtx);
                    _consumeCond.wait(lock, [&]{
                        return !_produceBuf.empty() || _stoped;
                    });
                    // 交换缓冲区
                    _produceBuf.swap(_consumeBuf);
                }
                // 2.唤醒生产者线程
                _produceCond.notify_all();
                // 3.消费数据
                _sink(_consumeBuf);
                // 4.消费缓冲区清空
                _consumeBuf.clear();
            }
        }
    };
}

#endif