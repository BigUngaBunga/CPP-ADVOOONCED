#pragma once
#define CHECK assert(Invariant())
#include <cassert>
#include <cstring>
#include <algorithm>
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
	size_type currentSize = 0;
	pointer container = nullptr;
	allocator _allocator;

#pragma region Helper Functions
	
	template<class Titerator>
	void CopyElements(const Titerator& begin, const Titerator& end) {
		currentSize = 0;
		std::for_each(begin, end, 
			[&](auto& value) {new (container + currentSize++) T(std::move(value));});
	}

	template<class Titerator>
	void MoveElements(const Titerator& begin, const Titerator& end) {
		currentSize = 0;
		for (auto iterator = begin; iterator < end; iterator++) {
			new (container + currentSize++) T(std::move(*iterator));
			iterator->~T();
		}
	}

	template<class Titerator>
	void DestructElements(const Titerator& begin, const Titerator& end) const {
		std::for_each(begin, end,[](auto& value) {value.~T(); });
	}

	void DeallocateContainer() {
		DestructElements(begin(), end());
		_allocator.deallocate(container, currentCapacity);
		currentCapacity = 0;
		currentSize = 0;
	}
#pragma endregion


public:

#pragma region Constructors and destructors

	Vector2() noexcept : currentCapacity(0){ }

	~Vector2() noexcept {
		CHECK;
		DeallocateContainer();
	}

	template<class Titerator>
	Vector2(const size_t capacity, const Titerator& begin, const Titerator& end) :
		currentCapacity(capacity), 
		container(_allocator.allocate(capacity))
	{
		try
		{
			CopyElements(begin, end);
			CHECK;
		}
		catch (const std::exception&)
		{
			DeallocateContainer();
		}
	}

	explicit(false) Vector2(const char* other) : 
		Vector2(strlen(other), other, other + strlen(other)){}

	Vector2(const Vector2& other) : Vector2(other.size(), other.cbegin(), other.cend()) {}

	Vector2(Vector2&& other) noexcept {
		*this = std::move(other);
		CHECK;
	}
#pragma endregion

#pragma region Assignment
	template<class U>
	void push_back(U value) {
		ResizeIfTooSmall();
		new (container + currentSize++) T(std::move(value));
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
	T& emplace_back(Args&& ... args) {
		ResizeIfTooSmall();
		return *(new (container + currentSize++) T(std::forward<Args>(args)...));
	}

	void swap(Vector2& other) noexcept {
		auto temporaryData = data();
		auto temporarySize = size();
		auto temporaryCapacity = capacity();
		container = other.data();
		currentSize = other.size();
		currentCapacity = other.capacity();
		other.container = temporaryData;
		other.currentSize = temporarySize;
		other.currentCapacity = temporaryCapacity;
	}

	//O(n) tidskomplexitet, utrymmeskomplexitet(other.size), lättläslig
	//Stark säkerhet, antingen ändras inget eller allt.
	Vector2& AssStrong(const Vector2& other) {
		Vector2 temporary(other);
		swap(temporary);
		return *this;
	}

	//Det absolut enklaste sättet jag kunde tänka mig
	//Alternativet är att använda kopiera koden som skrevs i AssStrong
	Vector2& AssSimple(const Vector2& other) {
		return AssStrong(other);
	}

	//O(n) tidskomplexitet, värstafall utrymmeskomplexitet(other.size)
	//allokerar bara nytt minne om other är större än capacity
	//Enkel säkerhet, är alltid brukbar men tar bort sina element 
	Vector2& AssFast(const Vector2& other) {
		destructiveReserve(other.size());
		CopyElements(other.begin(), other.end());
		if (other.size() > size())
		{
			CopyElement(other.begin(), other.begin + size());

		}

		CHECK;
		return *this;
	}
	//TODO ändra så att man bara tilldelar, inte dekonstruerar och sedan konstruerar
	//

	Vector2& Ass(const Vector2& other) {
		return (*this) = other;
	}

	Vector2& operator=(const Vector2& other) { return AssFast(other); }

	Vector2& operator=(Vector2&& other) noexcept {
		DeallocateContainer();
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
		auto endIterator = newCapacity >= currentSize ? end() : begin() + newCapacity;
		auto temporary = Vector2(newCapacity, begin(), endIterator);
		swap(temporary);
		CHECK;
	}

	void destructiveReserve(size_t newCapacity) {
		if (newCapacity > currentCapacity)
			destructiveSetCapacity(newCapacity);
		else
			DestructElements(begin(), end());
	}

	void destructiveSetCapacity(size_t newCapacity) {
		DeallocateContainer();
		container = _allocator.allocate(newCapacity);
		currentCapacity = newCapacity;
		CHECK;
	}

	void resize(size_t newSize) {
		if (newSize >= currentCapacity) {
			auto temporary = container;
			container = _allocator.allocate(newSize);
			MoveElements(temporary, temporary + size());
			_allocator.deallocate(temporary, currentCapacity);
			currentCapacity = newSize;
		}

		while (currentSize < newSize) {
				new(container + currentSize) T{};
				++currentSize;
		}
		if (currentSize > newSize) {
			for (auto i = newSize; i < currentSize; i++)
				(container + i)->~T();
			currentSize = newSize;
		}
		CHECK;
	}
	
	void shrink_to_fit(){ setCapacity(currentSize); }

	void ResizeIfTooSmall() {
		if (currentSize >= currentCapacity) {
			size_t doubledSize = currentCapacity * 2 + 1;
			setCapacity(doubledSize > initialCapacity ? doubledSize : initialCapacity);
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

	T* data() noexcept { return container; }
	const T* data() const noexcept { return container; }
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
	T& operator[] (size_t i) noexcept { return *(container + i); }
	
	const T& operator[] (size_t i) const noexcept { return *(container + i); }

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

	friend void swap(Vector2& lhs, Vector2& rhs) noexcept {
		lhs.swap(rhs);
	}
#pragma endregion
};