#pragma once
#include <vector>
#include <memory>
#include <functional>

class TcpConnection;
class ThreadPool;

class EventLoop
{
public:
    EventLoop();
    ~EventLoop();

    void addConnection(std::shared_ptr<TcpConnection> conn);
    void removeConnection(int fd);
    void run();

private:
    std::vector<std::shared_ptr<TcpConnection>> conns_;
    ThreadPool *threadPool_;
};