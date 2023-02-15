#pragma once
template<class T, class U>
constexpr Modifier getNextModifier() {
	constexpr char modifier = getNextNonConstModifier<T, U>();
	if (modifier == '&')
		return Reference;
	else if (modifier == '*')
		return Pointer;
	else if (modifier =='[')
		return Array;
	return None;
}

template<class T, class U>
constexpr char getNextNonConstModifier() {
	std::basic_string_view<char> tName = getTypeName<T>();
	std::basic_string_view<char> uName = getTypeName<U>();
	
	size_t uCounter = 0;
	for (size_t i = 0; i < tName.size(); i++)
	{
		char tFront = tName.at(i);
		if (uCounter < uName.size() && tFront == uName.at(uCounter))
			++uCounter;
		else if (uCounter >= uName.size() && (tFront == '&' || tFront == '*' || tFront == '['))
			return tFront;
	}

	return '+';;
}

template<class T, class U = BaseType_t<T>, Modifier nextModifier = getNextModifier<T, U>()>
struct RAC {
	using type = U;
};

template<class T>
using RAC_t = typename RAC<T>::type;

template<class T, class U>
struct RAC<T, U, Reference> {
	using type = RAC<T, std::add_lvalue_reference_t<U>>::type;
};

template<class T, class U>
struct RAC<T, U, Pointer> {
	using type = RAC<T, std::add_pointer_t<U>>::type;
};

template<class T, class U>
struct RAC<T, U, Array> {
	using type = RAC<T, U[]>::type;
};