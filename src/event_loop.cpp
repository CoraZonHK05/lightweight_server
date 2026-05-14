#include "event_loop.h"
#include "tcp_connection.h"
#include "thread_pool.h"
#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
static bool initWinsock()
{
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}
static bool winsockReady = initWinsock();
#else
#include <sys/select.h>
#include <unistd.h> // Linux 下提供 usleep
#endif

EventLoop::EventLoop() : threadPool_(new ThreadPool(4)) {}
EventLoop::~EventLoop() { delete threadPool_; }

void EventLoop::addConnection(std::shared_ptr<TcpConnection> conn)
{
    conns_.push_back(conn);
}

void EventLoop::removeConnection(int fd)
{
    for (auto it = conns_.begin(); it != conns_.end(); ++it)
    {
        if ((*it)->fd() == fd)
        {
            conns_.erase(it);
            return;
        }
    }
}

void EventLoop::run()
{
    std::cout << "[EventLoop] 启动\n";
    while (true)
    {
        fd_set readfds, writefds;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        int maxfd = 0;

        for (auto &conn : conns_)
        {
            if (conn->fd() >= 0)
            {
                FD_SET(conn->fd(), &readfds);
                if (conn->writeBuffer().readableSize() > 0)
                {
                    FD_SET(conn->fd(), &writefds);
                }
                if (conn->fd() > maxfd)
                    maxfd = conn->fd();
            }
        }

        if (maxfd == 0)
        {
#ifdef _WIN32
            _sleep(100);
#else
            usleep(100000); // 100ms = 100000us
#endif
            continue;
        }

        struct timeval tv = {0, 100000};
        int ret = select(maxfd + 1, &readfds, &writefds, nullptr, &tv);

        if (ret > 0)
        {
            for (auto &conn : conns_)
            {
                if (conn->fd() < 0)
                    continue;
                if (FD_ISSET(conn->fd(), &readfds))
                {
                    auto c = conn;
                    threadPool_->submit([c]()
                                        { c->handleRead(); });
                }
                if (FD_ISSET(conn->fd(), &writefds))
                {
                    auto c = conn;
                    threadPool_->submit([c]()
                                        { c->handleWrite(); });
                }
            }
        }
    }
}
