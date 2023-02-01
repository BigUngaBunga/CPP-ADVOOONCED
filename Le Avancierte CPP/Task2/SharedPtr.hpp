#pragma once
#define CHECK assert(Invariant());

#include <cassert>
#include <utility>
#include "PointerCounter.hpp"

template <class T>
class SharedPtr {
private:
	T* pointer;
	PointerCounter* counter;

public:
	#pragma region constructors
	constexpr SharedPtr() noexcept : pointer(nullptr), counter(nullptr) {	}
	

	explicit SharedPtr(T* pointer) : pointer(pointer) {
		counter = new PointerCounter();
		counter->addHardReference();
		bool boolius = Invariant();
		CHECK;
	}

	constexpr SharedPtr(std::nullptr_t) noexcept : SharedPtr() {}
	
	explicit SharedPtr(SharedPtr<T>& sharedPtr) noexcept : pointer(sharedPtr.pointer), counter(sharedPtr.counter)  {
		tryAddReference();
		CHECK;
	}

	explicit SharedPtr(SharedPtr<T>&& sharedPtr) noexcept : pointer(std::move(sharedPtr.pointer)), counter(std::move(sharedPtr.counter)) {
		sharedPtr.counter = nullptr;
		sharedPtr.pointer = nullptr;
		CHECK;
	}

	~SharedPtr() noexcept {
		CHECK;

		//TODO Är det inte fult att kika på counter?
		if (counter != nullptr)
		{
			counter->removeHardReference();
			if (counter->CanDestroy()) { delete pointer; }
			if (counter->Empty()) { delete counter; }
		}
	}
	#pragma endregion

	
	T* get() const noexcept { 
		CHECK;
		return pointer; 
	}
	size_t use_count() const noexcept {
		CHECK;
		//TODO se över detta, borde vara onödig tertiärsats
		return counter == nullptr ? 0 : counter->hardCount();
	}

	#pragma region operators
	
	SharedPtr<T>& operator=(SharedPtr<T>& sharedPtr) noexcept { 
		tryAddReference(sharedPtr.counter);
		tryRemoveReference();
		pointer = sharedPtr.pointer;
		counter = sharedPtr.counter;
		CHECK;
		return *this;
	}

	SharedPtr<T>& operator=(SharedPtr<T>&& sharedPtr) noexcept { 
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
	
	T& operator*() const noexcept { return *pointer; }
	T* operator->() const noexcept { return pointer; }
	operator bool() const noexcept{ return pointer != nullptr; }

	friend bool operator== (const SharedPtr& lhs, const SharedPtr& rhs) noexcept {
		return lhs <=> rhs == 0;
	}

	friend auto operator<=> (const SharedPtr& lhs, const SharedPtr& rhs) noexcept {
		return lhs.get() <=> rhs.get();
	}
	#pragma endregion



	#pragma region Debug
	bool Invariant() const noexcept {
		if (pointer == nullptr)
			return true;
		return counter != nullptr && counter->hardCount() > 0;
	}
	#pragma endregion

	#pragma region HelperFunctions
	void reset() noexcept {
		CHECK;
		if (counter != nullptr) {
			counter->removeHardReference();
			if (counter->CanDestroy()) { delete pointer; }
			if (counter->Empty()) { delete counter; }
		}//TODO kolla på onödig tilldelning av nullptr
		pointer = nullptr;
		counter = nullptr;
	}
private:
	void tryAddReference() const noexcept { tryAddReference(counter); }
	void tryAddReference(PointerCounter* counter) const noexcept {
		CHECK;
		if (counter != nullptr)
			counter->addHardReference();
	}

	void tryRemoveReference() noexcept { tryRemoveReference(counter); }
	void tryRemoveReference(PointerCounter* counter) noexcept {
		if (counter != nullptr) {
			counter->removeHardReference();
			if (counter->CanDestroy()) { delete pointer; }
			if (counter->Empty()) { delete counter; }
		}
	}
	#pragma endregion
};
