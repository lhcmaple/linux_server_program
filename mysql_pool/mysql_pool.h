#ifndef _MYSQL_POOL_H
#define _MYSQL_POOL_H

#include <list>
#include <string>
#include <assert.h>
#include <mysql/mysql.h>
#include <pthread.h>

using std::list;
using std::string;

#define N_CONNECTIONS 100 //初始MYSQL连接池连接数

class mysql_pool{
private:
    list<MYSQL *> connect_pool;
    pthread_mutex_t mutex;
private:
    mysql_pool();
public:
    void init(const string _host,const string _user,
        const string &_passwd,const string &_db_name,int _port);
    MYSQL *get_connection();
    void return_connection(MYSQL *con);
    ~mysql_pool();
    static mysql_pool *get_instance();
};

#endif
