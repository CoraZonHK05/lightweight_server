#pragma once
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <vector>

class ThreadPool
{
public:
    // 构造函数：提前创建 threads 个线程
    ThreadPool(size_t threads = 4);
    ~ThreadPool();

    // 提交任务：往队列里丢一个任务，线程会自动取走执行
    void submit(std::function<void()> task);

private:
    std::vector<std::thread> workers_;        // 线程数组
    std::queue<std::function<void()>> tasks_; // 任务队列
    std::mutex queue_mutex_;                  // 保护任务队列的锁
    std::condition_variable condition_;       // 用于通知线程"有任务来了"
    bool stop_;                               // 标记线程池是否要关闭
};