#ifndef _LOG_H_
#define _LOG_H_

#include <string>
#include <queue>
#include <pthread.h>

class log {
private:
    std::queue<std::string> qlog; 
    pthread_t tid;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
private:
    log();
    void process();
public:
    log *getlog();
    void push(std::string);

    friend void *_routine(void *);
};

#endif
