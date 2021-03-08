#ifndef _HTTP_CONN_H
#define _HTTP_CONN_H

#include <vector>
#include <string>
#include <sys/socket.h>

using std::string;
using std::vector;

#define CACHE_SIZE 4096

enum HTTP_STATUS{
    REQUEST_STATUS=0,
    HEADER_STATUS=1,
    DATA_STATUS=2,
    DONE_STATUS=3,
    ERROR_STATUS=4
};

enum LINE_STATUS{
    LINE_CLOSE=0,
    LINE_OPEN=1,
    LINE_ERROR=2
};

enum REQUEST_TYPE{
    GET=0,
    UNKNOWN=1
};

struct header{
    string property;
    string value;
};

class http_parser{
private:
    char cache[CACHE_SIZE];
    int line_start;
    int line_end;
    int start;
    int end;
    HTTP_STATUS http_status;
    LINE_STATUS line_status;
    int fd;
public:
    int data_len;
    string data;
    REQUEST_TYPE method;
    string uri;
    string version;
    vector<header> headers;
public:
    HTTP_STATUS parse(int _fd);
    LINE_STATUS line_parse();
    HTTP_STATUS request_parse();
    HTTP_STATUS header_parse();
    HTTP_STATUS data_parse();
};

#endif
