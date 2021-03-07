/* http状态机
 * 
 */

#include "http_conn.h"

HTTP_STATUS http_parser::parse(int _fd)
{
    fd=_fd;
    start=0;
    line_start=0;
    end=0;
    http_status=REQUEST_STATUS;
    line_status=LINE_OPEN;

    uri.clear();
    version.clear();
    headers.clear();

    while(http_status!=DONE_STATUS
        &&http_status!=ERROR_STATUS)
    {
        switch(http_status)
        {
            case REQUEST_STATUS:
                switch(line_status)
                {
                    case LINE_OPEN:
                        line_status=line_parse();
                        break;
                    case LINE_CLOSE:
                        line_status=LINE_OPEN;
                        http_status=request_parse();
                        break;
                }
                break;
            case HEADER_STATUS:
                switch(line_status)
                {
                    case LINE_OPEN:
                        line_status=line_parse();
                        break;
                    case LINE_CLOSE:
                        line_status=LINE_OPEN;
                        http_status=header_parse();
                        break;
                }
                break;
            case DATA_STATUS:
                break;
        }
    }
    return http_status;
}

LINE_STATUS http_parser::line_parse()
{
    if(start==end)
    {
        if(start==CACHE_SIZE)
        {
            http_status=ERROR_STATUS;
            return LINE_ERROR;
        }
        int len=recv(fd,cache+end,CACHE_SIZE-end,0);
        if(len<0)
        {
            http_status=ERROR_STATUS;
            return LINE_ERROR;
        }
        end=end+len;
    }
    for(int i=start;i<end;++i)
    {
        if(cache[i]=='\n')
        {
            if(i-1>=line_start&&cache[i-1]=='\r')
                line_end=i-1;
            else
                line_end=i;
            start=i+1;
            return LINE_CLOSE;
        }
    }
    return LINE_OPEN;
}

HTTP_STATUS http_parser::request_parse()
{
    int index=line_start;

    while(index<line_end&&cache[index]!=' ') index++;
    if(index==line_end)
        return ERROR_STATUS;
    string tmp(cache+line_start,index-line_start);
    if(tmp==string("GET"))
        method=GET;
    else
        method=UNKNOWN;
    
    while(index<line_end&&cache[index]==' ') index++;
    if(index==line_end)
        return ERROR_STATUS;
    line_start=index;
    while(index<line_end&&cache[index]!=' ') index++;
    if(index==line_end)
        return ERROR_STATUS;
    uri.assign(cache+line_start,index-line_start);

    while(index<line_end&&cache[index]==' ') index++;
    if(index==line_end)
        return ERROR_STATUS;
    version.assign(cache+index,line_end-index);

    line_start=start;
    return HEADER_STATUS;
}

HTTP_STATUS http_parser::header_parse()
{
    if(line_start==line_end)
    {
        line_start=start;
        return DATA_STATUS;
    }

    header htmp;
    int index=line_start;
    while(index<line_end&&cache[index]!=' '&&cache[index]!=':') index++;
    if(index==line_end)
        return ERROR_STATUS;
    htmp.property.assign(cache+line_start,index-line_start);
    
    while(index<line_end&&(cache[index]==' '||cache[index]==':')) index++;
    if(index==line_end)
        return ERROR_STATUS;
    htmp.value.assign(cache+index,line_end-index);

    line_start=start;
    return HEADER_STATUS;
}
