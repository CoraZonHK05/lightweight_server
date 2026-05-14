#pragma once
#include "mem_pool.h"
#include <string>
#include <cstring>

class Buffer
{
public:
    Buffer(size_t initial_size = 1024);
    ~Buffer();

    void append(const char *data, size_t len);
    void append(const std::string &str);

    std::string retrieve(size_t len);
    std::string retrieveAll();

    const char *peek() const;
    size_t readableSize() const;

    const char *findCRLF() const;

private:
    void ensureWritable(size_t len);

    char *data_;               // 用内存池管理的字符数组
    size_t capacity_;          // 总容量
    size_t readPos_;           // 读指针
    size_t writePos_;          // 写指针
    static FixedMemPool pool_; // 所有 Buffer 共用一个内存池
};