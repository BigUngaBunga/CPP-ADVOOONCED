#include <type_traits>

void TestPtr() {
	using iter = char*;
	using const_iter = const char*;

	const_iter cit1 = nullptr;
	iter it1 = nullptr;

	//Copy constr
	iter it2(it1);
	const_iter cit2(cit1);

	//cross over
	const_iter cit3(it1);
	//iter it3(cit1); //Not permitted!
}

template<class T, class CT>
class Iter {
	friend class Iter<T, const T>;

	T* _ptr = nullptr;
public:
	Iter(T* p) :_ptr(p) {}
	Iter() :_ptr(nullptr) {}
	Iter(const Iter& other) : _ptr(other._ptr) {}
	Iter(const Iter<T, T>& other) : _ptr(other._ptr) {}
};

void Test() {
	using iter = Iter<char, char>;
	using const_iter = Iter<char, const char>;

	const_iter cit1 = nullptr;
	iter it1 = nullptr;

	//Copy constr
	const_iter cit2(cit1);
	iter it2(it1);

	//cross over
	const_iter cit3(it1);
	//iter it3(cit1); //Not permitted!
}