#ifndef _ASYNCLOGGERCHARBUF_HPP_
#define _ASYNCLOGGERCHARBUF_HPP_

#include <vector>
#include <iostream>
#include <cassert>

namespace mylog
{
    // const long long DEFAULT_BUFFER_LEN = 1024 * 1024 * 10; // 默认缓冲区空间大小
    const long long DEFAULT_BUFFER_LEN = 1024 * 1024 * 1; // 默认缓冲区空间大小 1Mb
    class AsyncLoggerCharBuf
    {
    private:
        size_t _wptr;             // 读指针
        size_t _rptr;             // 写指针
        std::vector<char> _space; // 缓冲区空间
    public:
        AsyncLoggerCharBuf()
            : _wptr(0), _rptr(0), _space(DEFAULT_BUFFER_LEN)
        {
        }
        // 向缓冲区里写数据
        void push(const char *data, size_t len)
        {
            // len: 写进来的数据的长度
            // writableSize: 还能写的长度
            assert(len <= writableSize());
            std::copy(data, data + len, &_space[_wptr]);
            _wptr += len;
        }
        // 获取读指针
        const char *readBegin()
        {
            return &_space[_rptr];
        }
        // 获取当前可读大小
        size_t readEnd()
        {
            return _wptr - _rptr;
        }
        // 当前可写大小
        size_t writableSize()
        {
            return _space.size() - _wptr;
        }
        // 清空缓冲区
        void clear()
        {
            _rptr = 0;
            _wptr = 0;
        }
        // 交换两个缓冲区：this & cbuf
        void swap(AsyncLoggerCharBuf &cbuf)
        {
            _space.swap(cbuf._space);
            std::swap(_wptr, cbuf._wptr);
            std::swap(_rptr, cbuf._rptr);
        }
        // 缓冲区里是否没有数据
        bool empty()
        {
            return _rptr == _wptr;
        }
    };
}

#endif