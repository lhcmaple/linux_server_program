#include "timer.h"

static TIMETICK tick = 0;
static sem_t sem;
static bool STOP;

TIMETICK gettick()
{
    return tick;
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

void *_routine(void *arg)
{
    timer *t=(timer *)arg;
    while(!STOP)
    {
        sem_wait(&sem);
        pthread_mutex_lock(&t->mutex);
        // list_node *tmp=t->lst;
        // while(tmp->next!=t->lst)
        // {
        //     printf("%d",*(int *)(tmp->next->context));
        //     tmp=tmp->next;
        // }
        // printf("\n");
        while(!ISEMPTY(t->lst))
        {
            if(FRONT(t->lst)->deadtime>gettick())
                break;
            list_node *tmp=FRONT(t->lst);
            POP(t->lst);
            INIT_LIST(tmp);
            tmp->routine(tmp->context);
        }
        pthread_mutex_unlock(&t->mutex);
    }
    return 0;
}

timer::timer()
{
    lst=new list_node;
    INIT_LIST(lst);
    pthread_mutex_init(&mutex,NULL);
    tick_start();
    pthread_create(&tid,NULL,&_routine,this);
}

timer::~timer()
{
    tick_end();
    pthread_join(tid,NULL);
    pthread_mutex_destroy(&mutex);
    delete lst;
}

void timer::schedule(list_node *t)
{
    pthread_mutex_lock(&mutex);
    PUSH(lst,t);
    pthread_mutex_unlock(&mutex);
}

void timer::cancel(list_node *t)
{
    if(!ISEMPTY(t))//未被执行过
    {
        pthread_mutex_lock(&mutex);
        // list_node *tmp=lst;
        // while(tmp->next!=lst)
        // {
        //     printf("%d",*(int *)(tmp->next->context));
        //     tmp=tmp->next;
        // }
        // printf("\n");
        POP(t->prev);
        pthread_mutex_unlock(&mutex);
        // printf("*****\n");
        // INIT_LIST(t);
        // tmp=lst;
        // while(tmp->next!=lst)
        // {
        //     printf("%d",*(int *)(tmp->next->context));
        //     tmp=tmp->next;
        // }
        // printf("\n");
    }
}

timer *timer::gettimer()
{
    static timer instance;
    return &instance;
}
//GET / HTTP/1.1