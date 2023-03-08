#pragma once
//Based on https://github.com/lakshayg/compile_time

template <class T>
constexpr bool is_close(T x, T y) {
	T abs = x - y;
	if (abs < 0)
		abs *= -1;
	return abs <= 1e-7;
}

constexpr double Sqrt(double x, double guess) {
	return is_close(guess * guess, x) ? guess
		: Sqrt(x, (guess + x / guess) / static_cast<double>(2));
}

template <class T, size_t base>
constexpr T log(T t) {
	
}

template<class Float>
constexpr auto Ceil(Float x) {
	auto value = (int)x;
	return value == x ? value : value +1;
}

template<class Value>
constexpr Value Pow(Value base, int exponent) {
	if (exponent == 0 || base == 0)
		return 0;
	if (exponent == 1)
		return base;
	return base * Pow(base, exponent -1);
}