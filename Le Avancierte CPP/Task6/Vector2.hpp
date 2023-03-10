#pragma once
#define CHECK assert(Invariant())
#include <cassert>
#include <cstring>
#include "String.h"
#include <ostream>
#include "Iterator.hpp"
using std::ostream;

template <class T>
class Vector2 {
public:
#pragma region Member Types
	using value_type = T;
	using const_value_type = const T;
	using size_type = size_t;
	using difference_type = std::ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using allocator = Dalloc<T>;
	//using allocator = std::allocator<T>;
#pragma endregion


#pragma region Iterator Types
	using iterator = Iterator<value_type, 1>;
	using const_iterator = Iterator<const_value_type, 1>;
	using reverse_iterator = Iterator<value_type, -1>;
	using const_reverse_iterator = Iterator<const_value_type, -1>;
#pragma endregion

private:
	static const size_type initialCapacity = 10;
	size_type currentCapacity;
	size_type currentSize;
	pointer container;
	allocator _allocator;
public:

#pragma region Constructors and destructors

	Vector2() noexcept : currentCapacity(0), currentSize(0), container() { }

	Vector2(const char* other) :
		currentCapacity(std::max(initialCapacity, strlen(other) * 2)),
		currentSize(strlen(other)),
		container(_allocator.allocate(currentCapacity))
	{
		for (size_type i = 0; i < currentSize; ++i)
			operator[](i) = other[i];
		CHECK;
	}

	~Vector2() noexcept {
		CHECK;
		_allocator.deallocate(container, currentCapacity);
	}

	Vector2(const Vector2& other) : currentCapacity(other.size()),
		container(_allocator.allocate(currentCapacity))
	{
		for (size_type i = 0; i < other.size(); i++) {
			container[i] = other[i];
			currentSize++;
		}
		CHECK;
	}

	Vector2(Vector2&& other) noexcept {
		*this = std::move(other);
		CHECK;
	}
#pragma endregion

#pragma region Assignment
	Vector2& operator=(const Vector2& other) {
		if (currentCapacity <= other.size()) {
			setCapacity(other.capacity());
		}

		std::copy(other.begin(), other.end(), container);
		currentSize = other.size();
		CHECK;
		return *this;
	}

	Vector2& operator=(Vector2&& other) noexcept {
		//delete[] container;
		_allocator.deallocate(container, currentCapacity);
		container = std::move(other.container);
		currentCapacity = std::move(other.currentCapacity);
		currentSize = std::move(other.currentSize);
		other.container = nullptr;
		other.currentCapacity = 0;
		other.currentSize = 0;
		CHECK;
		return *this;
	}
#pragma endregion

#pragma region Size and capacity

	void reserve(size_t newCapacity) {
		if (newCapacity > currentCapacity)
			setCapacity(newCapacity);
	}

	void setCapacity(size_t newCapacity) {
		T* temporaryData = container;
		container = _allocator.allocate(newCapacity);
		currentSize = newCapacity > currentSize ? currentSize : newCapacity;
		std::copy(temporaryData, (temporaryData + currentSize), container);
		_allocator.deallocate(temporaryData, currentCapacity);
		currentCapacity = newCapacity;
		CHECK;
	}

	void resize(size_t newSize) {
		if (newSize >= currentCapacity)
			setCapacity(newSize * 2 + 1);

		if (currentSize < newSize)
		{
			for (auto iterator = (begin() + currentSize); iterator < (begin() + newSize); iterator++) {
				*iterator = T();
			}
		}
		currentSize = newSize;
		CHECK;
	}
	
	void shrink_to_fit(){ setCapacity(currentSize); }

	void ResizeIfTooSmall() {
		if (currentSize >= currentCapacity) {
			size_t doubledSize = currentCapacity * 2 + 1;
			reserve(doubledSize > initialCapacity ? doubledSize : initialCapacity);
		}
	}

	void ResizeIfTooBig() {
		if (currentSize <= currentCapacity / 4)
			setCapacity(currentCapacity / 2);
	}
#pragma endregion

#pragma region Getters
	size_t size() const noexcept { return currentSize; }
	size_t capacity() const noexcept { return currentCapacity; }
	bool Empty() const { return !(size() > 0); }
#pragma endregion

#pragma region Debug
	bool Invariant() const {
		bool isValid = size() <= capacity();
		if (capacity() != 0)
			isValid = isValid && container != nullptr;
		return isValid;
	}
#pragma endregion

#pragma region Access
	T& operator[] (const size_t i) { return *(container + i); }
	
	const T& operator[] (size_t i) const { return *(container + i); }
	
	void push_back(const T& value) {
		ResizeIfTooSmall();
		container[currentSize++] = value;
		CHECK;
	}
	
	void pop_back() {
		if (Empty())
			return;
		ResizeIfTooBig();
		--currentSize;
		CHECK;
	}
	
	template<class... Args>
	reference emplace_back(Args&& ... args) {
		ResizeIfTooSmall();
		return *(new (container + currentSize++) T(std::forward<Args>(args)...));
	}

	T& at(size_t i) {
		if (i >= currentSize)
			throw std::out_of_range("boot to big");
		return operator[](i);
	}
	const T& at(size_t i) const {
		if (i >= currentSize)
			throw std::out_of_range("boot too big");
		return operator[](i);
	}
	
	T* data() noexcept { return container; }
	const T* data() const noexcept { return container; }
#pragma endregion

#pragma region iterators
	iterator begin() { return { container }; }
	iterator end() { return { container + currentSize }; }
	
	const_iterator begin() const { return { container }; }
	const_iterator cbegin() const { return { container }; }
	const_iterator end() const { return { container + currentSize }; }
	const_iterator cend() const { return { container + currentSize }; }
	
	reverse_iterator rbegin() { return { container + currentSize - 1 }; }
	reverse_iterator rend() { return {container - 1}; }
	
	const_reverse_iterator rbegin() const { return { container + currentSize - 1 }; }
	const_reverse_iterator crbegin() const { return { container + currentSize - 1 }; }
	const_reverse_iterator rend() const { return { container - 1 }; }
	const_reverse_iterator crend() const { return { container - 1 }; }
#pragma endregion

#pragma region friends
	friend bool operator== (const Vector2& lhs, const Vector2& rhs) {
		if (lhs.size() != rhs.size())
			return false;
		for (size_t i = 0; i < lhs.size(); i++)
			if (lhs[i] != rhs[i])
				return false;

		return true;
	}

	friend auto operator<=> (const Vector2& lhs, const Vector2& rhs) {
		size_t smallestSize = std::min(lhs.size(), rhs.size());
		for (size_t i = 0; i < smallestSize; i++) {
			auto comparison = lhs.at(i) <=> rhs.at(i);
			if (comparison != 0)
				return comparison;
		}


		return lhs.size() <=> rhs.size();
	}

	friend std::ostream& operator<<(std::ostream& cout, const Vector2& other) {
		for (size_t i = 0; i < other.size(); ++i)
			cout << other[i];
		return cout;
	}

	friend void swap(Vector2& lhs, Vector2& rhs) {
		auto temporaryVector(std::move(lhs));
		lhs = std::move(rhs);
		rhs = std::move(temporaryVector);
	}
#pragma endregion
};