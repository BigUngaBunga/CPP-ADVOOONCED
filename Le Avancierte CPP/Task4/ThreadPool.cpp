#include "threadpool.h"

ThreadPool::ThreadPool(size_t threads)
{
	for (size_t i = 0; i < threads; i++) {
		workers->push_back(std::thread());
	}
}

template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) {
	auto boundFunction = std::bind_front(std::forward<F>(f), std::forward<Args>(args)...);
	conditionVariable.notify_one();
	return boundFunction;
}