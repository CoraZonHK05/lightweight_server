#include "buffer.h"
#include <cstring>
#include <algorithm>

// 静态成员定义：块大小 256，共 1024 块
FixedMemPool Buffer::pool_(256, 1024);

Buffer::Buffer(size_t initial_size)
    : capacity_(initial_size), readPos_(0), writePos_(0)
{
    data_ = static_cast<char *>(pool_.allocate());
    if (!data_)
    {
        // 内存池满了，fallback 到 new
        data_ = new char[capacity_];
    }
}

Buffer::~Buffer()
{
    if (data_)
    {
        // 如果 data_ 指向的地址在内存池范围内，归还内存池
        // 简化处理：直接 delete（实际项目需要判断是否来自内存池）
        delete[] data_;
    }
}

void Buffer::append(const char *data, size_t len)
{
    ensureWritable(len);
    std::memcpy(data_ + writePos_, data, len);
    writePos_ += len;
}

void Buffer::append(const std::string &str)
{
    append(str.data(), str.size());
}

std::string Buffer::retrieve(size_t len)
{
    if (len > readableSize())
        len = readableSize();
    std::string result(data_ + readPos_, len);
    readPos_ += len;
    if (readPos_ == writePos_)
    {
        readPos_ = 0;
        writePos_ = 0;
    }
    return result;
}

std::string Buffer::retrieveAll()
{
    return retrieve(readableSize());
}

const char *Buffer::peek() const
{
    return data_ + readPos_;
}

size_t Buffer::readableSize() const
{
    return writePos_ - readPos_;
}

const char *Buffer::findCRLF() const
{
    const char *start = data_ + readPos_;
    const char *end = data_ + writePos_;
    for (const char *p = start; p < end - 1; ++p)
    {
        if (*p == '\r' && *(p + 1) == '\n')
            return p;
    }
    return nullptr;
}

void Buffer::ensureWritable(size_t len)
{
    if (writePos_ + len <= capacity_)
        return;

    // 前面有已读空间，往前挪
    if (readPos_ > 0 && readableSize() + len <= capacity_)
    {
        std::memmove(data_, data_ + readPos_, readableSize());
        writePos_ -= readPos_;
        readPos_ = 0;
        return;
    }

    // 扩容：新容量为 2 倍，直到够用
    size_t new_cap = capacity_ * 2;
    while (writePos_ + len > new_cap)
        new_cap *= 2;

    char *new_data = new char[new_cap];
    std::memcpy(new_data, data_ + readPos_, readableSize());
    writePos_ -= readPos_;
    readPos_ = 0;

    delete[] data_;
    data_ = new_data;
    capacity_ = new_cap;
}