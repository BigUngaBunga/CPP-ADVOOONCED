#pragma once
#define CHECK assert(Invariant());
//#define InitialCapacity = 10;
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
	const size_t InitialCapacity = 10;

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
Vector<T>::Vector() noexcept : data(new T[InitialCapacity]), currentCapacity(sizeof(data)), currentSize(0) {CHECK}

template<class T>
Vector<T>::Vector(const char* other) : data(new T[strlen(other) * 2]), currentCapacity(sizeof(data)), currentSize(strlen(other))
{ 
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
	{}//TODO double capacity
	data[currentSize++] = value;
	CHECK
}


template<class T>
void Vector<T>::pop_back() {
	if (Empty())
		return;
	--currentSize;
	if (currentSize <= currentCapacity / 4) {
		//TODO halve capacity
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

//void Reserve(size_t newCapacity);
//void Resize(size_t newCapacity);
//void ShrinkToFit();

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
