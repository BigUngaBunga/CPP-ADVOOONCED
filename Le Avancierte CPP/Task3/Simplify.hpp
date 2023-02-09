#pragma once
#include <string_view>
enum Modifier
{
	Pointer, Reference, Const, Array, None
};

#pragma region Helper functions
template <class T>
constexpr std::basic_string_view<char> getTypeName()
{    //Solution based on https://stackoverflow.com/a/20170989
	using namespace std;
	basic_string_view p = __FUNCSIG__; // Signature of the function
	size_t start = p.find_last_of('<') + 1;
	size_t length = p.find_last_of('>') - start;
	return basic_string_view(p.data() + start, length); //Removes everything except the template type
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

#pragma endregion

#pragma region Simplify
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
#pragma endregion

#pragma region Base type
template<class T, Modifier modifier = getEnclosingModifier<T>()>
struct BaseType {
	using type = T;
};

template<class T>
using BaseType_t = typename BaseType<T, getEnclosingModifier<T>()>::type;

template<class T>
struct BaseType<T, Pointer> {

	using type = BaseType_t<std::remove_pointer_t<T>>;
};

template<class T>
struct BaseType<T, Reference> {
	using type = BaseType_t<std::remove_reference_t<T>>;
};

template<class T>
struct BaseType<T, Const> {
	using type = BaseType_t<std::remove_const_t<T>>;
};

template<class T>
struct BaseType<T, Array> {
	using type = BaseType_t<std::remove_extent_t<T>>;
};
#pragma endregion