#pragma once
#include <functional>
#include <memory>

class TcpConnection;

class Acceptor
{
public:
    using NewConnectionCallback = std::function<void(std::shared_ptr<TcpConnection>)>;

    Acceptor(int port);
    ~Acceptor();

    void setNewConnectionCallback(NewConnectionCallback cb);
    void listen();

private:
    int listenFd_;
    int port_;
    NewConnectionCallback newConnectionCallback_;
};