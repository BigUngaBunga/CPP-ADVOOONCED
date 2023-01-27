#pragma once
#pragma region Iterator
template<class T, int direction>
class Iterator
{
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T*;
	using reference = T&;
	using iterator_category = std::random_access_iterator_tag;

public:
	Iterator(pointer _pointer);
	Iterator(const Iterator<T, direction>& other);
	Iterator();
	~Iterator();

	Iterator& operator=(const Iterator& other) { data = other.data; return *this; }

	//T& operator*() const { return direction == -1 ? *(data - 1) : *data; }
	//T* operator->() const { return direction == -1 ? (data - 1) : data; }
	//T& operator[](size_t i) const { return direction == -1 ? (data - 1)[i] : data[i]; }

	T& operator*() const { return *data; }
	T* operator->() const { return data; }
	T& operator[](size_t i) const { return data[i]; }
	Iterator operator++(int change) {
		auto temporary = this;
		move(1);
		return *temporary;
	}
	Iterator operator--(int change) {
		auto temporary = this;
		move(-1);
		return *temporary;
	}
	Iterator operator+(difference_type value) const {
		/*auto temporary = this;
		temporary += value;*/
		return getMovedIterator(value);
	}
	Iterator operator-(difference_type value) const {
		auto temporary(this);
		return *(temporary -= value);
	}
	Iterator& operator++() { move(1); return *this; }
	Iterator& operator--() { move(-1); return *this; }
	Iterator& operator+=(difference_type value) { move(value); return *this; }
	Iterator& operator-=(difference_type value) { move(-value); return *this; }
	difference_type operator-(const Iterator& other) const { return data - other.data; }

	friend bool operator== (const Iterator& lhs, const Iterator& rhs) {
		return lhs.operator*() == rhs.operator*();
	}

	friend auto operator<=> (const Iterator& lhs, const Iterator& rhs) {
		return lhs - rhs;
	}

private:
	pointer data;
	void move(difference_type distance) { data += distance * direction; }
	Iterator<T, direction> getMovedIterator(difference_type distance) const {
		auto newData = data;
		newData += distance;
		//Iterator<T, direction> temporary(*this);
		//Iterator<T, direction> temporary2(*this);
		Iterator<T, direction> temporary3(data);
		Iterator<T, direction> temporary4(newData);
		
		return temporary4;
	}
};

#pragma region Constructors
template<class T, int direction>
Iterator<T, direction>::Iterator(pointer _pointer) : data(_pointer) {}

template<class T, int direction>
Iterator<T, direction>::Iterator(const Iterator<T, direction>& other) : data(other.data) {}

template<class T, int direction>
Iterator<T, direction>::Iterator() : data(nullptr) {}

template<class T, int direction>
Iterator<T, direction>::~Iterator(){}
#pragma endregion

#pragma endregion