#include <iostream>
#include <chrono>
#include <cstdlib>
#include <pthread.h>
#include <time.h>

#include "../base/SkipList.h"

#define NUM_THREADS 1
#define TEST_COUNT 500000
#define LEVEL_OF_SKIPLIST 18

SkipList<int, std::string> testSkipList(LEVEL_OF_SKIPLIST);

void* insertElement(void* threadId) {
    long tid;
    tid = reinterpret_cast<long> (threadId);
    std::cout << "tid : " << tid << std::endl;
    int temp = TEST_COUNT / NUM_THREADS;

    for(int i = tid * temp, cnt = 0; cnt < temp; ++i) {
        ++cnt;
        testSkipList.insertElement(rand() % TEST_COUNT, "a");
    }

    pthread_exit(NULL);
}

void* getElement(void* threadId) {
    long tid;
    tid = reinterpret_cast<long> (threadId);
    std::cout << "tid : " << tid << std::endl;
    int temp = TEST_COUNT / NUM_THREADS;
    std::string value;

    for(int i = tid * temp, cnt = 0; cnt < temp; ++i) {
        ++cnt;
        testSkipList.searchElement(rand() % TEST_COUNT, value);
    }

    pthread_exit(NULL);
}

void test_insert() {
    srand(time(NULL));
    pthread_t threads[NUM_THREADS];
    int rc;
    int i;

    auto start = std::chrono::high_resolution_clock::now();

    for(i = 0; i < NUM_THREADS; ++i) {
        std::cout << "main() : creating thread, " << i << std::endl;
        rc = pthread_create(&threads[i], NULL, insertElement, (void *)i);

        if(rc) {
            std::cout << "Error : unable to create thread," << rc << std::endl;
            exit(-1);
        }
    }

    void* ret;
    for(i = 0; i < NUM_THREADS; ++i) {
        if(pthread_join(threads[i], &ret) != 0) {
            perror("pthread_join() error");
            exit(3);
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "insert costs: " << elapsed.count() << "s" << std::endl;
    // pthread_exit(NULL); // 只终止当前线程，不影响其他正在执行的线程
}

void test_get() {
    srand(time(NULL));
    pthread_t threads[NUM_THREADS];
    int rc;
    int i;

    auto start = std::chrono::high_resolution_clock::now();

    for(i = 0; i < NUM_THREADS; ++i) {
        std::cout << "main() : creating thread, " << i << std::endl;
        rc = pthread_create(&threads[i], NULL, getElement, (void *)i);

        if(rc) {
            std::cout << "Error : unable to create thread," << rc << std::endl;
            exit(-1);
        }
    }

    void* ret;
    for(i = 0; i < NUM_THREADS; ++i) {
        if(pthread_join(threads[i], &ret) != 0) {
            perror("pthread_join() error");
            exit(3);
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "get costs: " << elapsed.count() << "s" << std::endl;
    // pthread_exit(NULL); // 只终止当前线程，不影响其他正在执行的线程
}


int main(int argc, char* argv[]) {
    srand(time(NULL));
    
    test_insert();
    
    test_get();

    return 0;
}

