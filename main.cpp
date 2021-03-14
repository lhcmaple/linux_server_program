#include <cstdlib>

#include "server/server.h"

int main(int argc,char *argv[])
{
    if(argc<2)
    {
        printf("usage: %s port\n",basename(argv[0]));
        exit(-1);
    }

    server s;
    s.run("0.0.0.0",atoi(argv[1]));
    return 0;
}
