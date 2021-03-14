#ifndef _HTTP_CONN_H
#define _HTTP_CONN_H

#include <vector>
#include <string>
#include <sys/socket.h>
#include <cstring>

using std::string;
using std::vector;

#define CACHE_SIZE 4096

enum HTTP_STATUS{
    REQUEST_STATUS=0,
    HEADER_STATUS,
    DATA_STATUS,
    DONE_STATUS,
    ERROR_STATUS
};

enum LINE_STATUS{
    LINE_CLOSE=0,
    LINE_OPEN,
    LINE_ERROR
};

enum REQUEST_TYPE{
    GET=0,
    POST,
    UNKNOWN
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
    int data_len;
public:
    REQUEST_TYPE method;
    string uri;
    string version;
    vector<header> headers;

    string user;
    string password;
    string type;
    string data;
public:
    HTTP_STATUS parse(int _fd);
    LINE_STATUS line_parse();
    HTTP_STATUS request_parse();
    HTTP_STATUS header_parse();
    HTTP_STATUS data_parse();
};

#endif
