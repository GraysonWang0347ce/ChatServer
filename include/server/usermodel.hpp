#ifndef USERMODEL_H
#define USERMODEL_H

#include "sqlconn.hpp"

class UserModel
{
public:
    UserModel() = default;
    bool insert(USER &user);
    USER query(int id);
    bool update(USER );

    void reset_all();

private:
};

#endif