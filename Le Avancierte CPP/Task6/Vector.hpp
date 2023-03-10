#pragma once
#define CHECK assert(Invariant())
#define InitialCapacity 10
#include <cassert>
#include <cstring>
#include "String.h"
#include <ostream>
#include "Iterator.hpp"
using std::ostream;

template <class T>
class Vector {
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
#pragma endregion


#pragma region Iterator Types
	using iterator = Iterator<value_type, 1>;
	using const_iterator = Iterator<const_value_type, 1>;
	using reverse_iterator = Iterator<value_type, -1>;
	using const_reverse_iterator = Iterator<const_value_type, -1>;
#pragma endregion

private:
	pointer container;
	size_type currentCapacity;
	size_type currentSize;
	allocator _allocator;
public:

#pragma region Constructors and destructors

	Vector() noexcept : container(), currentCapacity(0), currentSize(0) { }

	Vector(const char* other) : currentCapacity(std::max((size_type)InitialCapacity, strlen(other) * 2)), currentSize(strlen(other))
	{
		
		container = new T[currentCapacity];
		for (size_type i = 0; i < currentSize; ++i)
			container[i] = other[i];
		CHECK;
	}

	~Vector() noexcept {
		CHECK;
		delete[] container;
	}

	Vector(const Vector& other) : currentCapacity(other.size()){
		container = new T[currentCapacity];
		for (size_type i = 0; i < other.size(); i++) {
			container[i] = other[i];
			currentSize++;
		}
		CHECK;
	}

	Vector(Vector&& other) noexcept {
		*this = std::move(other);
		CHECK;
	}
#pragma endregion

#pragma region Assignment
	Vector<T>& operator=(const Vector<T>& other) {
		if (currentCapacity <= other.size()) {
			setCapacity(other.capacity());
		}

		std::copy(other.begin(), other.end(), container);
		currentSize = other.size();
		CHECK;
		return *this;
	}

	Vector<T>& operator=(Vector<T>&& other) noexcept {
		delete[] container;
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
		currentCapacity = newCapacity;
		T* temporaryData = container;
		container = new T[currentCapacity];
		currentSize = currentCapacity > currentSize ? currentSize : currentCapacity;
		std::copy(temporaryData, (temporaryData + currentSize), container);
		delete[] temporaryData;
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
		if (currentSize >= currentCapacity)
			reserve(currentCapacity * 2 > InitialCapacity ? currentCapacity * 2 : InitialCapacity);
		container[currentSize++] = value;
		CHECK;
	}
	
	void pop_back() {
		if (Empty())
			return;
		if (currentSize <= currentCapacity / 4)
			setCapacity(currentCapacity / 2);
		--currentSize;
		CHECK;
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
	friend bool operator== (const Vector& lhs, const Vector& rhs) {
		if (lhs.size() != rhs.size())
			return false;
		for (size_t i = 0; i < lhs.size(); i++)
			if (lhs[i] != rhs[i])
				return false;

		return true;
	}

	friend auto operator<=> (const Vector& lhs, const Vector& rhs) {
		size_t smallestSize = std::min(lhs.size(), rhs.size());
		for (size_t i = 0; i < smallestSize; i++) {
			auto comparison = lhs.at(i) <=> rhs.at(i);
			if (comparison != 0)
				return comparison;
		}


		return lhs.size() <=> rhs.size();
	}

	friend std::ostream& operator<<(std::ostream& cout, const Vector& other) {
		for (size_t i = 0; i < other.size(); ++i)
			cout << other[i];
		return cout;
	}

	friend void swap(Vector<T>& lhs, Vector<T>& rhs) {
		auto temporaryVector(std::move(lhs));
		lhs = std::move(rhs);
		rhs = std::move(temporaryVector);
	}
#pragma endregion
};