all:lock_queue/lock_queue.o thread_pool/thread_pool.o mysql_pool/mysql_pool.o http_conn/http_conn.o server/server.o
	g++ -c main.cpp -o main.o -std=c++11
	g++ lock_queue/lock_queue.o thread_pool/thread_pool.o mysql_pool/mysql_pool.o server/server.o main.o -o webserver -std=c++11

lock_queue/lock_queue.o:lock_queue/lock_queue.cpp
	g++ -c lock_queue/lock_queue.cpp -o lock_queue/lock_queue.o -std=c++11

thead_pool/thread_pool.o:thread_pool/thread_pool.cpp
	g++ -c thread_pool/thread_pool.cpp -o thread_pool/thread_pool.o -std=c++11

http_conn/http_conn.o:http_conn/http_conn.cpp
	g++ -c http_conn/http_conn.cpp -o http_conn/http_conn.o -std=c++11

mysql_pool/mysql_pool.o:mysql_pool/mysql_pool.cpp
	g++ -c mysql_pool/mysql_pool.cpp -o mysql_pool/mysql_pool.o -std=c++11 `mysql_config --cflags --libs`

server/server.o:server/server.cpp 
	g++ -c server/server.cpp -o server/server.o -std=c++11

clean:
	rm lock_queue/lock_queue.o thread_pool/thread_pool.o mysql_pool/mysql_pool.o

test.out:test/test.cpp http_conn/http_conn.o
	g++ -c test/test.cpp -o test/test.o -std=c++11
	g++ test/test.o http_conn/http_conn.o -o test.out -std=c++11
	./test.out
