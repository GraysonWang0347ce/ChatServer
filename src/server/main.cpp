#include "chatserver.hpp"
#include "chatservice.hpp"
#include <iostream>
#include <signal.h>

void serverexithandler(int)
{
    ChatService::getInstance()->reset();
    exit(0);
}

int main()
{
    signal(SIGINT, serverexithandler);

    EventLoop loop;
    InetAddress addr("127.0.0.1", 34560);

    ChatServer server(addr, &loop, "server");

    server.start();
    loop.loop();

    return 0;
}