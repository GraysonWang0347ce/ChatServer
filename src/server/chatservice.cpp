#include "chatservice.hpp"
#include "usermodel.hpp"
#include "offlinemessagemodel.hpp"
#include "friendmodel.hpp"
#include "groupmodel.hpp"

ChatService *ChatService::getInstance()
{
    static ChatService service;
    return &service;
}

ChatService::ChatService()
{
    _userModel = std::make_unique<UserModel>();
    _offlinemessagemodel = std::make_unique<OfflineMsgModel>();
    _friendmodel = std::make_unique<FriendModel>();

    // Register all handlers to the map
    _msgId2Handler.insert({MSG_LOGIN, std::bind(&ChatService::login, this,
                                                std::placeholders::_1,
                                                std::placeholders::_2,
                                                std::placeholders::_3)});

    _msgId2Handler.insert({MSG_REGISTER, std::bind(&ChatService::user_resister, this,
                                                   std::placeholders::_1,
                                                   std::placeholders::_2,
                                                   std::placeholders::_3)});

    _msgId2Handler.insert({MSG_ONE_CHAT, std::bind(&ChatService::p2pchat, this,
                                                   std::placeholders::_1,
                                                   std::placeholders::_2,
                                                   std::placeholders::_3)});

    _msgId2Handler.insert({MSG_ADD_FRIEND, std::bind(&ChatService::add_friend, this,
                                                     std::placeholders::_1,
                                                     std::placeholders::_2,
                                                     std::placeholders::_3)});

    _msgId2Handler.insert({MSG_ADD_GROUP, std::bind(&ChatService::add_group, this,
                                                    std::placeholders::_1,
                                                    std::placeholders::_2,
                                                    std::placeholders::_3)});

    _msgId2Handler.insert({MSG_CREAT_GROUP, std::bind(&ChatService::creat_group, this,
                                                      std::placeholders::_1,
                                                      std::placeholders::_2,
                                                      std::placeholders::_3)});

    _msgId2Handler.insert({MSG_GROUP_CHAT, std::bind(&ChatService::group_chat, this,
                                                     std::placeholders::_1,
                                                     std::placeholders::_2,
                                                     std::placeholders::_3)});
}

void ChatService::login(const TcpConnectionPtr &tcpconn,
                        json &js,
                        Timestamp ts)
{
    int id = js["id"].get<int>();
    std::string password = js["password"];

    USER user = _userModel->query(id);
    if (user.get_id() == id && user.get_password() == password)
    {
        if (user.get_state() == "online")
        {
            // User already online
            json response;
            response["msgId"] = MSG_LOGIN_ACK;
            response["errNo"] = ALREADY_ONLINE;
            response["errMsg"] = "Failed to login the account already online.";

            tcpconn->send(response.dump());
        }
        else
        {
            // Really OK
            // Update online state into database
            user.set_state("online");
            _userModel->update(user);

            json response;
            response["msgId"] = MSG_LOGIN_ACK;
            response["errNo"] = SUCCESS;
            response["id"] = id;
            response["name"] = user.get_name();

            {
                std::lock_guard<std::mutex> lock(_idMutex);
                _id2TcpPtr.insert({id, tcpconn});
            }

            // Get offline messages
            std::vector<std::string> res_vec = _offlinemessagemodel->query(id);
            if (!res_vec.empty())
            {
                response["offlinemsg"] = res_vec;
            }

            _offlinemessagemodel->remove(id);

            // Get friend list
            std::vector<USER> friend_list = _friendmodel->get_friend(id);
            if (!friend_list.empty())
            {
                std::vector<std::string> row;
                for (auto &usr : friend_list)
                {
                    json user;
                    user["id"] = usr.get_id();
                    user["name"] = usr.get_name();
                    user["status"] = usr.get_state();

                    row.push_back(user.dump());
                }

                response["friends"] = row;
            }

            tcpconn->send(response.dump());
        }
    }
    else
    {
        // Password Error.
        json response;
        response["msgId"] = MSG_LOGIN_ACK;
        response["errNo"] = LOGIN_FAILED;
        response["errMsg"] = "Login failed! Please try again.";
    }
}

void ChatService::user_resister(const TcpConnectionPtr &tcpconn,
                                json &js,
                                Timestamp ts)
{
    std::string name = js["name"];
    std::string password = js["password"];

    USER user(0, name, password);
    bool ret = _userModel->insert(user);
    if (ret)
    {
        // success
        json tmpUsr;
        tmpUsr["messageId"] = MSG_REGISTER_ACK;
        tmpUsr["errNo"] = SUCCESS;
        tmpUsr["id"] = user.get_id();
        tcpconn->send(tmpUsr.dump());
    }
    else
    {
        // fail
        json response;
        response["msgId"] = MSG_REGISTER_ACK;
        response["errNo"] = REGISTER_FAILED;
        response["errMsg"] = "Registeration failed! Please try again.";
        tcpconn->send(response.dump());
    }
}

MsgHandler ChatService::getHandler(int msgid)
{
    auto it = _msgId2Handler.find(msgid);
    if (it == _msgId2Handler.end())
    {
        // msgid not exist
        return [=](auto a, auto b, auto c) -> void
        {
            LOG_ERROR << "Msgid invalid, got" << msgid;
        };
    }
    return it->second;
}

void ChatService::clientCloseException(const TcpConnectionPtr conn)
{
    int id = -1;
    {
        std::lock_guard<std::mutex> lock(_idMutex);
        for (auto it : _id2TcpPtr)
        {
            if (it.second == conn)
            {
                id = it.first;
                _id2TcpPtr.erase(id);
                break;
            }
        }
    }

    if (id == -1)
        return;

    USER user;
    user.set_id(id);
    user.set_state("offline");

    _userModel->update(user);
}

void ChatService::p2pchat(const TcpConnectionPtr &conn,
                          json &js,
                          Timestamp ts)
{
    int peer_id = js["to"].get<int>();
    int self_id = js["from"].get<int>();

    std::string message = js["message"];
    {
        std::lock_guard<std::mutex> lock(_idMutex);
        auto it = _id2TcpPtr.find(peer_id);
        if (it == _id2TcpPtr.end())
        {
            // Peer offline
            _offlinemessagemodel->insert(peer_id, message);
        }
        else
        {
            json msg;
            msg["message"] = message;
            msg["from"] = self_id;
            msg["name"] = js["name"];
            msg["time"] = ts.toString();

            conn->send(msg.dump());
        }
    }
}

void ChatService::creat_group(const TcpConnectionPtr &conn,
                              json &js,
                              Timestamp ts)
{
    int userid = js["id"].get<int>();
    std::string name = js["groupname"];
    std::string desc = js["groupdesc"];

    Group group(-1, name, desc);
    if (_groupModel->create_group(group))
    {
        _groupModel->add_group(userid, group.get_id(), "creator");
    }
}

void ChatService::add_group(const TcpConnectionPtr &conn,
                            json &js,
                            Timestamp ts)
{
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();

    _groupModel->add_group(userid, groupid, "normal");
}

void ChatService::group_chat(const TcpConnectionPtr &conn,
                             json &js,
                             Timestamp ts)
{
    int userid = js["id"].get<int>();
    int groupid = js["groupid"].get<int>();

    std::vector<int> userids = _groupModel->query_group_users(userid, groupid);
    for (int id : userids)
    {
        std::lock_guard<std::mutex> lock(_idMutex);
        auto it = _id2TcpPtr.find(id);
        if (it != _id2TcpPtr.end())
        {
            // Forward online messages
            it->second->send(js.dump());
        }
        else
        {
            // Forward offline messages
            _offlinemessagemodel->insert(id,js.dump());
        }
    }
}

void ChatService::reset()
{
    _userModel->reset_all();
}

void ChatService::add_friend(const TcpConnectionPtr &conn,
                             json &js,
                             Timestamp ts)
{
    int peer_id = js["to"].get<int>();
    int self_id = js["from"].get<int>();

    _friendmodel->insert(self_id, peer_id);
}
