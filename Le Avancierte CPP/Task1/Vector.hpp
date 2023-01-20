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
	T* data;
	size_t currentCapacity;
	size_t currentSize;
	//constexpr size_t InitialCapacity = 10;

public:
	Vector() noexcept;
	~Vector() noexcept;
	Vector(const char* other);
	Vector(const Vector& other);
	
	Vector& operator=(const Vector& other);

	size_t size() const noexcept;
	size_t capacity() const noexcept;
	void push_back(const T& value);
	void pop_back();
	void Reserve(size_t newCapacity);
	void Resize(size_t newCapacity);
	void ShrinkToFit();


	bool Empty() const;
	bool Invariant() const;
	T& operator[] (const size_t i);
	const T& operator[] (size_t i) const;
	
	T& at(size_t i);
	const T& at(size_t i) const;

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
};

/// <summary>
/// constructors and destructors
/// </summary>

template<class T>
Vector<T>::Vector() noexcept : data(new T[InitialCapacity]), currentCapacity(InitialCapacity), currentSize(0) {CHECK}

template<class T>
Vector<T>::Vector(const char* other) : currentCapacity(std::max((size_t)InitialCapacity, strlen(other) * 2)), currentSize(strlen(other))
{ 
	data = new T[currentCapacity];
	for (size_t i = 0; i < currentSize; ++i)
		data[i] = other[i];
	CHECK 
}

template<class T>
Vector<T>::~Vector() noexcept {
	CHECK
	delete[] data;
}

template<class T>
Vector<T>::Vector(const Vector& other) {
	currentCapacity = other.capacity();
	delete[] data;
	data = new T[currentCapacity];
	for (size_t i = 0; i < other.size(); i++) {
		data[i] = other[i];
		currentSize++;
	}
	CHECK
}

template<class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
	if (currentCapacity <= other.size()) {
		//TODO öka storlek
	}

	std::copy(other.Begin(), other.End(), data);
	currentSize = other.size();
	CHECK
	return *this;
}

/// <summary>
/// Member functions
/// </summary>

template<class T>
bool Vector<T>::Invariant() const {
	return size() <= capacity() && data != nullptr;
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
		Reserve(currentCapacity * 2 + 1);
	}
	data[currentSize++] = value;
	CHECK
}


template<class T>
void Vector<T>::pop_back() {
	if (Empty())
		return;
	--currentSize;
	if (currentSize <= currentCapacity / 4) {
		Resize(currentCapacity / 2);
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
void Vector<T>::Reserve(size_t newCapacity) {
	if (newCapacity > currentCapacity)
		Resize(newCapacity);
}

template<class T>
void Vector<T>::Resize(size_t newCapacity) {
	currentCapacity = newCapacity;
	T* temporaryData = data;
	data = new T[currentCapacity];
	size_t newSize = currentCapacity > currentSize ? currentSize : currentCapacity;
	std::copy(temporaryData, (temporaryData + newSize), data);
	currentSize = newSize;
	delete[] temporaryData;
	CHECK
}

template<class T>
void Vector<T>::ShrinkToFit() {
	Resize(currentSize);
}

 /// <summary>
 /// Iterators
 /// </summary>

 template<class T>
 T* Vector<T>::Begin() const { return data; }
 
 template<class T>
 T* Vector<T>::End() const { return (data + currentSize); }



/// <summary>
/// Overloaded operators
/// </summary>

template<class T>
T& Vector<T>::operator[] (size_t i) {
	return *(data + i);
}

template<class T>
const T& Vector<T>::operator[] (size_t i) const {
	return *(data + i);
}
