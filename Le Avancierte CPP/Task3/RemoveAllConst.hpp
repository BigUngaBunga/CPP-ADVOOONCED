#pragma once
template <class T>
constexpr int constOcurrances()
{
	using namespace std;
	basic_string_view p = getTypeName<T>();
	size_t start = 0;
	int ocurrances = 0;
	while (true)
	{
		start = p.find("const", start);
		if (start < 0 || start > p.size())
			break;
		++ocurrances;
		++start;
	}
	return ocurrances;
}

//template<class T, int numberOfConst = constOcurrances<T>()>
//struct RAC {
//	using type = RAC<std::remove_const_t<T>, numberOfConst - 1>::type;
//};
//
//template<class T>
//concept IsPointer = requires (T t) {
//	std::is_pointer_v<T>;
//};
//
//template<class T>
//using RAC_t = typename RAC<T>::type;
//
//
//template<class T> requires IsPointer<T>
//struct RAC<T*> {
//	using type = RAC<std::remove_const_t<T>, constOcurrances<T>()>::type;
//};
//
//template<class T>
//struct RAC<T, 0> {
//	using type = T;
//};

template<class T, bool isConst = (constOcurrances<T>() > 0)>
struct RAC {
	using type = RAC<std::remove_const_t<T>>::type;
};

template<class T>
using RAC_t = typename RAC<T>::type;

template<class T>
concept IsPointer = requires (T t) {
	std::is_pointer_v<T>;
};

template<class T> requires IsPointer<T>
struct RAC<T*, true> {
	using type = RAC<std::add_pointer_t<std::remove_const_t<std::remove_pointer_t<T>>>>::type;
};

template<class T> requires IsPointer<T>
struct RAC<T, false> {
	using type = T;
};