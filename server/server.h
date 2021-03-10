#ifndef _SERVER_H
#define _SERVER_H

#include <arpa/inet.h>
#include <memory.h>
#include <sys/epoll.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "../thread_pool/thread_pool.h"
#include "../http_conn/http_conn.h"

#define N_BACKLOG 64
#define N_EPOLL_SIZE 1000
#define N_EVENT_NUMBER N_EPOLL_SIZE
#define N_THREAD_POOL_SIZE 100

class server{
private:
    int listenfd;
    int epollfd;
public:
    void run(const char *ipaddr,short port);
};

#endif
