#include "friendmodel.hpp"

void FriendModel::insert(int from, int to)
{
    char query[1024] = {0};
    sprintf(query,
            "INSERT INTO Friend VALUES(%d,%d);", from, to);

    SQL sql;
    if (sql.connect())
    {
        sql.update(query);
    }
}

std::vector<USER> FriendModel::get_friend(int id)
{
    std::vector<USER> res;

    char query[1024] = {0};
    sprintf(query,
            "SELECT id,name status FROM (Friend f INNER JOIN User u ON f.userid = u.id) WHERE f.friendid = %d;", id);

    SQL sql;
    if (sql.connect())
    {
        USER usr;
        MYSQL_RES *resp = sql.query(query);
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(resp)) != nullptr)
        {
            usr.set_id(atoi(row[1]));
            usr.set_name(row[1]);
            usr.set_state(row[2]);
            res.push_back(usr);
        }
        mysql_free_result(resp);
    }
    return res;
}