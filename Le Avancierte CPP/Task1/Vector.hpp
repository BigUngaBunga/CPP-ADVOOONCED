#pragma once
#define CHECK assert(Invariant());
#define InitialCapacity 10
#include <cstring>
#include "String.h"
#include <ostream>

using std::ostream;

template <class T>
class Vector {

private:
	T* container;
	size_t currentCapacity;
	size_t currentSize;
	//constexpr size_t InitialCapacity = 10;

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

	T* Begin() const;
	T* End() const;

	//Friend functions
	friend bool operator== (const Vector& lhs, const Vector& rhs) {
		if (lhs.size() != rhs.size())
			return false;
		for (size_t i = 0; i < lhs.size(); i++)
			if (lhs[i] != rhs[i])
				return false;

		return true;
	}

	friend bool operator< (const Vector& lhs, const Vector& rhs) {
		if (lhs.size() < rhs.size())
			return true;
		for (size_t i = 0; i < lhs.size(); i++)
			if (lhs[i] < rhs[i])
				return true;

		return false;
	}

	friend bool operator<= (const Vector & lhs, const Vector & rhs) { return !(lhs > rhs); }

	friend bool operator> (const Vector& lhs, const Vector& rhs) { return !(lhs < rhs || lhs.size() == rhs.size()); }

	friend bool operator>= (const Vector& lhs, const Vector& rhs) { return !(lhs < rhs); }

	friend bool operator!= (const Vector& lhs, const Vector& rhs) { return !(lhs == rhs); }

	friend std::ostream& operator<<(std::ostream& cout, const Vector& other) {
		for (size_t i = 0; i < other.size(); ++i)
			cout << other[i];
		return cout;
	}

	friend void swap(Vector<T>& lhs, Vector<T>& rhs) {
		Vector<T> temporaryVector = lhs;
		lhs = rhs;
		rhs = temporaryVector;
	}
};

/// <summary>
/// constructors and destructors
/// </summary>

template<class T>
Vector<T>::Vector() noexcept : container(new T[InitialCapacity]), currentCapacity(InitialCapacity), currentSize(0) {CHECK}

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
	swap(*this, other);
	other.currentSize = 0;
	CHECK
}


template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
	if (currentCapacity <= other.size()) {
		resize(other.capacity());
	}

	std::copy(other.Begin(), other.End(), container);
	currentSize = other.size();
	CHECK
	return *this;
}

template<class T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept{
	swap(*this, other);
	other.currentSize = 0;
	CHECK
	return *this;
}

/// <summary>
/// Member functions
/// </summary>

template<class T>
bool Vector<T>::Invariant() const {
	return size() <= capacity() && container != nullptr;
}
template<class T>
bool Vector<T>::Empty() const {return !(size() > 0);}

template<class T>
size_t Vector<T>::size() const noexcept {
	return currentSize;
}

template<class T>
size_t Vector<T>::capacity() const noexcept {
	return currentCapacity;
}

template<class T>
void Vector<T>::push_back(const T& value) {
	if (currentSize >= currentCapacity)
	{
		reserve(currentCapacity * 2 + 1);
	}
	container[currentSize++] = value;
	CHECK
}


template<class T>
void Vector<T>::pop_back() {
	if (Empty())
		return;
	--currentSize;
	if (currentSize <= currentCapacity / 4) {
		setCapacity(currentCapacity / 2);
	}
	CHECK
}

template<class T>
T& Vector<T>::at(size_t i) {
	if (i < 0 && i >= currentSize) {
		throw std::out_of_range("boot to big");
	}
	return operator[](i);
}

template<class T>
const T& Vector<T>::at(size_t i) const {
	if (i < 0 && i >= currentSize) {
		throw std::out_of_range("boot too big");
	}
	return operator[](i);
}

template<class T>
T* Vector<T>::data() noexcept { return container; }

template<class T>
const T* Vector<T>::data() const noexcept { return container; }

/// <summary>
/// Size and capacity changes
/// </summary>

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
	setCapacity(newSize);
	if (currentSize < currentCapacity)
	{
		for (auto iterator = (Begin() + currentSize); iterator < (Begin() + currentCapacity); iterator++) {
			*iterator = T();
			++currentSize;
		}
	}
	CHECK
}

template<class T>
void Vector<T>::shrinkToFit() {
	setCapacity(currentSize);
}

 /// <summary>
 /// Iterators
 /// </summary>

 template<class T>
 T* Vector<T>::Begin() const { return container; }
 
 template<class T>
 T* Vector<T>::End() const { return (container + currentSize); }



/// <summary>
/// Overloaded operators
/// </summary>

template<class T>
T& Vector<T>::operator[] (size_t i) {
	return *(container + i);
}

template<class T>
const T& Vector<T>::operator[] (size_t i) const {
	return *(container + i);
}
