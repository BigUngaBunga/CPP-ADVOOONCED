#ifdef _DEBUG
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#include <crtdbg.h>

#include "TestLevel.h"

#if (defined(LEVEL)  && (LEVEL>80) ||  !defined(LEVEL))   && (defined(VG_BETYG) || defined(G_BETYG))

#include "Vector.hpp"

//Generates all functions declared
//Uncomment this to check that all functions compiles.
//template class Vector<int>; 

#include <iostream>
using std::cout;
using std::endl;
#include <cassert>
#include <type_traits>

template<class X>
bool operator==(const Vector<X>& lhs, const char* rhs) {
    return lhs == Vector<X>(rhs);
}

template <class X>
struct IsConstOrConstRef {
    static const bool value = std::is_const_v<std::remove_reference_t<X>>;
};

template <class X>
bool IsConstOrConstRefFun(X& x) {
    return std::is_const_v<std::remove_reference_t<X>>;
};

template<class X>
void CheckVec(const Vector<X>& v) {
    assert(v.Invariant());
    assert(v.size() <= v.capacity());
    for (size_t i = 0; i < v.capacity(); ++i)
        auto x = v[i];
}

struct C {
    static std::string usedConstr;
    void Test() {}
    C() {
        usedConstr += "DC";
    }
    C(const C& c) {
        usedConstr += "CC";
    }
    C(C&& c) {
        usedConstr += "MC";
    }
    C& operator=(const C& c) {
        usedConstr += "CA";
        return *this;
    }
    C& operator=(C&& c) {
        usedConstr += "MA";
        return *this;
    }
};

std::string C::usedConstr{};

//Test av push_back&&
void TestPushBack() {
    Vector<C> a;
    C::usedConstr = "";
    C c;
    assert(C::usedConstr == "DC");
    a.reserve(4);
    C::usedConstr = "";
    a.push_back(c);
    assert(C::usedConstr == "CA");
    //C::usedConstr = "";
    //a.push_back(std::move(c));
    //assert(C::usedConstr == "MC");
}

void TestRolOp();
void TestCapacity();
void Print(std::string message);

void Print(std::string message) {
    cout << message << "\n";
}

void TestVector() {
    {//Vector<char>(char *)
        Vector<char> Foo("Foo");
        const Vector<char> FooC("Foo");
        Vector<char> Bar("Bar");
        const Vector<char> BarC("Bar");
        CheckVec(Foo); CheckVec(FooC);
        CheckVec(Bar); CheckVec(BarC);
    }
    Print("tested vector<char>");
    {//check empty vectors;
        Vector<char> v1;
        assert(v1.capacity() == 0);
        assert(v1.data() == nullptr);
        Vector<char> v2(v1);
        assert(v2 == "");
    }
    Print("tested empty vectors");

    {//Move constructor
        Vector<char> a("foo");
        Vector<char> b(std::move(a));
        assert(b == "foo" && a.data() == nullptr);
    }
    Print("tested move constructor");

    {//Vector<char>(Copy constructor)
        Vector<char> v1("foo"); assert(v1 == "foo");
        Vector<char> v2(v1); assert(v2 == "foo");
        Vector<char> v3("bar");  assert(v3 == "bar");

        //-	~Vector<char>() Kom ihåg destruktorn!
        delete new Vector<char>("hej");

        //	-	operator =(Sträng sträng)
        v2 = "hej";
        assert((v2 = v3) == v3);
        assert((v2 = v2) == v3);	//self assignment

        //Ej samma buffert
        v2 = "hej";
        v3 = v2;
        v2[0] = 'x';
        assert(v3[0] == 'h');
        v3[1] = 'y'; assert(v2[1] == 'e');
    }
    Print("tested copy constructor");

    {//Kedjat assignment
        Vector<char> v1("foo"), v2("bar"), v3("hej");
        v3 = v2 = v1;
        assert(v3 == "foo");
        assert(v3 == v2);
        assert(v1 == v2);
    }
    Print("tested chained assignments");

#ifdef VG_BETYG
    {//No extra realloc
        Vector<char> v1("foo"), v2("bar"), v3("hej");
        auto xxx = v1.data();
        v1 = v2;
        assert(xxx == v1.data());
    }
    Print("tested no extra reallocation");

#endif

    {// Move assignment
        Vector<char> Foo("Foo");
        const Vector<char> FooC("Foo");
        Vector<char> Bar("Bar");
        const Vector<char> BarC("Bar");
        CheckVec(Foo);
        CheckVec(Bar);
        Foo = std::move(Bar);
        CheckVec(Foo);
        CheckVec(Bar);
        assert(Foo == "Bar");
        assert(Bar == "" || Bar == "Foo");
        assert(Bar == "");  //Vanliga resultatet men Foo är också okej
        //assuming that we now have a nullptr in Bar
        //we test that we can do a push_back;
        Bar.push_back('x');
        assert(Bar.capacity() > 0);
    }
    Print("tested move assignment");


    {//-	operator[](size_t i) som indexerar utan range check.
        Vector<char> vecFoo("Foo");
        const Vector<char> vecCFoo("Foo");
        Vector<char> vecBar("Bar");
        const Vector<char> vecBarC("Bar");

        vecBar = "bar";
        vecBar[-1]; vecBar[1000];	//No error
        assert(vecBar[1] == 'a');
        vecBar[1] = 'y';
        assert(vecBar[1] == 'y');

        //-	operator[](size_t i) const.
        assert(IsConstOrConstRefFun(vecBarC[1]));
        assert(vecBarC[1] == 'a');
    }
    Print("tested operator[] ");

    {//-	at(size_t i) som indexerar med range check
        Vector<char> vecBar("Bar");
        const Vector<char> vecBarC("Bar");

        try {
            vecBar.at(-1);
            assert(false);
        }
        catch (std::out_of_range&) {
            goto exitlabel;
        };
        assert(false);
    exitlabel:
        try {
            vecBar.at(3);
            assert(false);
        }
        catch (std::out_of_range&) {};

        //- at indexerar
        try {
            vecBar.at(vecBar.size());
            assert(false);
        }
        catch (std::out_of_range&) {};

        //-	at(size_t i) 
        vecBar = "bar";
        assert(vecBar.at(1) == 'a');
        vecBar.at(1) = 'y';
        assert(vecBar.at(1) == 'y');
        assert(vecBarC.at(1) == 'a');
        assert(IsConstOrConstRefFun(vecBarC.at(1)));
    }
    Print("tested indexing with at()");


    // data
    {
        Vector<char> vecBar("Bar");
        const Vector<char> vecBarC("Bar");

        assert(vecBar.data() == &vecBar[0]);
        assert(!IsConstOrConstRefFun(*vecBar.data()));
        assert(IsConstOrConstRefFun(*vecBarC.data()));
    }
    Print("tested data");


    //-	push_back(char c) lägger till ett tecken sist.
    {
        Vector<char> vecBar("Bar");
        const Vector<char> vecBarC("Bar");

        vecBar.push_back('a');
        assert(vecBar == "Bara");
    }
    Print("tested push_back");


    // resize
    {
        Vector<char> vecBar("Bar");
        const Vector<char> vecBarC("Bar");
        //size up
        vecBar.shrinkToFit();
        auto buf = vecBar.data();
        vecBar.resize(6);
        assert(vecBar[2] == 'r' && vecBar[3] == 0 && vecBar[4] == 0 && vecBar[5] == 0);
        assert(buf != vecBar.data() && vecBar.capacity() >= 6 && vecBar.size() == 6);
        buf = vecBar.data();
        vecBar.resize(5);
        assert(buf == vecBar.data() && vecBar.capacity() >= 6 && vecBar.size() == 5);
    }
    Print("tested resize");

    {//minitest push_back &&
        Vector<char> vecBar("Bar");
        vecBar.push_back('a');
        assert(vecBar == "Bara");
    }
    Print("tested push_back again");


    //-	operator== 
    //testas överallt!

    {//Swap
        Vector<char> Bar("Bar");
        Vector<char> Foo("Foo");
        swap(Bar, Foo);
        assert(Foo == "Bar" && Bar == "Foo");
        //Test for efficient swap!
        auto fptr = Foo.data();
        auto bptr = Bar.data();
        swap(Foo, Bar);
        assert(fptr == Bar.data() && bptr == Foo.data());
    }
    Print("tested swap");

    TestRolOp();
    Print("tested RolOp ??");

    TestCapacity();
    Print("tested capacity");

    TestPushBack();
    Print("tested push back again... again");


    cout << "TestVector klar\n\n";

}

void TestPushBackReallocation() {
    Vector<char> vec("hej");
    assert(vec.size() <= vec.capacity());
    assert(vec.size() == 3);
    assert(vec == "hej");
    //we try to take 20 empty places: (size+1 < capacity)
    while (vec.size() + 20 >= vec.capacity() && vec.size() < 1000)
        vec.push_back('A' + rand() % 32);
    assert(vec.size() < 1000);	//If this fail it prbably the case that capacity is increased with a constant.

    auto internalBuf = &vec[0];
    auto cap = vec.capacity();
    auto siz = vec.size();
    size_t i;
    for (i = siz + 1; i <= cap; ++i) {
        vec.push_back(char(i) + 'a');
        assert(internalBuf == &vec[0]);
        assert(cap == vec.capacity());
        assert(i == vec.size());
    }
    vec.push_back(char(i));
    assert(internalBuf != &vec[0]);
    assert(cap < vec.capacity());
    assert(i == vec.size());
}

template<class C>
void TestRel(const C& lhs, const C& rhs,
    bool(*trueFun)(const C& lhs, const C& rhs),
    bool(*falseFun)(const C& lhs, const C& rhs)) {
    assert(trueFun(lhs, rhs));
    assert(falseFun(lhs, rhs));
}

#define RelTest(a, b, c, d)  a c b && !(a d b)

void TestRolOp() {
    Vector<char> vecAbcdef("Abcdef");
    Vector<char> vecBbcdef("Bbcdef");
    Vector<char> vecAbcdeF("AbcdeF");
    Vector<char> vecAbcdefg("Abcdefg");

    { bool f = RelTest(vecAbcdef, vecAbcdef, == , != ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecAbcdeF, != , == ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecBbcdef, != , == ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecAbcdefg, != , == ); assert(f); }
    { bool f = RelTest(vecAbcdefg, vecAbcdef, != , == ); assert(f); }

    { bool f = RelTest(vecAbcdef, vecAbcdef, == , < ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecAbcdeF, > , < ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecBbcdef, <, >); assert(f); }
    { bool f = RelTest(vecAbcdef, vecAbcdefg, <, >); assert(f); }

    { bool f = RelTest(vecAbcdef, vecAbcdef, <= , != ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecAbcdeF, >= , <= ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecBbcdef, <= , >= ); assert(f); }
    { bool f = RelTest(vecAbcdef, vecAbcdefg, <= , >= ); assert(f); }

    Vector<char> vecA("A");
    Vector<char> vecAxxx("Axxx");
    Vector<char> vecB("B");

    bool biggusMaximus = vecAxxx > vecB;
    bool biggusMinimus = vecAxxx < vecB;

    { bool f = RelTest(vecA, vecAxxx, < , >= ); assert(f); }
    { bool f = RelTest(vecAxxx, vecA, > , <= ); assert(f); }
    { bool f = RelTest(vecB, vecAxxx, > , <= ); assert(f); }
    { bool f = RelTest(vecAxxx, vecB, < , >= ); assert(f); }

}

void TestCapacity() {
    Vector<char> vecBar("Bar");

    //-	size(), capacity() and reloccation test;
    TestPushBackReallocation();


    //	reserve()
    auto internalBuf = &vecBar[0];
    auto cap = vecBar.capacity();
    auto siz = vecBar.size();

    vecBar.reserve(cap);
    assert(internalBuf == &vecBar[0]);
    assert(cap == vecBar.capacity());
    assert(siz == vecBar.size());

    vecBar.reserve(cap + 1);
    assert(internalBuf != &vecBar[0]);
    assert(cap < vecBar.capacity());
    assert(siz == vecBar.size());

    {
        auto internalBuf = &vecBar[0];
        auto cap = vecBar.capacity();
        auto siz = vecBar.size();

        vecBar.reserve(cap - 1);
        assert(internalBuf == &vecBar[0]);
        assert(cap == vecBar.capacity());
        assert(siz == vecBar.size());
    }

    // shrink_to_fit
    vecBar = "hej";
    vecBar.reserve(10);
    internalBuf = &vecBar[0];
    cap = vecBar.capacity();
    siz = vecBar.size();

    vecBar.shrinkToFit();
    assert(internalBuf != &vecBar[0]);
    assert(vecBar.capacity() == vecBar.size());
    assert(vecBar == "hej");

}

//Test of typedefs!
using std::is_same;
//static_assert(is_same<int, Vector<int>::value_type>::value, "typdef missing");
//static_assert(is_same<size_t, Vector<int>::size_type>::value, "typdef missing");
//static_assert(is_same<ptrdiff_t, Vector<int>::difference_type>::value, "typdef missing");
//static_assert(is_same<int&, Vector<int>::reference>::value, "typdef missing");
//static_assert(is_same<const int&, Vector<int>::const_reference>::value, "typdef missing");
//static_assert(is_same<int*, Vector<int>::pointer>::value, "typdef missing");
//static_assert(is_same<const int*, Vector<int>::const_pointer>::value, "typdef missing");
//static_assert(is_same<Vector<int>::iterator, Vector<int>::iterator>::value, "typdef missing");
//static_assert(is_same<Vector<int>::const_iterator, Vector<int>::const_iterator>::value, "typdef missing");
//static_assert(is_same<Vector<int>::reverse_iterator, Vector<int>::reverse_iterator>::value, "typdef missing");
//static_assert(is_same<Vector<int>::const_reverse_iterator, Vector<int>::const_reverse_iterator>::value, "typdef missing");

#else
void TestVector() {}
#endif
