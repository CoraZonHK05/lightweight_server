#include "mem_pool.h"

FixedMemPool::FixedMemPool(size_t block_size, size_t block_count)
    : block_size_(block_size > sizeof(MemNode) ? block_size : sizeof(MemNode)),
      block_count_(block_count), free_count_(block_count)
{

    pool_ = static_cast<MemNode *>(::operator new(block_size_ * block_count_));

    free_list_ = pool_;
    for (size_t i = 0; i < block_count_ - 1; ++i)
    {
        auto *node = reinterpret_cast<MemNode *>(
            reinterpret_cast<char *>(pool_) + i * block_size_);
        auto *next = reinterpret_cast<MemNode *>(
            reinterpret_cast<char *>(pool_) + (i + 1) * block_size_);
        node->next = next;
    }
    auto *last = reinterpret_cast<MemNode *>(
        reinterpret_cast<char *>(pool_) + (block_count_ - 1) * block_size_);
    last->next = nullptr;
}

FixedMemPool::~FixedMemPool()
{
    ::operator delete(pool_);
}

void *FixedMemPool::allocate()
{
    if (!free_list_)
        return nullptr;
    MemNode *head = free_list_;
    free_list_ = head->next;
    --free_count_;
    return head;
}

void FixedMemPool::deallocate(void *ptr)
{
    if (!ptr)
        return;
    MemNode *node = static_cast<MemNode *>(ptr);
    node->next = free_list_;
    free_list_ = node;
    ++free_count_;
}

size_t FixedMemPool::block_size() const { return block_size_; }
size_t FixedMemPool::free_count() const { return free_count_; }
bool FixedMemPool::empty() const { return free_list_ == nullptr; }