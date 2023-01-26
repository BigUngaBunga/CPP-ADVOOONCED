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

	//TODO lägg till <=> och ==
public:
	Iterator(pointer _pointer);
	Iterator(const Iterator<T, direction>& other);
	Iterator();
	~Iterator();

	Iterator& operator=(const Iterator& other);

	T& operator*() const { return *data; }
	T* operator->() const { return data; }
	T& operator[](size_t i) const { return data[i]; }
	Iterator operator++(int change) {
		auto temporary = this;
		move(1);
		return temporary;
	}
	Iterator operator--(int change) {
		auto temporary = this;
		move(-1);
		return temporary;
	}
	Iterator operator+(difference_type value) const {
		auto temporary = this;
		temporary.move(value);
		return temporary;
	}
	Iterator operator-(difference_type value) const {
		auto temporary = this;
		temporary.move(-value);
		return temporary;
	}
	Iterator& operator++() { move(1); return this; }
	Iterator& operator--() { move(-1); return this; }
	Iterator& operator+=(difference_type value) { move(value); return this; }
	Iterator& operator-=(difference_type value) { move(-value); return this; }
	difference_type operator-(const Iterator& other) const { return data - other.data; }

private:
	pointer data;
	void move(difference_type distance) { data += distance * direction; }
};

template<class T, int direction>
Iterator<T, direction>::Iterator(pointer data) : data(data) {}

template<class T, int direction>
Iterator<T, direction>::Iterator(const Iterator<T, direction>& other) : data(other.data) {}

template<class T, int direction>
Iterator<T, direction>::Iterator() : data(nullptr) {}


template<class T, int direction>
Iterator<T, direction>::~Iterator()
{
	//TODO tänk över om pekaren skall bort. Den fås av föremål som vill behålla sina pekare
	//if (pointer != nullptr)
	//	delete pointer;
}

#pragma endregion