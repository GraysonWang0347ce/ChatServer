#ifndef GROUPMODEL_H
#define GROUPMODEL_H

#include "public.hpp"
#include "sqlconn.hpp"

class Group
{
public:
    Group(int id = -1, std::string name = "", std::string desc = "")
        : _id(id), _name(name), _desc(desc) {}

    void set_id(int id) { _id = id; }
    void set_name(std::string name) { _name = name; }
    void set_desc(std::string desc) { _desc = desc; }

    int get_id() const { return _id; }
    std::string get_name() const { return _name; }
    std::string get_desc() const { return _desc; }

    std::vector<GroupUser> &get_users() { return _users; }

private:
    int _id;
    std::string _name;
    std::string _desc;
    std::vector<GroupUser> _users;
};

class GroupUser : public USER
{
public:
    GroupUser(int id = 0,
              std::string name = "",
              std::string password = "",
              std::string state = "offline",
              std::string role)
        : USER(id, name, password, state), _role(role) {}

    void set_role(std::string role) { _role = role; }
    std::string get_role() const { return _role; }

private:
    std::string _role;
};

class GroupModel
{
public:
    bool create_group(Group &);

    void add_group(int, int, std::string);

    std::vector<Group> query_groups(int);

    std::vector<int> query_group_users(int, int);
};

#endif