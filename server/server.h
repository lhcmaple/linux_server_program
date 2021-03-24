#ifndef _SERVER_H
#define _SERVER_H

#include <arpa/inet.h>
#include <memory.h>
#include <sys/epoll.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include "../thread_pool/thread_pool.h"
#include "../http_conn/http_conn.h"
#include "../mysql_pool/mysql_pool.h"

#define N_BACKLOG 64
#define N_EPOLL_SIZE 1000
#define N_EVENT_NUMBER N_EPOLL_SIZE
#define N_THREAD_POOL_SIZE 100
#define N_TIMERTICK_KEEP 60

class task{
private:
    int fd;
public:
    task(int _fd=0):fd(_fd){
    };
    void process();
    void response(char *,int);
};

class server{
private:
    int listenfd;
    int epollfd;
    thread_pool<task,N_THREAD_POOL_SIZE> tpool;
public:
    void run(const char *ipaddr,short port);
};

#endif
