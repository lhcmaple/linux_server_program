#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <pthread.h>
#include <vector>
#include <stdio.h>

#include "../lock_queue/lock_queue.h"

using std::vector;

template<typename T,int NPOOL> void *_run(void *);

template<typename TASK,int NPOOL> class thread_pool{
private:
    vector<pthread_t> pool;
    lock_queue<TASK> qtask;
public:
    thread_pool();
    void push(TASK t);
    template<typename T,int N> friend void *_run(void *);
};

template<typename TASK,int NPOOL> thread_pool<TASK,NPOOL>::thread_pool():pool(NPOOL),qtask()
{
    for(int i=0;i<NPOOL;++i)
    {
        // printf("thread %d initilizes\n",i);
        assert(
            0==pthread_create(&pool[i],NULL,&(_run<TASK,NPOOL>),this)
            );
    }
}

template<typename TASK,int NPOOL> void *_run(void *arg)
{
    thread_pool<TASK,NPOOL> *tpool=(thread_pool<TASK,NPOOL> *)arg;
    TASK task;
    while(true)
    {
        tpool->qtask.lock_wait();
            // printf("thread %ld\n",pthread_self());
        task=tpool->qtask.top();
        tpool->qtask.pop();
        tpool->qtask.unlock();
        task.process();
    }
    return NULL;
}

template<typename TASK,int NPOOL> void thread_pool<TASK,NPOOL>::push(TASK t)
{
    qtask.lock();
    qtask.push(t);
    qtask.unlock();
}

#endif
