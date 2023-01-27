#pragma once
#pragma region Iterator
template<class T, int direction>
class Iterator
{

public:
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T*;
	using reference = T&;
	using iterator_category = std::random_access_iterator_tag;

	Iterator(pointer _pointer);
	Iterator(const Iterator<T, direction>& other);
	Iterator();
	~Iterator();

	Iterator& operator=(const Iterator& other) { data = other.data; return *this; }

	reference operator*() const { return *data; }
	pointer operator->() const { return data; }
	reference operator[](size_t i) const { return data[i]; }
	Iterator operator++(int change) {
		pointer oldData = data;
		move(1);
		return Iterator(oldData);
	}
	Iterator operator--(int change) {
		pointer oldData = data;
		move(-1);
		return Iterator(oldData);
	}
	Iterator operator+(difference_type value) const {
		return Iterator(data + value);
	}
	Iterator operator-(difference_type value) const {
		return Iterator(data - value);
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
};

#pragma region Constructors
template<class T, int direction>
Iterator<T, direction>::Iterator(pointer _pointer) : data(_pointer) {
	data = _pointer;
}

template<class T, int direction>
Iterator<T, direction>::Iterator(const Iterator<T, direction>& other) : data(other.data) {}

template<class T, int direction>
Iterator<T, direction>::Iterator() : data(nullptr) {}

template<class T, int direction>
Iterator<T, direction>::~Iterator(){}
#pragma endregion

#pragma endregion