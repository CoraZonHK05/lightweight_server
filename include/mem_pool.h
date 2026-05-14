#pragma once
#include <cstddef>

class FixedMemPool
{
public:
    FixedMemPool(size_t block_size = 256, size_t block_count = 1024);
    ~FixedMemPool();

    void *allocate();
    void deallocate(void *ptr);

    size_t block_size() const;
    size_t free_count() const;
    bool empty() const;

private:
    union MemNode
    {
        MemNode *next;
        char data[1];
    };

    MemNode *pool_;
    MemNode *free_list_;
    size_t block_size_;
    size_t block_count_;
    size_t free_count_;
};