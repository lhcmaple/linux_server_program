#include <cstdlib>

#include "server/server.h"
#include "mysql_pool/mysql_pool.h"

int main(int argc,char *argv[])
{
    if(argc<2)
    {
        printf("usage: %s port\n",basename(argv[0]));
        exit(-1);
    }

    mysql_pool::get_instance()->init("127.0.0.1","lhc","1235","db_webchat",3306);
    server s;
    s.run("0.0.0.0",atoi(argv[1]));
    return 0;
}
