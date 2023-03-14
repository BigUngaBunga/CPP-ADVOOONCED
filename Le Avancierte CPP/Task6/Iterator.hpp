#pragma once
template<class T, int direction>
class Iterator
{

public:
	using difference_type = std::ptrdiff_t;
	using value_type = T;
	using pointer = T*;
	using reference = T&;
	using iterator_category = std::random_access_iterator_tag;

	Iterator(pointer _pointer) noexcept : data(_pointer) {}
	Iterator(const Iterator<T, direction>& other) noexcept : data(other.data) {}
	Iterator() noexcept : data(nullptr) {}
	~Iterator() noexcept = default;


	Iterator& operator=(const Iterator& other) { data = other.data; return *this; }

	reference operator*() const noexcept { return *data; }
	pointer operator->() const noexcept { return data; }
	reference operator[](size_t i) const noexcept { return data[i * direction]; }
	
	#pragma region movement
	Iterator operator++([[maybe_unused]]int change) noexcept {
		pointer oldData = data;
		move(1);
		return Iterator(oldData);
	}
	Iterator operator--([[maybe_unused]] int change) noexcept {
		pointer oldData = data;
		move(-1);
		return Iterator(oldData);
	}
	Iterator operator+(difference_type value) const noexcept { return Iterator(data + value * direction); }
	Iterator operator-(difference_type value) const noexcept { return Iterator(data - value * direction); }
	Iterator& operator++() noexcept { move(1); return *this; }
	Iterator& operator--() noexcept { move(-1); return *this; }
	Iterator& operator+=(difference_type value) noexcept { move(value); return *this; }
	Iterator& operator-=(difference_type value) noexcept { move(-value); return *this; }
	difference_type operator-(const Iterator& other) const noexcept { return (data - other.data) * direction; }
	#pragma endregion

	friend bool operator== (const Iterator& lhs, const Iterator& rhs) noexcept {
		return operator<=>(lhs, rhs) == 0;
	}

	friend auto operator<=> (const Iterator& lhs, const Iterator& rhs) noexcept {
		if (direction == -1)
			return rhs.operator->() <=> lhs.operator->();
		return  lhs.operator->() <=> rhs.operator->();
	}

private:
	pointer data;
	void move(difference_type distance) noexcept { data += distance * direction; }
};