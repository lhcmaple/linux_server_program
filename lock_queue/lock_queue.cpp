/* 加锁队列
 * 每次只允许一个线程读/写
 */

#include<queue>
#include<pthread.h>

using std::queue;

template<typename T> class _lock_queue{
private:
    pthread_mutex_t mutex;
    queue<T> q;
public:
    _lock_queue();
    void push(const T &val);
    T &top();
    void pop();
    bool empty();
    int lock();
    int unlock();
    ~_lock_queue();
};

template<typename T> _lock_queue<T>::_lock_queue()
{
    pthread_mutex_init(&mutex,NULL);
}

template<typename T> _lock_queue<T>::~_lock_queue()
{
    pthread_mutex_destroy(&mutex);
}

template<typename T> void _lock_queue<T>::push(const T &val)
{
    q.push(val);
}

template<typename T> T &_lock_queue<T>::top()
{
    return q.top();
}

template<typename T> void _lock_queue<T>::pop()
{
    q.pop();
}

template<typename T> bool _lock_queue<T>::empty()
{
    return q.empty();
}

template<typename T> int _lock_queue<T>::lock()
{
    return pthread_mutex_lock(&mutex);
}

template<typename T> int _lock_queue<T>::unlock()
{
    return pthread_mutex_unlock(&mutex);
}
