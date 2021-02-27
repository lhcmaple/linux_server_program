all:lock_queue/lock_queue.o thread_pool/thread_pool.o

lock_queue/lock_queue.o:lock_queue/lock_queue.cpp
	g++ -c lock_queue/lock_queue.cpp -o lock_queue/lock_queue.o -std=c++11

thead_pool/thread_pool.o:thread_pool/thread_pool.cpp
	g++ -c thread_pool/thread_pool.cpp -o thread/pool thread_pool.o -std=c++11
