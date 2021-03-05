/* 加锁队列
 * 每次只允许一个线程读/写
 */

#include "lock_queue.h"

template<typename T> lock_queue<T>::lock_queue()
{
    pthread_mutex_init(&mutex,NULL);
}

template<typename T> lock_queue<T>::~lock_queue()
{
    pthread_mutex_destroy(&mutex);
}

template<typename T> void lock_queue<T>::push(const T &val)
{
    q.push(val);
}

template<typename T> T &lock_queue<T>::top()
{
    return q.top();
}

template<typename T> void lock_queue<T>::pop()
{
    q.pop();
}

template<typename T> bool lock_queue<T>::empty()
{
    return q.empty();
}

template<typename T> int lock_queue<T>::lock()
{
    return pthread_mutex_lock(&mutex);
}

template<typename T> int lock_queue<T>::unlock()
{
    return pthread_mutex_unlock(&mutex);
}
