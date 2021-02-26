/* 线程池,创建一定数目的线程
 * 每个线程执行同一个函数,争夺队列里的任务并执行,否则空闲
 */

#include<pthread.h>
#include<vector>

using std::vector;

template<typename TASK> class _thread_pool{

private:
    _thread_pool();
public:

};
