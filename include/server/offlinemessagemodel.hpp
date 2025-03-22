#ifndef OFFLINEMESSAGEMODEL_H
#define OFFLINEMESSAGEMODEL_H

#include "sqlconn.hpp"

class OfflineMsgModel
{
public:
    OfflineMsgModel() = default;
    void insert(int, std::string);

    void remove(int);

    std::vector<std::string> query(int);

private:
};

#endif