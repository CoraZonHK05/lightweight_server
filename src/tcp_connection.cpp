#include "tcp_connection.h"

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define close closesocket
#undef send
#undef recv
#endif

#include <iostream>

TcpConnection::TcpConnection(int fd)
    : fd_(fd), readBuf_(1024), writeBuf_(1024) {}

TcpConnection::~TcpConnection()
{
    if (fd_ >= 0)
    {
        close(fd_);
    }
}

int TcpConnection::fd() const { return fd_; }

void TcpConnection::setReadCallback(Callback cb)
{
    readCallback_ = std::move(cb);
}

void TcpConnection::handleRead()
{
    char buf[1024];
    int n = ::recv(fd_, buf, sizeof(buf), 0);
    if (n > 0)
    {
        readBuf_.append(buf, n);
        const char *crlf = readBuf_.findCRLF();
        if (crlf && readCallback_)
        {
            std::string line = readBuf_.retrieve(crlf - readBuf_.peek() + 2);
            readCallback_(line);
        }
    }
    else if (n == 0)
    {
        std::cout << "[TcpConnection] fd=" << fd_ << " closed by peer\n";
        close(fd_);
        fd_ = -1;
    }
    else
    {
        std::cerr << "[TcpConnection] recv error on fd=" << fd_ << "\n";
        close(fd_);
        fd_ = -1;
    }
}

void TcpConnection::handleWrite()
{
    if (writeBuf_.readableSize() == 0)
        return;
    int n = ::send(fd_, writeBuf_.peek(), writeBuf_.readableSize(), 0);
    if (n > 0)
    {
        writeBuf_.retrieve(n);
    }
    else if (n == 0)
    {
        close(fd_);
        fd_ = -1;
    }
    else
    {
        std::cerr << "[TcpConnection] send error on fd=" << fd_ << "\n";
        close(fd_);
        fd_ = -1;
    }
}

void TcpConnection::send(const std::string &msg)
{
    writeBuf_.append(msg);
}