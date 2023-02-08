#pragma once

template<class T>
struct SimplifyType {
	using type = T;
};

template<class T>
using SimplifyType_t = typename SimplifyType<T>::type;

