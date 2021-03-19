all:http_conn/http_conn.cpp mysql_pool/mysql_pool.cpp server/server.cpp main.cpp
	g++ -g $^ -o webserver -std=c++11 `mysql_config --cflags --libs` -lpthread

clean:
	rm -rf webserver test.out

test.out:test/test.cpp http_conn/http_conn.cpp
	g++ $^ -o test.out -std=c++11
	./test.out
