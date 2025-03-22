#ifndef CHARSERVER_H
#define CHATSERVER_H

#include "public.hpp"

class ChatServer
{
public:
    ChatServer(const InetAddress&,
               EventLoop *loop,
               const string &nameArg);

    /*
        @brief Start the server
    */
    void start();

private:
    // Callback when connected
    void onConnection(const TcpConnectionPtr &);

    // Callback when messaged
    void onMessage(const TcpConnectionPtr &,
                   Buffer *,
                   Timestamp);
    TcpServer _server;
    EventLoop *_loop;
};

#endif