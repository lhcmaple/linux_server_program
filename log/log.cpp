#include "log.h"

static void *_routine(void *arg)
{
    log *record=(log *)arg;
    record->process();
}

void log::push(std::string mesg)
{
    pthread_mutex_lock(&mutex);
    //add mesg to qlog
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

log::log()
{
    pthread_cond_init(&cond,NULL);
    pthread_mutex_init(&mutex,NULL);
    pthread_create(&tid,NULL,&_routine,this);
}

log::~log()
{
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

log *log::getlog()
{
    static log instance;
    return &instance;
}

void log::process()
{
    while(true)
    {
        pthread_mutex_lock(&mutex);
        while(qlog.empty())
            pthread_cond_wait(&cond,&mutex);
        while(!qlog.empty())
        {
            //process qlog.front();
            qlog.pop();
        }
        pthread_mutex_unlock(&mutex);
    }
}
