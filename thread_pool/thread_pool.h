#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include <pthread.h>
#include <vector>

#include "../lock_queue/lock_queue.h"

using std::vector;

template<typename T> static void *_run(void *);

template<typename TASK,int NPOOL> class thread_pool{
private:
    lock_queue<TASK> qtask;
    vector<pthread_t> pool;
public:
    thread_pool();

    template<typename T> friend void *_run(void *);
};

#endif