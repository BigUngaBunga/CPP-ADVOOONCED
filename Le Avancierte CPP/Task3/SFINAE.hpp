#pragma once
#include <type_traits>

template<class T>
using EnableIfCopyConstructible = typename std::enable_if_t<std::is_nothrow_copy_constructible_v<T>>;

template<class T>
using EnableIfNotCopyConstructible = typename std::enable_if_t<!std::is_nothrow_copy_constructible_v<T>>;

template<class T, class = EnableIfCopyConstructible<T>>
bool NoThrowCopyConstructible(T t) {
	return true;
}

template<class T, class = EnableIfNotCopyConstructible<T>>
bool NoThrowCopyConstructible(T& t) {
	return false;
}