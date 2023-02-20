
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <functional>
#include <mutex>
#include <condition_variable>

class ThreadPool {

    std::mutex taskMutex;
    std::condition_variable conditionVariable;
    std::unique_ptr<std::vector<std::thread>> workers = 
        std::make_unique<std::vector<std::thread>>();
    //std::queue<std::unique_ptr<>> tasks;
    bool endOfLife = false;
public:
    //used by main (you are free to change here and in main)
    explicit ThreadPool(size_t threads);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args);

    /* todo: your implementation and more methods and variables*/
    void ExecuteTask() {
        while (!endOfLife)
        {
            {
                std::unique_lock<std::mutex> taskLock(taskMutex);
//TODO om det inte finns några uppgifter vänta på uppgifter
                //if (true)
                //    conditionVariable.wait(taskLock);
            }
        }
    }
};

#endif


