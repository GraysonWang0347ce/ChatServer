#ifndef FRIENGMODEL_H
#define FRIENDMODEL_H

#include "public.hpp"
#include "sqlconn.hpp"

/*
    Maintaining friend's info
*/
class FriendModel
{
public:
    // Insert 1 to 1 friend's relationship
    void insert(int from, int to);

    // Returns self's friend's id, name and status
    std::vector<USER> get_friend(int id);
private:
};

#endif