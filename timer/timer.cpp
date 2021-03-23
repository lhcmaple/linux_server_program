#include "timer.h"

static TIMETICK tick=0;
static sem_t sem;
static bool STOP;

TIMETICK gettick()
{
    return tick;
}

void tick_start()
{
    sem_init(&sem,0,0);

    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_handler=&alrm_handler;
    sigaction(SIGALRM,&act,NULL);

    STOP=false;
    alarm(SECONDSPERTICK);
}

void tick_end()
{
    STOP=true;
}

void alrm_handler(int sig)
{
    tick++;
    if(!STOP)
    {
        sem_post(&sem);//信号量剩余亦可
        alarm(SECONDSPERTICK);
    }
    else
        sem_destroy(&sem);
}

timer::timer()
{
    lst=new list_node;
    INIT_LIST(lst);
    pthread_mutex_init(&mutex,NULL);
    tick_start();
}

timer::~timer()
{
    tick_end();
    pthread_mutex_destroy(&mutex);
    delete lst;
}

void timer::schedule(time_task t,void *context,TIMETICK deadtime)
{
    list_node *tmp=new list_node;
    tmp->routine=t;
    tmp->context=context;
    tmp->deadtime=deadtime;
    pthread_mutex_lock(&mutex);
    PUSH(lst,tmp);
    pthread_mutex_unlock(&mutex);
}

timer *timer::gettimer()
{
    static timer instance;
    return &instance;
}
