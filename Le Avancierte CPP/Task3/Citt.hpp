#pragma once
//TODO se om jag ens behöver denna
#include <type_traits>
template<class T, class CT>
class Iter {
	friend class Iter<T, const T>;

	T* _ptr = nullptr;
public:
	Iter(T* p) :_ptr(p) {}
	Iter() :_ptr(nullptr) {}
	Iter(const Iter& other) : _ptr(other._ptr) {}
	
	template<std::enable_if_t<std::is_const_v<CT>> = true>
	Iter(const Iter<T, T>& other) : _ptr(other._ptr) {};
};