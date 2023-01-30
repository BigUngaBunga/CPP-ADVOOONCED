#pragma once
#define CHECK assert(Invariant());

#include <cassert>
#include "PointerCounter.hpp"

template <class T>
class SharedPtr {
private:
	T* pointer;
	PointerCounter* counter;

public:
	SharedPtr() noexcept;
	explicit SharedPtr(T* pointer);
	~SharedPtr() noexcept;


	bool Invariant() const noexcept;
	T* get() const noexcept;
	size_t use_count() const noexcept;
};

template<class T>
SharedPtr<T>::SharedPtr() noexcept : pointer(nullptr), counter(nullptr) {}

template<class T>
SharedPtr<T>::SharedPtr(T* pointer)  : pointer(pointer) {
	//TODO skapa eller få tag på räknare
	counter->AddHardReference();
}

template<class T>
SharedPtr<T>::~SharedPtr() noexcept {
	counter->RemoveHardReference();
	if (counter->CanDestroy()){delete pointer;}
	if (counter->Empty()){delete counter;}
}

template<class T>
T* SharedPtr<T>::get() const noexcept { return pointer; }

template<class T>
size_t SharedPtr<T>::use_count() const noexcept {
	CHECK
	return counter->hardCount();
}

template<class T>
bool SharedPtr<T>::Invariant() const noexcept {
	if (pointer == nullptr)
		return true;
	return counter != nullptr && counter->hardCount() > 0;
}