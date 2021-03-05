/* 数据库连接池
 * 单例模式
 * 使用前要先init
 */

#include "mysql_pool.h"

mysql_pool::mysql_pool()
{
    assert(0==mysql_library_init(0,NULL,NULL));
    pthread_mutex_init(&mutex,NULL);
}

mysql_pool::~mysql_pool()
{
    while(!connect_pool.empty())
    {
        mysql_close(connect_pool.back());
        connect_pool.pop_back();
    }
    mysql_library_end();
}

mysql_pool *mysql_pool::get_instance()
{
    static mysql_pool instance;
    return &instance;
}

void mysql_pool::init(const string host,const string user,
    const string &passwd,const string &db_name,int port)
{

    for(int i=0;i<N_CONNECTIONS;++i)
    {
        MYSQL *con=mysql_init(NULL);
        assert(NULL!=con);
        assert(
            NULL!=mysql_real_connect(
                con,host.c_str(),user.c_str(),passwd.c_str(),
                db_name.c_str(),port,NULL,0)
            );
        connect_pool.push_back(con);
    }
}

MYSQL *mysql_pool::get_connection()
{
    MYSQL *res=NULL;
    pthread_mutex_lock(&mutex);
    if(!connect_pool.empty())
    {
        res=connect_pool.back();
        connect_pool.pop_back();
    }
    pthread_mutex_unlock(&mutex);
    return res;
}

void mysql_pool::return_connection(MYSQL *con)
{
    pthread_mutex_lock(&mutex);
    connect_pool.push_back(con);
    pthread_mutex_unlock(&mutex);
}
