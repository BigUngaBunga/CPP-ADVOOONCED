#pragma once
#define CHECK assert(Invariant());

#include <cassert>
#include <utility>
#include "PointerCounter.hpp"

template <class T>
class SharedPtr;

template<class T>
class WeakPtr {
private:
	T* pointer;
	PointerCounter* counter;

public:
#pragma region Constructor
	WeakPtr() noexcept : pointer(nullptr), counter(nullptr) { CHECK; }
	WeakPtr(std::nullptr_t) noexcept : pointer(nullptr), counter(nullptr) { CHECK; }

	WeakPtr(const SharedPtr<T>& sharedPointer) noexcept : pointer(sharedPointer.get()), counter(sharedPointer.getCounter()) {
		tryAddReference();
		CHECK;
	}

	WeakPtr(const WeakPtr& weakPointer) noexcept : pointer(weakPointer.pointer), counter(weakPointer.counter) {
		tryAddReference();
		CHECK;
	}

	~WeakPtr() noexcept {
		CHECK;
		tryRemoveReference();
	}

#pragma endregion

	SharedPtr<T> lock() const noexcept {
		CHECK;
		return SharedPtr<T>(*this);
	}

	bool expired() noexcept {//TODO ta bort referens till räknare om det inte finns något i pekaren
		if (counter != nullptr)
			return counter->hardCount() <= 0;
		return true;
	}

	T* get() const noexcept {
		return pointer;
	}
	PointerCounter* getCounter() const noexcept {
		return counter;
	}

#pragma region operators
	WeakPtr<T>& operator=(WeakPtr<T>& weakPtr) noexcept {
		tryAddReference(weakPtr.counter);
		tryRemoveReference();
		pointer = weakPtr.pointer;
		counter = weakPtr.counter;
		CHECK;
		return *this;
	}

	WeakPtr<T>& operator=(WeakPtr<T>&& sharedPtr) noexcept {
		auto temporaryCounter = std::move(sharedPtr.counter);
		auto temporaryPointer = std::move(sharedPtr.pointer);

		tryAddReference(temporaryCounter);
		if (counter != temporaryCounter)
			tryRemoveReference();
		sharedPtr.reset();

		counter = std::move(temporaryCounter);
		pointer = std::move(temporaryPointer);
		CHECK;
		return *this;
	}

	WeakPtr<T>& operator=(SharedPtr<T>& sharedPtr) noexcept {
		tryAddReference(sharedPtr.getCounter());
		tryRemoveReference();
		pointer = sharedPtr.get();
		counter = sharedPtr.getCounter();
		CHECK;
		return *this;
	}

	friend void swap(WeakPtr& lhs, WeakPtr& rhs) noexcept {
		auto temporary(std::move(lhs));
		lhs = std::move(rhs);
		rhs = std::move(temporary);
	}

	friend bool operator== (const WeakPtr& lhs, const WeakPtr& rhs) noexcept {
		return lhs <=> rhs == 0;
	}

	friend auto operator<=> (const WeakPtr& lhs, const WeakPtr& rhs) noexcept {
		return lhs.get() <=> rhs.get();
	}
#pragma endregion

#pragma region HelperFunctions
	void reset() noexcept {
		CHECK;
		tryRemoveReference();
		pointer = nullptr;
		counter = nullptr;
	}

private:
	void tryAddReference() const noexcept { tryAddReference(counter); }
	void tryAddReference(PointerCounter* counter) const noexcept {
		if (counter != nullptr)
			counter->addWeakReference();
	}

	void tryRemoveReference() noexcept { tryRemoveReference(counter); }
	void tryRemoveReference(PointerCounter* counter) noexcept {
		if (counter != nullptr) {
			counter->removeWeakReference();
			if (counter->Empty()) { delete counter; }
		}
	}
#pragma endregion
public:
#pragma region Debug
	bool Invariant() const noexcept {
		if (pointer == nullptr)
			return true;
		return counter != nullptr && counter->weakCount() > 0;
	}
#pragma endregion
};