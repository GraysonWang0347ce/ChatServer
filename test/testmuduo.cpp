#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <iostream>
#include <functional>
#include <thread>

using namespace std;
using namespace muduo;
using namespace muduo::net;

class ChatServer
{
public:
    ChatServer(EventLoop *loop,
               const InetAddress &listenAddr,
               const string &nameArg) : _server(loop, listenAddr, nameArg),
                                        _loop(loop)
    {
        _server.setConnectionCallback(bind(&ChatServer::onConectionCb, this, placeholders::_1));

        _server.setMessageCallback(bind(&ChatServer::onMessageCb, this,
                                        placeholders::_1,
                                        placeholders::_2,
                                        placeholders::_3));

        _server.setThreadNum(thread::hardware_concurrency());
    };
    ~ChatServer() {};

    void start() { _server.start(); };

private:
    void onConectionCb(const TcpConnectionPtr &tcpPtr)
    {
        cout << tcpPtr->localAddress().toIpPort() << " -> " << tcpPtr->peerAddress().toIpPort() << endl;
    };
    void onMessageCb(const TcpConnectionPtr &tcpPtr,
                     Buffer *buf,
                     Timestamp time)
    {
        string buffer = buf->retrieveAllAsString();
        cout << "recv data: " << buffer << endl;
        tcpPtr->send(buffer);
    };
    TcpServer _server;
    EventLoop *_loop;
};

int main(int argc, char *argv[])
{
    EventLoop loop;
    InetAddress addr("127.0.0.1", 43210);
    ChatServer server(&loop, addr, "Server");
    server.start();
    loop.loop();

    return 0;
}