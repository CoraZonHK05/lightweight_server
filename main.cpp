#include "acceptor.h"
#include "event_loop.h"
#include "tcp_connection.h"
#include <iostream>
#include <thread>
#include <vector>

int main()
{
    EventLoop loop;

    // 保存所有连接，防止 shared_ptr 引用计数归零
    std::vector<std::shared_ptr<TcpConnection>> connections;

    Acceptor acceptor(8080);
    acceptor.setNewConnectionCallback([&](std::shared_ptr<TcpConnection> conn)
                                      {
        std::cout << "新连接 fd=" << conn->fd() << "\n";

        conn->setReadCallback([conn](const std::string& msg) {
            std::cout << "收到: " << msg;
            conn->send("HTTP/1.1 200 OK\r\nContent-Length: 5\r\n\r\nHello");
        });

        connections.push_back(conn);
        loop.addConnection(conn); });

    // 另一个线程跑 accept
    std::thread t([&]()
                  { acceptor.listen(); });

    loop.run();
    return 0;
}