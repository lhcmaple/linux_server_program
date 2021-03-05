#ifndef _LOCK_QUEUE_H
#define _LOCK_QUEUE_H

#include <queue>
#include <pthread.h>

using std::queue;

template<typename T> class lock_queue{
private:
    pthread_mutex_t mutex;
    queue<T> q;
public:
    lock_queue();
    void push(const T &val);
    T &top();
    void pop();
    bool empty();
    int lock();
    int unlock();
    ~lock_queue();
};

#endif