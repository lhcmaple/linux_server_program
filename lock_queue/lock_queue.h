#ifndef _LOCK_QUEUE_H
#define _LOCK_QUEUE_H

#include <queue>
#include <pthread.h>
#include <stdio.h>

using std::queue;

template<typename T> class lock_queue{
private:
    pthread_mutex_t mutex;
    pthread_cond_t cond;
public:
    queue<T> q;
    lock_queue();
    void push(const T &val);
    T &top();
    void pop();
    bool empty();
    int lock_wait();
    int lock();
    int unlock();
    ~lock_queue();
};

template<typename T> lock_queue<T>::lock_queue():q()
{
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
}

template<typename T> lock_queue<T>::~lock_queue()
{
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
}

template<typename T> void lock_queue<T>::push(const T &val)
{
    q.push(val);
    pthread_cond_signal(&cond);
}

template<typename T> T &lock_queue<T>::top()
{
    return q.front();
}

template<typename T> void lock_queue<T>::pop()
{
    q.pop();
}

template<typename T> bool lock_queue<T>::empty()
{
    return q.empty();
}

template<typename T> int lock_queue<T>::lock_wait()
{
    int ret=pthread_mutex_lock(&mutex);
    while(q.empty())
        pthread_cond_wait(&cond,&mutex);
    return ret;
}

template<typename T> int lock_queue<T>::lock()
{
    return pthread_mutex_lock(&mutex);
}

template<typename T> int lock_queue<T>::unlock()
{
    return pthread_mutex_unlock(&mutex);
}

#endif