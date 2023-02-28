#pragma once
#include <type_traits>

template <int X, int Y>
struct Add {
	static const int value = Add<X - 1, Y + 1>::value;
};

//template <int X, int Y>
//using EnableIfXIsZero = 

template <int Y>
struct Add<0, Y> {
	static const int value = Y;
};

int AddFun(int x, int y) {
	if (x == 0)
		return y;
	else
		return AddFun(x - 1, y + 1);
}