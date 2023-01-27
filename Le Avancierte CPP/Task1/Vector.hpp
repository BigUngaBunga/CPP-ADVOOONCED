#pragma once
#define CHECK assert(Invariant());
#define InitialCapacity 10
#include <cassert>
#include <cstring>
#include "String.h"
#include <ostream>
#include "Iterator.hpp"
using std::ostream;

#pragma region Iterator Types
template <class T>
using iterator = Iterator<T, 1>;
template <class T>
using const_iterator = Iterator<const T, 1>;
template <class T>
using reverse_iterator = Iterator<T, -1>;
template <class T>
using const_reverse_iterator = Iterator<const T, -1>;
#pragma endregion

template <class T>
class Vector {

public:
	using iterator = iterator<T>;
	using const_iterator = const_iterator<T>;
	using reverse_iterator = reverse_iterator<T>;
	using const_reverse_iterator = const_reverse_iterator<T>;

private:
	T* container;
	size_t currentCapacity;
	size_t currentSize;

public:
	Vector() noexcept;
	~Vector() noexcept;
	Vector(const char* other);
	Vector(const Vector& other);
	Vector(Vector&& other) noexcept;
	
	Vector& operator=(const Vector& other);
	Vector& operator=(Vector&& other) noexcept;

	size_t size() const noexcept;
	size_t capacity() const noexcept;

	void push_back(const T& value);
	void pop_back();
	void reserve(size_t newCapacity);
	void setCapacity(size_t newCapacity);
	void resize(size_t newSize);
	void shrinkToFit();


	bool Empty() const;
	bool Invariant() const;

	T& operator[] (const size_t i);
	const T& operator[] (size_t i) const;
	T& at(size_t i);
	const T& at(size_t i) const;
	T* data() noexcept;
	const T* data() const noexcept;

	#pragma region iterators
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;
	const_iterator cbegin() const;
	const_iterator cend() const;
	reverse_iterator rbegin();
	reverse_iterator rend();
	const_reverse_iterator rbegin() const;
	const_reverse_iterator rend() const;
	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;
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

#pragma region Constructors and destructors

template<class T>
Vector<T>::Vector() noexcept : container(), currentCapacity(0), currentSize(0) { }

template<class T>
Vector<T>::Vector(const char* other) : currentCapacity(std::max((size_t)InitialCapacity, strlen(other) * 2)), currentSize(strlen(other))
{
	container = new T[currentCapacity];
	for (size_t i = 0; i < currentSize; ++i)
		container[i] = other[i];
	CHECK 
}

template<class T>
Vector<T>::~Vector() noexcept {
	CHECK
	delete[] container;
}

template<class T>
Vector<T>::Vector(const Vector& other) {
	currentCapacity = other.capacity();
	delete[] container;
	container = new T[currentCapacity];
	for (size_t i = 0; i < other.size(); i++) {
		container[i] = other[i];
		currentSize++;
	}
	CHECK
}


template<class T>
Vector<T>::Vector(Vector&& other) noexcept {
	*this = std::move(other);
	CHECK
}
#pragma endregion

#pragma region Size and capacity
template<class T>
void Vector<T>::reserve(size_t newCapacity) {
	if (newCapacity > currentCapacity)
		setCapacity(newCapacity);
}

template<class T>
void Vector<T> ::setCapacity(size_t newCapacity) {
	currentCapacity = newCapacity;
	T* temporaryData = container;
	container = new T[currentCapacity];
	currentSize = currentCapacity > currentSize ? currentSize : currentCapacity;
	std::copy(temporaryData, (temporaryData + currentSize), container);
	delete[] temporaryData;
	CHECK
}

template<class T>
void Vector<T>::resize(size_t newSize) {
	if (newSize >= currentCapacity)
		setCapacity(newSize * 2 + 1);

	if (currentSize < newSize)
	{
		for (auto iterator = (begin() + currentSize); iterator < (begin() + newSize); iterator++) {
			*iterator = T();
		}
	}
	currentSize = newSize;
	CHECK
}

template<class T>
void Vector<T>::shrinkToFit() {
	setCapacity(currentSize);
}
#pragma endregion

#pragma region Iterators
 template<class T>
 iterator<T> Vector<T>::begin() { return iterator(container); }
 template<class T>
 iterator<T> Vector<T>::end() { return iterator(container + currentSize); }

 template<class T>
 const_iterator<T> Vector<T>::begin() const { return const_iterator(container); }
 template<class T>
 const_iterator<T> Vector<T>::end() const { return const_iterator(container + currentSize); }

 template<class T>
 const_iterator<T> Vector<T>::cbegin() const { return const_iterator(container); }
 template<class T>
 const_iterator<T> Vector<T>::cend() const { return const_iterator(container + currentSize); }
 
 template<class T>
 reverse_iterator<T> Vector<T>::rbegin() { return reverse_iterator(container + currentSize - 1); }
 template<class T>
 reverse_iterator<T> Vector<T>::rend() { return reverse_iterator(container - 1); }
 
 template<class T>
 const_reverse_iterator<T> Vector<T>::rbegin() const { return const_reverse_iterator(container + currentSize - 1); }
 template<class T>
 const_reverse_iterator<T> Vector<T>::rend() const { return const_reverse_iterator(container - 1); }

 template<class T>
 const_reverse_iterator<T> Vector<T>::crbegin() const { return const_reverse_iterator(container + currentSize - 1); }
 template<class T>
 const_reverse_iterator<T> Vector<T>::crend() const { return const_reverse_iterator(container - 1); }

#pragma endregion

#pragma region Overloaded operators
 template<class T>
 Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
	if (currentCapacity <= other.size()) {
		setCapacity(other.capacity());
	}

	std::copy(other.begin(), other.end(), container);
	currentSize = other.size();
	CHECK
		return *this;
}

template<class T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
	delete[] container;
	container = std::move(other.container);
	currentCapacity = std::move(other.currentCapacity);
	currentSize = std::move(other.currentSize);
	other.container = nullptr;
	other.currentCapacity = 0;
	other.currentSize = 0;
	CHECK
	return *this;
}
 
template<class T>
T& Vector<T>::operator[] (size_t i) { return *(container + i); }

template<class T>
const T& Vector<T>::operator[] (size_t i) const { return *(container + i); }
#pragma endregion

#pragma region Getters
template<class T>
bool Vector<T>::Empty() const { return !(size() > 0); }

template<class T>
size_t Vector<T>::size() const noexcept { return currentSize; }

template<class T>
size_t Vector<T>::capacity() const noexcept { return currentCapacity; }

template<class T>
T* Vector<T>::data() noexcept { return container; }

template<class T>
const T* Vector<T>::data() const noexcept { return container; }
#pragma endregion

#pragma region Access
template<class T>
void Vector<T>::push_back(const T& value) {
	if (currentSize >= currentCapacity)
		reserve(currentCapacity * 2 > InitialCapacity ? currentCapacity * 2 : InitialCapacity);
	container[currentSize++] = value;
	CHECK
}

template<class T>
void Vector<T>::pop_back() {
	if (Empty())
		return;
	if (currentSize <= currentCapacity / 4)
		setCapacity(currentCapacity / 2);
	--currentSize;
	CHECK
}

template<class T>
T& Vector<T>::at(size_t i) {
	if (i >= currentSize)
		throw std::out_of_range("boot to big");
	return operator[](i);
}

template<class T>
const T& Vector<T>::at(size_t i) const {
	if (i >= currentSize)
		throw std::out_of_range("boot too big");
	return operator[](i);
}
#pragma endregion

#pragma region Debug
template<class T>
bool Vector<T>::Invariant() const {
	bool isValid = size() <= capacity();
	if (capacity() != 0)
		isValid = isValid && container != nullptr;
	return isValid;
}
#pragma endregion
