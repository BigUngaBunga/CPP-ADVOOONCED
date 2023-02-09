#pragma once
#include <string_view>
enum Modifier
{
	Pointer, Reference, Const, Array, None
};

template <class T>
constexpr std::basic_string_view<char> getTypeName()
{    //Solution based on https://stackoverflow.com/a/20170989
	using namespace std;
	basic_string_view p = __FUNCSIG__; // Signature of the function
	size_t start = p.find_last_of('<') + 1;
	return p.substr(start, p.size() - start - 3); //Removes everything except the template type
}


template<class T>
constexpr Modifier getEnclosingModifier() {
	auto typeName = getTypeName<T>();
	if (typeName.ends_with('&'))
		return Reference;
	else if (typeName.ends_with('*'))
		return Pointer;
	else if (typeName.ends_with(']'))
		return Array;
	else if (typeName.ends_with("const") || typeName.starts_with("const"))
		return Const;

	return None;
}

template<class T, Modifier modifier = getEnclosingModifier<T>()>
struct SimplifyType {
	using type = T;
};

template<class T>
using SimplifyType_t = typename SimplifyType<T, getEnclosingModifier<T>()>::type;

template<class T>
struct SimplifyType<T, Pointer> {
	using type = std::remove_pointer_t<T>;
};

template<class T>
struct SimplifyType<T, Reference> {
	using type = std::remove_reference_t<T>;
};

template<class T>
struct SimplifyType<T, Const> {
	using type = std::remove_const_t<T>;
};

template<class T>
struct SimplifyType<T, Array> {
	using type = std::remove_extent_t<T>;
};
