#include "acceptor.h"
#include "tcp_connection.h"
#include <iostream>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define close closesocket

// Windows 需要初始化 Winsock
static bool initWinsock()
{
    WSADATA wsaData;
    return WSAStartup(MAKEWORD(2, 2), &wsaData) == 0;
}
static bool winsockReady = initWinsock();
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#endif

Acceptor::Acceptor(int port) : port_(port), listenFd_(-1) {}

Acceptor::~Acceptor()
{
    if (listenFd_ >= 0)
        close(listenFd_);
}

void Acceptor::setNewConnectionCallback(NewConnectionCallback cb)
{
    newConnectionCallback_ = std::move(cb);
}

void Acceptor::listen()
{
    listenFd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (listenFd_ < 0)
    {
        std::cerr << "[Acceptor] socket() failed\n";
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenFd_, (sockaddr *)&addr, sizeof(addr)) < 0)
    {
        std::cerr << "[Acceptor] bind() failed\n";
        return;
    }

    if (::listen(listenFd_, 10) < 0)
    {
        std::cerr << "[Acceptor] listen() failed\n";
        return;
    }

    std::cout << "[Acceptor] 监听端口 " << port_ << "\n";

    while (true)
    {
        int clientFd = accept(listenFd_, nullptr, nullptr);
        if (clientFd < 0)
            continue;
        std::cout << "[Acceptor] 新连接 fd=" << clientFd << "\n";
        if (newConnectionCallback_)
        {
            auto conn = std::make_shared<TcpConnection>(clientFd);
            newConnectionCallback_(conn);
        }
    }
}