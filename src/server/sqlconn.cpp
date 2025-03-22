#include "sqlconn.hpp"

SQL::SQL()
{
    _conn = mysql_init(nullptr);

    std::ifstream sql_config("../include/sqlconfig.json", std::ios::binary);
    sql_config >> _sql_config;

    host = _sql_config["ip"];
    user = _sql_config["user"];
    password = _sql_config["password"];
    dbname = _sql_config["dbname"];
}

SQL::~SQL()
{
    if (_conn)
    {
        mysql_close(_conn);
    }
}

bool SQL::connect()
{
    MYSQL *p = mysql_real_connect(_conn, host.c_str(),
                                  user.c_str(),
                                  password.c_str(),
                                  dbname.c_str(),
                                  3306, nullptr, 0);
    if (p == nullptr)
    {
        LOG_WARN << "Failed to connect to mysql server at" << host.c_str() << ":3306!";
        return false;
    }

    mysql_query(_conn, "set names gbk");
    LOG_INFO << "Success connecting to " << host.c_str() << ":3306!";
    return true;
}

bool SQL::update(std::string query)
{
    if (mysql_query(_conn, query.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ": " << query << " update failed!";
        return false;
    }

    return true;
}

MYSQL_RES *SQL::query(std::string query)
{
    if (mysql_query(_conn, query.c_str()))
    {
        LOG_INFO << __FILE__ << ":" << __LINE__ << ": " << query << " query failed!";
        return nullptr;
    }

    return mysql_use_result(_conn);
}

MYSQL *SQL::get_connection()
{
    return _conn;
}