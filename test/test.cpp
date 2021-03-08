#include "../http_conn/http_conn.h"
#include <arpa/inet.h>

int main(int argc,char *argv[])
{
    int fd=socket(PF_INET,SOCK_STREAM,0);
    sockaddr_in addr;
    addr.sin_family=AF_INET;
    addr.sin_port=htons(5000);
    inet_aton("127.0.0.1",&addr.sin_addr);
    bind(fd,(sockaddr *)&addr,sizeof(addr));
    listen(fd,64);
    int clifd=accept(fd,NULL,NULL);
    http_parser hp;
    printf("%d\n",hp.parse(clifd));
    printf("%s %s %s\n",hp.method==GET?"GET":"UNKNOWN",hp.uri.c_str(),hp.version.c_str());
    for(auto &h:hp.headers)
        printf("%s:%s\n",h.property.c_str(),h.value.c_str());
    if(hp.data_len>0)
        printf("%s",hp.data.c_str());
    return 0;
}