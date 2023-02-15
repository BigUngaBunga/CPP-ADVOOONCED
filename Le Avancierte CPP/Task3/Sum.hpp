#pragma once

template<class T>
auto Sum(const T& container) {
	using returnType = decltype(*container.begin());
	decltype(std::remove_cvref_t<returnType>())  sum{};
	for (auto i : container)
		sum += i;
	return sum;
}
