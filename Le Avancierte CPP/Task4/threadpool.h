
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>               
#include <queue>                
#include <thread>               
#include <functional>   //std::bind
#include <future>       //std::future, std::packaged_tast
#include <mutex>
#include <condition_variable>

class ThreadPool {

    std::mutex taskMutex;
    std::condition_variable conditionVariable;
    std::vector<std::thread> workers;
    std::queue<std::shared_ptr<std::packaged_task<void>>> tasks;
    bool endOfLife = false;
public:
    
    
    //used by main (you are free to change here and in main)
    explicit ThreadPool(size_t threads);
    
    template<class F, class... Args>
    static bool IsVoid() { return std::is_same_v<std::invoke_result_t<F, Args...>, void>; }

    template<class F, class... Args, class Return = std::invoke_result_t<F, Args...>>
    Return enqueue(F&& f, Args&&... args){
        using packageType = std::packaged_task<Return()>;

        packageType task(std::bind_front(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<Return> futureTask = task.get_future();
        if constexpr (std::is_void_v<Return>)
        {
            AddTask(std::move(task));
            return;
        }
        else
            AddTask([&task]{std::invoke(task); });

        futureTask.wait();
        return futureTask.get();
    }

    /* todo: your implementation and more methods and variables*/
    void ExecuteTask();
    
    template<class F>
    void AddTask(F&& task) {
        {
            std::unique_lock taskLock(taskMutex);
            tasks.push(std::make_shared<std::packaged_task<void>>(
                std::forward<decltype(task)>(task)));
        }
        conditionVariable.notify_one();
    }
};

#endif


