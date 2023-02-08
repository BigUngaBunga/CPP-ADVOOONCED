#pragma once
#include <string_view>
enum EnclosingModifier
{
	Pointer, Reference, Const, None
};

template<class T>
struct SimplifyType {
	using type = T;
};

template<class T>
using SimplifyType_t = typename SimplifyType<T>::type;

template<class T>
concept IsEnclosingModifier = requires(T, EnclosingModifier modifier) {
	equalsEnclosingModifier<T>(modifier);
};

template<IsEnclosingModifier T>
struct SimplifyType {
	using type = std::remove_pointer_t<T>();
};

template<class T> requires (equalsEnclosingModifier<T>(Reference))
struct SimplifyType {
	using type = std::remove_reference_t<T>();
};

template<class T> requires (equalsEnclosingModifier<T>(Const))
struct SimplifyType {
	using type = std::remove_const_t<T>();
};

template <class T> constexpr auto getTypeName()
{    //Solution based on https://stackoverflow.com/a/20170989
	using namespace std;
	basic_string_view p = __FUNCSIG__; // Signature of the function
	return basic_string_view(p.data() + 23, p.size() - 23 - 7); //Removes everything except the template type
}

template<class T>
constexpr bool equalsEnclosingModifier(EnclosingModifier modifier) {
	std::basic_string_view typeName = getTypeName<T>();
	if (typeName.ends_with("&"))
		return Reference == modifier;
	else if (typeName.ends_with("*"))
		return Pointer == modifier;
	else if (typeName.ends_with("const") || typeName.starts_with("const"))
		return Const == modifier;
	else
		return None == modifier;
}