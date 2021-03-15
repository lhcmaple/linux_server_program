/* 
 * 
 */

#include "server.h"

epoll_event events[N_EVENT_NUMBER];

void task::process()
{
    http_parser hp;
    char cache[512];
    MYSQL *con=NULL;
    if(hp.parse(fd)==DONE_STATUS)
    {
        switch(hp.method)
        {
            case GET:
                if(hp.uri==string("/")||hp.uri==string("/index.html"))
                {
                    int filefd=open("./root/index.html",O_RDONLY);
                    if(filefd<0)
                        break;
                    sprintf(cache,"HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nConnections: Close\r\n\r\n",lseek(filefd,0,SEEK_END));
                    response(cache,filefd);
                }
                else
                {
                    int filefd=open("./root/404.html",O_RDONLY);
                    if(filefd<0)
                        break;
                    sprintf(cache,"HTTP/1.1 404 NOT FOUND\r\nContent-Length: %ld\r\nConnections: Close\r\n\r\n",lseek(filefd,0,SEEK_END));
                    response(cache,filefd);
                }
                break;
            case POST:
                con=mysql_pool::get_instance()->pop_connection();
                if(hp.type==string("register"))
                {
                    sprintf(cache,"insert into userinfo values(%s,%s);",hp.user.c_str(),hp.password.c_str());
                    
                }
                else if(hp.type==string("recv"))
                {
                    sprintf(cache,"select * from userinfo where username='%s'",hp.user.c_str());
                    mysql_query(con,cache);
                    MYSQL_RES *res=mysql_store_result(con);
                    if(mysql_num_rows(res)==0)
                    {
                        int filefd=open("./root/infoError.html",O_RDONLY);
                        if(filefd<0)
                            break;
                        sprintf(cache,"HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nConnections: Close\r\n\r\n",lseek(filefd,0,SEEK_END));
                        response(cache,filefd);
                        break;
                    }
                    stoi(hp.data);
                }
                else if(hp.type==string("send"))
                {
                    sprintf(cache,"select * from userinfo where username='%s'",hp.user.c_str());
                    if(mysql_query(con,cache) != 0)
                        break;
                    MYSQL_RES *res=mysql_store_result(con);
                    if(mysql_num_rows(res)==0)
                    {
                        int filefd=open("./root/infoError.html",O_RDONLY);
                        if(filefd<0)
                            break;
                        sprintf(cache,"HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nConnections: Close\r\n\r\n",lseek(filefd,0,SEEK_END));
                        response(cache,filefd);
                        break;
                    }
                }
                else
                {
                    int filefd=open("./root/typeError.html",O_RDONLY);
                    if(filefd<0)
                        break;
                    sprintf(cache,"HTTP/1.1 200 OK\r\nContent-Length: %ld\r\nConnections: Close\r\n\r\n",lseek(filefd,0,SEEK_END));
                    response(cache,filefd);
                }
                mysql_pool::get_instance()->push_connection(con);
                break;
            default:
                break;
        }
    }
    close(fd);
}

void task::response(char *cache,int filefd)
{
    int slen,len=strlen(cache);
    while(len>0&&(slen=send(fd,cache,len,0))>0)
    {
        len-=slen;
    }
    if(slen<0)
    {
        close(filefd);
        return;
    }
    lseek(filefd,0,SEEK_SET);
    while((len=read(filefd,cache,512))>0)
    {
        cache[511]=0;
        slen=0;
        while( len > 0 &&
            ( slen = send(fd,cache,len,0) ) > 0 )
        {
            len-=slen;
        }
        if(slen<0)
            break;
    }
    close(filefd);
}

void pipe_handler(int sig)
{

}

static int Socket(int domain,int type,int protocal,const char *ipaddr,short port)
{
    int fd=socket(domain,type,protocal);
    assert(fd>0);
    sockaddr_in addr;
    memset(&addr,0,sizeof(addr));
    addr.sin_family=AF_INET;
    addr.sin_port=htons(port);
    inet_aton(ipaddr,&addr.sin_addr);
    assert(
        0==bind(fd,(sockaddr *)&addr,sizeof(addr))
    );
    return fd;
}

static int setnonblocking(int fd)
{
    int old_option=fcntl(fd,F_GETFL);
    int new_option=old_option|O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

void server::run(const char *ipaddr,short port)
{
    struct sigaction act;
    memset(&act,0,sizeof(act));
    act.sa_handler=&pipe_handler;
    sigaction(SIGPIPE,&act,NULL);

    listenfd=Socket(PF_INET,SOCK_STREAM,0,ipaddr,port);
    setnonblocking(listenfd);
    assert(
        0==listen(listenfd,N_BACKLOG)
    );

    epollfd=epoll_create(N_EPOLL_SIZE);
    assert(epollfd>0);
    epoll_event levent;
    memset(&levent,0,sizeof(levent));
    levent.events|=EPOLLIN;
    levent.data.fd=listenfd;
    epoll_ctl(epollfd,EPOLL_CTL_ADD,listenfd,&levent);

    while(true)
    {
        int number=epoll_wait(epollfd,events,N_EVENT_NUMBER,-1);
        assert(number>0);
        for(int i=0;i<number;++i)
        {
            int sockfd=events[i].data.fd;
            if(sockfd==listenfd)
            {
                epoll_event tmp_event;
                memset(&tmp_event,0,sizeof(tmp_event));
                tmp_event.events|=EPOLLIN;
                while((tmp_event.data.fd=accept(listenfd,NULL,NULL))>0)
                {
                    epoll_ctl(epollfd,EPOLL_CTL_ADD,tmp_event.data.fd,&tmp_event);
                }
                assert(errno==EWOULDBLOCK||errno==EAGAIN);
            }
            else
            {
                epoll_ctl(epollfd,EPOLL_CTL_DEL,sockfd,&events[i]);
                tpool.push(task(sockfd));
            }
        }
    }
}
