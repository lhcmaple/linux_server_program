/* 线程池,创建一定数目的线程
 * 每个线程执行同一个函数,争夺队列里的任务并执行,否则空闲
 */

#include<pthread.h>
#include<vector>

#include "../lock_queue/lock_queue.h"

using std::vector;

template<typename T> static void *_run(void *);

template<typename TASK,int NPOOL> class _thread_pool{
private:
    lock_queue<TASK> qtask;
    vector<pthread_t> pool;
public:
    _thread_pool();

    template<typename T> friend void *_run(void *);
};

template<typename TASK,int NPOOL> _thread_pool<TASK,NPOOL>::_thread_pool():pool(NPOOL)
{
    for(int i=0;i<NPOOL;++i)
    {
        assert(
            0==pthread_create(&pool[i],NULL,&(_run<TASK,NPOOL>),this)
            );
    }
}

template<typename TASK,int NPOOL> void *_run(void *arg)
{
    _thread_pool<TASK,NPOOL> *tpool=(_thread_pool<TASK,NPOOL> *)arg;
    while(true)
    {
        tpool->qtask.lock();
        if(!tpool->qtask.empty())
        {
            TASK task=tpool->qtask.top();
            tpool->qtask.pop();
            tpool->qtask.unlock();
            task->process();
        }
        else
            tpool->qtask.unlock();
    }
    return NULL;
}
