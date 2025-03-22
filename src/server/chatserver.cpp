#include "chatserver.hpp"
#include "chatservice.hpp"

using namespace std;
using json = nlohmann::json;

ChatServer::ChatServer(const InetAddress &listenaddr,
                       EventLoop *loop,
                       const string &nameArg)
    : _server(loop, listenaddr, nameArg), _loop(loop)
{
    _server.setConnectionCallback(std::bind(&ChatServer::onConnection,
                                            this, std::placeholders::_1));

    _server.setMessageCallback(std::bind(&ChatServer::onMessage,
                                         this, placeholders::_1,
                                         placeholders::_2,
                                         placeholders::_3));

    _server.setThreadNum(thread::hardware_concurrency());
}

void ChatServer::start()
{
    _server.start();
}

void ChatServer::onConnection(const TcpConnectionPtr &ptr)
{
    // TODO Notify service layer

    // User Logged out
    if (!ptr->connected())
    {
        ChatService::getInstance()->clientCloseException(ptr);
        ptr->shutdown();
    }
}

void ChatServer::onMessage(const TcpConnectionPtr &ptr,
                           Buffer *buf,
                           Timestamp ts)
{
    string buffer = buf->retrieveAllAsString();
    json js = json::parse(buffer);

    int msgId = js["messageId"].get<int>();
    ChatService *pService = ChatService::getInstance();
    auto handler = pService->getHandler(msgId);

    // Service code down here
    handler(ptr, js, ts);
}
