#include "groupmodel.hpp"

bool GroupModel::create_group(Group &group)
{
    char query[1024] = {0};

    sprintf(query,
            "INSERT INTO allgroup(groupname, groupdesc) VALUES('%s', '%s');",
            group.get_name().c_str(), group.get_desc().c_str());

    SQL sql;
    if (sql.connect())
    {
        if (sql.update(query))
        {
            group.set_id(mysql_insert_id(sql.get_connection()));
            return true;
        }
    }
    return false;
}

void GroupModel::add_group(int userid, int groupid, std::string role)
{
    char query[1024] = {0};

    sprintf(query,
            "INSERT INTO groupuser VALUES(%d, %d, '%s');",
            groupid, userid, role.c_str());

    SQL sql;
    if (sql.connect())
    {
        sql.update(query);
    }
}

std::vector<Group> GroupModel::query_groups(int userid)
{
    char query[1024] = {0};
    sprintf(query,
            "SELECT a.id, a.groupname,a.groupdesc FROM allgroup a INNER JOIN\
            groupuser b ON a.id=b.groupid WHERE b.userid=%d;",
            userid);

    std::vector<Group> groups;

    SQL sql;
    if (sql.connect())
    {
        MYSQL_RES *res;
        if ((res = sql.query(query)) != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                Group group;
                group.set_id(atoi(row[0]));
                group.set_name(row[1]);
                group.set_desc(row[2]);

                groups.push_back(group);
            }
        }
    }
    return groups;
}

std::vector<int> GroupModel::query_group_users(int userid, int groupid)
{
    char query[1024] = {0};
    sprintf(query,
            "SELECT userid FROM groupuser WHERE groupid=%d AND userid != %d;",
            groupid, userid);

    std::vector<int> res_vec;
    SQL sql;
    if (sql.connect())
    {
        MYSQL_RES *res;
        if ((res = sql.query(query)) != nullptr)
        {
            MYSQL_ROW row;
            while ((row = mysql_fetch_row(res)) != nullptr)
            {
                res_vec.push_back(atoi(row[0]));
            }
        }
    }
    return res_vec;
}