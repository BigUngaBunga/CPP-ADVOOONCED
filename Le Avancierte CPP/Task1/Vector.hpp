#pragma once
#define CHECK assert(Invariant());
template <class T>
class Vector {

private:
	T * data;
	size_t currentSize;
	size_t currentCapacity;

public:
	Vector(size_t initialCapacity = 10) noexcept;
	~Vector() noexcept;
	Vector(const char* other);

	size_t size() const noexcept;
	size_t capacity() const noexcept;

	bool Invariant() const;


};

template<class T>
Vector<T>::Vector(size_t initialCapacity) noexcept : currentCapacity(initialCapacity), currentSize(0), data(new T[currentCapacity]) {CHECK}

template<class T>
Vector<T>::~Vector() noexcept {
	CHECK
	delete[] data;
}

template<class T>
size_t Vector<T>::size() const noexcept {
	return currentSize;
}

template<class T>
size_t Vector<T>::capacity() const noexcept {
	return currentCapacity;
}

template<class T>
bool Vector<T>::Invariant() const {
	return size() <= capacity() && data != nullptr;
}

