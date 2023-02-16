#pragma once
#include <algorithm>

#pragma region Helper Functions
template<class T, class U>
constexpr Modifier getNextModifier() {
	char modifier = getNonConstInName<T, U>();
	if (modifier == '&')
		return Reference;
	else if (modifier == '*')
		return Pointer;
	else if (modifier =='[')
		return Array;
	return None;
}

constexpr size_t getFirstModifierPosition(std::basic_string_view<char> typeName) {
	size_t position = std::min<size_t>(typeName.find_first_of('*'), typeName.find_first_of('&'));
	return std::min<size_t>(typeName.find_first_of('['), position);
}

template<class T, class U>
constexpr char getNonConstInName(std::basic_string_view<char> tName = getTypeName<T>(),
	std::basic_string_view<char> uName = getTypeName<U>()) {

	if (!uName.empty())
	{	
		uName.remove_prefix(std::min<size_t>(uName.size(), getFirstModifierPosition(uName)));
		if (!uName.empty())
		{
			tName.remove_prefix(tName.find_first_of(uName.front()) + 1);
			uName.remove_prefix(1);
		}
		return getNonConstInName<T, U>(tName, uName);
	}
	else if (!tName.empty()) {
		if ((tName.front() == '&' || tName.front() == '*' || tName.front() == '['))
			return tName.front();
		tName.remove_prefix(1);
		return getNonConstInName<T, U>(tName, uName);
	}
	return '+';
}
#pragma endregion


#pragma region Previous solution
//template<class T, class U>
//constexpr char getNextNonConstModifier() {
//	constexpr std::basic_string_view<char> tName = getTypeName<T>();
//	constexpr std::basic_string_view<char> uName = getTypeName<U>();
//	size_t uCounter = 0;
//	for (size_t i = 0; i < tName.size(); i++)
//	{
//		char tFront = tName.at(i);
//		if (uCounter < uName.size() && tFront == uName.at(uCounter))
//			++uCounter;
//		else if (uCounter >= uName.size() && (tFront == '&' || tFront == '*' || tFront == '['))
//			return tFront;
//	}
//
//	return '+';
//}
#pragma endregion



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