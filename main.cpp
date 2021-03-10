
#include "server/server.h"

int main(int argc,char *argv[])
{
    server s;
    s.run("127.0.0.1",5000);
    return 0;
}
