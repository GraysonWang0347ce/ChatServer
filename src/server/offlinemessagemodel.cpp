#include "offlinemessagemodel.hpp"

void OfflineMsgModel::insert(int id, std::string msg)
{
    char query[1024] = {0};
    sprintf(query,
            "INSERT INTO OfflineMessage(userid, message) values(%d, %s);",
            id, msg.c_str());

    SQL sql;
    if (sql.connect())
    {
        sql.update(query);
    }
}

void OfflineMsgModel::remove(int id)
{
    char query[1024] = {0};
    sprintf(query,
            "DELETE FROM OfflineMessage WHERE userid=%d;",
            id);

    SQL sql;
    if (sql.connect())
    {
        sql.update(query);
    }
}

/*
    @brief Return all offline messages of id's.
    @return Vector of messages on success, empty vector on failure. 
*/
std::vector<std::string> OfflineMsgModel::query(int id)
{
    char query[1024] = {0};
    sprintf(query,
            "SELECT message FROM OfflineMessage WHERE userid=%d;",
            id);

    SQL sql;
    std::vector<std::string> res_vec;
    if (sql.connect())
    {
        MYSQL_RES *res = sql.query(query);
        if (res != nullptr)
        {
            MYSQL_ROW row;
            while((row = mysql_fetch_row(res)) != nullptr)
            {
                res_vec.push_back(row[0]);
            }
            mysql_free_result(res);
        }
    }
    return res_vec;
}