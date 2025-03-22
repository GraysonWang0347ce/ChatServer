#ifndef PUBLIC_H
#define PUBLIC_H

#include <muduo/net/TcpServer.h>
#include <muduo/net/EventLoop.h>
#include <string>
#include <functional>
#include <thread>
#include <json.hpp>
#include <unordered_map>
#include <muduo/base/Logging.h>
#include <iostream>
#include <mysql/mysql.h>
#include <fstream>
#include <mutex>
#include <vector>

using namespace muduo;
using namespace muduo::net;
using json = nlohmann::json;

enum MsgType
{
    MSG_LOGIN,
    MSG_LOGIN_ACK, // Acknowledge of the login message
    MSG_REGISTER,
    MSG_REGISTER_ACK, // Acknowledge of the register message
    MSG_ONE_CHAT,     // P2P chat
    MSG_GROUP_CHAT,
    MSG_CREAT_GROUP,
    MSG_ADD_GROUP,
    MSG_ADD_FRIEND,
    MSG_GROUP_CHAT,
};

enum ErrNo
{
    SUCCESS,
    REGISTER_FAILED,
    LOGIN_FAILED,
    ALREADY_ONLINE,
};

#endif