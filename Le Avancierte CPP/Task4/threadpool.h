
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <future>
#include <mutex>
#include <condition_variable>

class ThreadPool {

    std::mutex taskMutex;
    std::condition_variable conditionVariable;
    //std::shared_ptr<std::vector<std::thread>> workers = 
    //    std::make_shared<std::vector<std::thread>>();
    std::vector<std::jthread> workers;
    std::queue<std::shared_ptr<std::future<void>>> tasks;
    bool endOfLife = false;
public:
    //used by main (you are free to change here and in main)
    explicit ThreadPool(size_t threads);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args);

    /* todo: your implementation and more methods and variables*/
    void ExecuteTask();
};

#endif


