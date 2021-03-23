#ifndef _TIMER_H_
#define _TIMER_H_

#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <memory.h>
#include <pthread.h>
#include <assert.h>

typedef unsigned long long TIMETICK;

#define SECONDSPERTICK 1

typedef void (*time_task)(void *);

class timer{
private:
struct list_node {
    time_task routine;
    void *context;
    TIMETICK deadtime;
    list_node *prev;
    list_node *next;
};

#define INIT_LIST(lst)\
{\
    lst->prev = lst;\
    lst->next = lst;\
}
#define ISEMPTY(lst) (lst->next == lst)
#define PUSH(lst,node)\
{\
    node->next = lst;\
    node->prev = lst->prev;\
    lst->prev->next = node;\
    lst->prev = node;\
}
#define POP(lst)\
{\
    lst->next = lst->next->next;\
    lst->next->prev = lst;\
}

private:
    list_node *lst;
    pthread_mutex_t mutex;

private:
    timer();

public:
    static timer *gettimer();

    void schedule(time_task,void *,TIMETICK);
    ~timer();
};

#endif
