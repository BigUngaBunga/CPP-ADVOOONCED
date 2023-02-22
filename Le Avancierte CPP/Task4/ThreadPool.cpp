#include "threadpool.h"

ThreadPool::ThreadPool(size_t threads)
{
	for (size_t i = 0; i < threads; i++) {
		workers.emplace_back(&ThreadPool::ExecuteTask, this);
	}
}

void ThreadPool::ExecuteTask() {
	while (!endOfLife)
	{
		std::unique_lock taskLock(taskMutex);
		//TODO om det inte finns några uppgifter vänta på uppgifter
		conditionVariable.wait(taskLock, [this] {return !tasks.empty(); });
		auto task = std::move(tasks.front());
		tasks.pop();
		taskLock.unlock();
		(*task)();
	}
	
}