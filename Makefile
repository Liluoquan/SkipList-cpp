CC = g++
CXXFLAGS = -std=c++0x
CFLAGS = -I

all:test_base_function test_stress test_threadPool test_client test_server 

test_base_function: ./test/functional_test.cc
	$(CC) -o ./bin/test/test_base_function ./test/functional_test.cc -std=c++11 -lpthread

test_stress: ./test/stress_test.cc
	$(CC) -o ./bin/test/test_stress ./test/stress_test.cc -std=c++11 -lpthread

test_threadPool: ./base/test/test_threadPool.cc
	$(CC) -o ./bin/test/test_threadPool ./base/test/test_threadPool.cc -std=c++11 -lpthread

test_client: ./client/client.cc
	$(CC) -o ./bin/client ./client/client.cc -std=c++11

test_server: ./server/main.cc ./server/Server.cc
	$(CC) -o ./bin/server ./server/main.cc ./server/Server.cc -std=c++11 -lpthread

.PHONY: clean
clean:
	rm -f ./*.o