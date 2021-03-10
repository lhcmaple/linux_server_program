/* 线程池,创建一定数目的线程
 * 每个线程执行同一个函数,争夺队列里的任务并执行,否则空闲
 */

#include "thread_pool.h"

template<typename TASK,int NPOOL> thread_pool<TASK,NPOOL>::thread_pool():pool(NPOOL),qtask()
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
    thread_pool<TASK,NPOOL> *tpool=(thread_pool<TASK,NPOOL> *)arg;
    TASK task;
    while(true)
    {
        tpool->qtask.lock();
        if(!tpool->qtask.empty())
        {
            task=tpool->qtask.top();
            tpool->qtask.pop();
            tpool->qtask.unlock();
            task.process();
        }
        else
            tpool->qtask.unlock();
    }
    return NULL;
}

template<typename TASK,int NPOOL> void thread_pool<TASK,NPOOL>::push(TASK t)
{
    qtask.lock();
    qtask.push(t);
    qtask.unlock();
}