#ifndef CHATSERVICE_H
#define CHATSERVICE_H

#include "public.hpp"

class UserModel;
class OfflineMsgModel;
class FriendModel;
class GroupModel;

// Abstract function to all message handlers
using MsgHandler = std::function<void(const TcpConnectionPtr &,
                                      json &,
                                      Timestamp)>;

class ChatService
{
public:
    static ChatService *getInstance();

    // User login to the server
    void login(const TcpConnectionPtr &,
               json &,
               Timestamp);

    // User register as a new client
    void user_resister(const TcpConnectionPtr &,
                       json &,
                       Timestamp);

    void p2pchat(const TcpConnectionPtr &,
                 json &,
                 Timestamp);

    void add_friend(const TcpConnectionPtr &,
                    json &,
                    Timestamp);

    void creat_group(const TcpConnectionPtr &,
                     json &,
                     Timestamp);

    void add_group(const TcpConnectionPtr &,
                   json &,
                   Timestamp);

    void group_chat(const TcpConnectionPtr &,
                    json &,
                    Timestamp);

    void clientCloseException(const TcpConnectionPtr);

    MsgHandler getHandler(int msgid);

    // Server quit, set all users' status into 'offline'
    void reset();

private:
    ChatService();

    // Strategy pattern
    std::unordered_map<int, MsgHandler> _msgId2Handler;

    std::unique_ptr<UserModel> _userModel;
    std::unique_ptr<OfflineMsgModel> _offlinemessagemodel;
    std::unique_ptr<FriendModel> _friendmodel;
    std::unique_ptr<GroupModel> _groupModel;

    std::mutex _idMutex;
    std::unordered_map<int, TcpConnectionPtr> _id2TcpPtr;
};

#endif