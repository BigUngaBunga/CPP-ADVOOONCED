#include "threadpool.h"

ThreadPool::ThreadPool(size_t threads)
{
	for (size_t i = 0; i < threads; i++) {
		workers.emplace_back(&ExecuteTask);
	}
}

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) {
	auto boundTask = std::bind_front(std::forward<F>(f), std::forward<Args>(args)...);
	//std::packaged_task<(F), (Args ...)> packagedFunction = 
	std::packaged_task<F(args...)> packagedTask(boundTask);

	//std::packaged_task<int(int, int)> packagedTask([](int a, int b) {return a+b;});
	std::future< F(args...)> futureTask = packagedTask.get_future();
	tasks.push(std::make_unique<std::future< F(args...)>>(futureTask));
	//TODO add to queue
	conditionVariable.notify_one();
	futureTask.wait();
	return futureTask.get();
}

void ThreadPool::ExecuteTask() {
	std::shared_ptr<std::future<void>> task;
	while (!endOfLife)
	{
		{
			std::unique_lock<std::mutex> taskLock(taskMutex);
			//TODO om det inte finns några uppgifter vänta på uppgifter
			conditionVariable.wait(taskLock, [&] {return !tasks.empty(); });
			task = tasks.front();

		}
	}
}