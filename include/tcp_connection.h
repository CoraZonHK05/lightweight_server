#pragma once
#include "buffer.h"
#include <functional>
#include <string>

class TcpConnection
{
public:
    using Callback = std::function<void(const std::string &)>;

    TcpConnection(int fd);
    ~TcpConnection();

    int fd() const;

    // 收到数据时，读 Buffer 里攒了完整请求后，调回调
    void setReadCallback(Callback cb);

    // 被 EventLoop 调用：从 socket 读数据到读 Buffer
    void handleRead();
    // 被 EventLoop 调用：把写 Buffer 里的数据发出去
    void handleWrite();

    // 发数据：先把要发的数据放到写 Buffer
    void send(const std::string &msg);

    Buffer &readBuffer() { return readBuf_; }
    Buffer &writeBuffer() { return writeBuf_; }

private:
    int fd_;
    Buffer readBuf_;
    Buffer writeBuf_;
    Callback readCallback_;
};