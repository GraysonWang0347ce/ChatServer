#ifndef SQLCONN_H
#define SQLCONN_H

#include "public.hpp"

class SQL
{
public:
    SQL();
    ~SQL();

    bool connect();
    bool update(std::string query);
    MYSQL_RES *query(std::string qurry);

    MYSQL *get_connection();

private:
    MYSQL *_conn;
    json _sql_config;
    std::string host;
    std::string user;
    std::string password;
    std::string dbname;
};

// ORM of user
class USER
{
public:
    explicit USER(int id = 0,
                  std::string name = "",
                  std::string password = "",
                  std::string state = "offline") : _id(id),
                                                   _name(name),
                                                   _password(password),
                                                   _state(state) {};

    void set_id(int id) { _id = id; };
    void set_name(std::string name) { _name = name; };
    void set_password(std::string password) { _password = password; };
    void set_state(std::string state) { _state = state; };

    int get_id() const { return _id; };
    std::string get_name() const { return _name; };
    std::string get_password() const { return _password; };
    std::string get_state() const { return _state; };

protected:
    int _id;
    std::string _name;
    std::string _password;
    std::string _state;
};

#endif