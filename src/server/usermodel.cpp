#include "usermodel.hpp"

bool UserModel::insert(USER &user)
{
    char query[1024] = {0};

    sprintf(query,
            "INSERT INTO User(name, password, state) values('%s' ,'%s','%s');",
            user.get_name().c_str(),
            user.get_password().c_str(),
            user.get_state().c_str());

    SQL sql;
    if (sql.connect())
    {
        if (sql.update(query))
        {
            user.set_id(mysql_insert_id(sql.get_connection()));
            return true;
        }
    }

    return false;
}

/*
    @brief Get full user detail with primary key id.
    @param id Uinque identifier of each user.
    @return USER Object on success, USER()default on failure.
*/
USER UserModel::query(int id)
{
    char query[1024];
    sprintf(query,
            "SELECT * FROM User WHERE id=%d;", id);

    SQL sql;
    if (sql.connect())
    {
        MYSQL_RES *res = sql.query(query);
        if (res != nullptr)
        {
            MYSQL_ROW res_row = mysql_fetch_row(res);
            USER user;
            user.set_id(atoi(res_row[0]));
            user.set_name(res_row[1]);
            user.set_password(res_row[2]);
            user.set_state(res_row[3]);

            mysql_free_result(res);
            return user;
        }
        else
            return;
    }
}

/*
    @brief Update user's state base on user's id.
    @param user Must have id and state
*/
bool UserModel::update(USER user)
{
    char query[1024] = {0};

    sprintf(query,
            "UPDATE User SET state=%s WHERE id=%d;",
            user.get_state().c_str(),
            user.get_id());

    SQL sql;
    if (sql.connect())
    {
        if (sql.update(query))
        {
            return true;
        }
    }
    return false;
}

void UserModel::reset_all()
{
    char query[1024] = "UPDATE User SET state='offline' WHERE state='online';";
    SQL sql;
    if (sql.connect())
        sql.update(query);
}