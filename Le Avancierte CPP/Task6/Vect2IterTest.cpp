#include "TestLevel.h"

#if (defined(LEVEL)  && (LEVEL>80) ||  !defined(LEVEL))   && (defined(VG_BETYG) || defined(G_BETYG))

#include "Vector2.hpp"

#include <iostream>
#include<random>
#include <cassert>
#include "DebugPrinting.h"
#include "Dhelper2.h"
using std::cout;
using std::endl;

#pragma region help jox

using IT = Vector2<char>::iterator;
using CIT = Vector2<char>::const_iterator;
using RIT = Vector2<char>::reverse_iterator;
using CRIT = Vector2<char>::const_reverse_iterator;

#if DEL == 2
#endif DEL

template<class T>
struct IsSameFun {
    template<class V>
    static void e(V v) {
        static_assert(std::is_same<T, V>::value, "Not right return type, look where IsSameFun is used!");
    }
};

template<class T, class U>
struct IsSameS {
    static_assert(std::is_same_v<T, U>, "Not right type");
    static const bool value = true;
};

template< class T, class U >
constexpr bool IsSameCheck = IsSameS<T, U>::value;

#pragma endregion help jox

#pragma region +, +, += och --, -
//also test most begin and end versions

void TestIterPart_2() {
    Vector2<char> s1("foxbar");
    Vector2<char> r1;
    for (auto i = s1.begin(); i != s1.end(); i++)
        r1.push_back(*i);
    assert(r1 == "foxbar");
    auto it = r1.begin();
    assert(*it == 'f');
    assert(*(it++) == 'f' && *it == 'o');
    ++it;
    assert(*++it == 'b');
    assert(*(it + 1) == 'a');
    assert(it[2] == 'r');
    assert(*(it += 1) == 'a' && *it == 'a');

    r1 = "rabxof";
    it = r1.end();
    --it;
    assert(*it == 'f');
    assert(*(it--) == 'f' && *it == 'o');
    --it;
    assert(*--it == 'b');
    assert(*(it - 1) == 'a');
    assert(it[-2] == 'r');
    //assert(*(it -= 1) == 'a' && *it == 'a');

    assert(((it + 3) - it) == +3);
    assert(((it - 3) - it) == -3);
}

void TestIterPartC_2() {
    Vector2<char> s1("foxbar");
    Vector2<char> r1;
    for (auto i = s1.cbegin(); i != s1.cend(); i++)
        r1.push_back(*i);
    assert(r1 == "foxbar");
    auto it = r1.cbegin();
    assert(*it == 'f');
    assert(*(it++) == 'f' && *it == 'o');
    ++it;
    assert(*++it == 'b');
    assert(*(it + 1) == 'a');
    assert(it[2] == 'r');
    assert(*(it += 1) == 'a' && *it == 'a');

    r1 = "rabxof";
    it = r1.cend();
    --it;
    assert(*it == 'f');
    assert(*(it--) == 'f' && *it == 'o');
    --it;
    assert(*--it == 'b');
    assert(*(it - 1) == 'a');
    assert(it[-2] == 'r');
    //assert(*(it -= 1) == 'a' && *it == 'a');

    assert(((it + 3) - it) == +3);
    assert(((it - 3) - it) == -3);
}

void TestIterPartR_2() {
    Vector2<char> s1("foxbar");
    Vector2<char> r1;
    for (auto i = s1.rbegin(); i != s1.rend(); i++)
        r1.push_back(*i);
    assert(r1 == "rabxof");
    auto it = r1.rbegin();
    assert(*it == 'f');
    assert(*(it++) == 'f' && *it == 'o');
    ++it;
    assert(*++it == 'b');
    assert(*(it + 1) == 'a');
    assert(it[+2] == 'r');

    r1 = s1;
    it = r1.rend();
    --it;
    assert(*it == 'f');
    assert(*(it--) == 'f' && *it == 'o');
    --it;
    assert(*--it == 'b');
    assert(*(it - 1) == 'a');
    assert(it[-2] == 'r');
    //assert(*(it -= 1) == 'a' && *it == 'a');

    assert(((it + 3) - it) == +3);
    assert(((it - 3) - it) == -3);
}

void TestIterPartCR_2() {
    Vector2<char> s1("foxbar");
    Vector2<char> r1;
    for (auto i = s1.crbegin(); i != s1.crend(); i++)
        r1.push_back(*i);
    assert(r1 == "rabxof");
    auto it = r1.crbegin();
    assert(*it == 'f');
    assert(*(it++) == 'f' && *it == 'o');
    ++it;
    assert(*++it == 'b');
    assert(*(it + 1) == 'a');
    assert(it[+2] == 'r');

    r1 = s1;
    it = r1.crend();
    --it;
    assert(*it == 'f');
    assert(*(it--) == 'f' && *it == 'o');
    --it;
    assert(*--it == 'b');
    assert(*(it - 1) == 'a');
    assert(it[-2] == 'r');
    //assert(*(it -= 1) == 'a' && *it == 'a');

    assert(((it + 3) - it) == +3);
    assert(((it - 3) - it) == -3);
}

#pragma endregion +, +, += och --, -

#pragma region relop
void TestRelOp2() {
    Vector2<char> Foo("Foo");
    {
        auto b = Foo.begin();
        auto a = b++;
        auto c = b;
        assert(b == c && !(b != c));
        assert(a != c && !(a == c));
        assert(a < c && !(a >= c));
        assert(c > a && !(c <= a));
    }
    {
        auto b = Foo.cbegin();
        auto a = b++;
        auto c = b;
        assert(b == c && !(b != c));
        assert(a != c && !(a == c));
        assert(a < c && !(a >= c));
        assert(c > a && !(c <= a));
    }
    {
        auto b = Foo.rbegin();
        auto a = b++;
        auto c = b;
        assert(b == c && !(b != c));
        assert(a != c && !(a == c));
        assert(a < c && !(a >= c));
        assert(c > a && !(c <= a));
    }
    {
        auto b = Foo.crbegin();
        auto a = b++;
        auto c = b;
        assert(b == c && !(b != c));
        assert(a != c && !(a == c));
        assert(a < c && !(a >= c));
        assert(c > a && !(c <= a));
    }
}

#pragma endregion relop

#pragma region constr och assignment
void TestConstrAssignm2() {
    Vector2<char> v("foxbar");
    auto it = v.begin();
    auto cit = v.cbegin();
    auto rit = v.rbegin();
    auto crit = v.crbegin();

    //Borde g�ra om assignment som constructor nedan.
    it = it;
    cit = cit;
    //cit = it; //kr�vs ej i Del1
    //it = cit; //Ska inte g�
    rit = rit;
    crit = crit;
    //crit = rit; //Kr�vs ej i Del1
    //rit = crit; //Ska inte g�

    //Constructor;
    Vector2<char>::iterator it2 = it;
    assert(&*it2 == &*it);
    //Vector<char>::const_iterator cit2 = it; //Kr�vs ej i Del1
    //Vector<char>::iterator it3 = cit;  //Ska inte g�
    Vector2<char>::const_iterator cit3 = cit;
    assert(&*cit3 == &*cit);

    Vector2<char>::reverse_iterator rit2 = rit;
    assert(&*rit2 == &*rit);
    //Vector<char>::const_reverse_iterator crit2 = rit; //Kr�vs ej i Del1
    //Vector<char>::reverse_iterator rit3 = crit;  //Ska inte g�
    Vector2<char>::const_reverse_iterator crit3 = crit;
    assert(&*crit3 == &*crit);
}
#pragma endregion constr och assignment

#pragma region * -> []
void TestAccess2() {
    Vector2<char> Bar("BarBar");
    const Vector2<char> BarC("BarBar");
    {
        auto it = Bar.begin();
        assert(*it == 'B');
        IsSameCheck<char&, decltype(*it)>;
        auto cit = BarC.begin();
        assert(*cit == 'B');
        IsSameCheck<const char&, decltype(*cit)>;
        auto cit2 = Bar.cbegin();
        assert(*cit2 == 'B');
        IsSameCheck<const char&, decltype(*cit2)>;
    }
    {
        auto it = Bar.rbegin();
        assert(*it == 'r');
        IsSameCheck<char&, decltype(*it)>;
        auto cit = BarC.rbegin();
        assert(*cit == 'r');
        IsSameCheck<const char&, decltype(*cit)>;
        auto cit2 = Bar.crbegin();
        assert(*cit2 == 'r');
        IsSameCheck<const char&, decltype(*cit2)>;
    }
    Vector2<Dhelper2> Dcont;
    const Vector2<Dhelper2> DcontC;
    Dcont.push_back(Dhelper2(17));
    {
        auto i = Dcont.begin()->Test();
        auto ci = DcontC.begin()->Test();
        auto ci2 = Dcont.cbegin()->Test();
        assert(i == 1);
        assert(ci == 2);
        assert(ci2 == 2);
    }
    {
        auto i = Dcont.rbegin()->Test();
        auto ci = DcontC.rbegin()->Test();
        auto ci2 = Dcont.crbegin()->Test();
        assert(i == 1);
        assert(ci == 2);
        assert(ci2 == 2);
    }
}
#pragma endregion * ->

#pragma region Test Iterator in Algorithm
#include <random>
template <class IT>
void random_shuffle(IT begin, IT end) {
    shuffle(begin, end, std::default_random_engine());
}

void TestIterInAlg2() {
    static const int N = 26;
    Vector2<char> v;
    v.reserve(N);
    for (int i = 0; i < N; ++i) {
        v.push_back('a' + i);
    }
    auto b = std::begin(v);
    auto e = std::end(v);

    random_shuffle(b, e);

    cout << v << endl;

    std::sort(b, e);
    //    std::stable_sort(b, e);  //Maybe less op needed

    cout << v << endl;
}

void TestRevIterInAlg2() {
    static const int N = 26;
    Vector2<char> v;
    v.reserve(N);
    for (int i = 0; i < N; ++i) {
        v.push_back('a' + i);
    }
    auto b = std::rbegin(v);
    auto e = std::rend(v);

    random_shuffle(b, e);

    cout << v << endl;

    std::sort(b, e);
    //    std::stable_sort(b, e);  //Maybe less op needed

    cout << v << endl;
}
#pragma endregion Test Iterator in Algorithm


void TestIter2() {

    //Iteratorerna ska kunna g�ra:
    //-	*it, ++it, it++, (it+i), it[i], == och !=
    //also test most begin and end versions
    TestIterPart_2();
    FinishedTest("Regular iterator");
    TestIterPartC_2();
    FinishedTest("Constant iterator");

    TestIterPartR_2();
    FinishedTest("Reverse iterator");

    TestIterPartCR_2();
    FinishedTest("Constant reverse iterator");


    //Constructorer och assignment
    TestConstrAssignm2();
    FinishedTest("Constructors and assignment");


    //Test * ->
    TestAccess2();
    FinishedTest("Access");


    //Test ==, !=, <, ...
    TestRelOp2();
    FinishedTest("comparisons");


    //typdefs f�r iterator, const_iterator,  reverse_iterator och const_revers_iterator
    //   tested with static_assert below

    //Prova att sortera
    TestIterInAlg2();
    FinishedTest("In algorithms");

    TestRevIterInAlg2();
    FinishedTest("In algorithms 2 electric boogaloo");


    cout << "\nTest f�r (V�l-)Godk�nt Iterator klar\n";

}

#pragma region Test of typedefs!
using std::is_same;

template<class Iter>
struct CheckTypedefs {
    static_assert(is_same<int, typename Iter::value_type>::value, "value_type is missing");
    static_assert(is_same<ptrdiff_t, typename Iter::difference_type>::value, "typdef missing");
    static_assert(is_same<int&, typename Iter::reference>::value, "typdef missing, int&");
    static_assert(is_same<int*, typename Iter::pointer>::value, "typdef missing, int*");
    static_assert(is_same<std::random_access_iterator_tag, typename Iter::iterator_category>::value, "should be random_access_iterator_tag");
};

template<class Iter>
struct CheckTypedefsConst {
    static_assert(is_same<int, typename Iter::value_type>::value ||
        is_same<const int, typename Iter::value_type>::value, "value_type should be without const");
    static_assert(is_same<ptrdiff_t, typename Iter::difference_type>::value, "typdef missing");
    static_assert(is_same<const int&, typename Iter::reference>::value, "should be with const");
    static_assert(is_same<const int*, typename Iter::pointer>::value, "should be with const");
    static_assert(is_same<std::random_access_iterator_tag, typename Iter::iterator_category>::value, "should be random_access_iterator_tag");
};

template struct CheckTypedefs<std::vector<int>::iterator>;
template struct CheckTypedefsConst<std::vector<int>::const_iterator>;

template struct CheckTypedefs<Vector2<int>::iterator>;
template struct CheckTypedefsConst<Vector2<int>::const_iterator>;
template struct CheckTypedefs<Vector2<int>::reverse_iterator>;
template struct CheckTypedefsConst<Vector2<int>::const_reverse_iterator>;

#pragma endregion Test of typedefs!

#pragma region Test Begin/End types
void TestTypeBeginEnd2() {
    Vector2<char> Foo("Foo");
    const Vector2<char> FooC("Foo");
    Vector2<char> Bar("Bar");
    const Vector2<char> BarC("Bar");
    IsSameFun<IT>::e(Foo.begin());
    IsSameFun<CIT>::e(FooC.begin());
    IsSameFun<CIT>::e(Foo.cbegin());
    IsSameFun<RIT>::e(Foo.rbegin());
    IsSameFun<CRIT>::e(FooC.rbegin());
    IsSameFun<CRIT>::e(Foo.crbegin());

    IsSameFun<IT>::e(Foo.end());
    IsSameFun<CIT>::e(FooC.end());
    IsSameFun<CIT>::e(Foo.cend());
    IsSameFun<RIT>::e(Foo.rend());
    IsSameFun<CRIT>::e(FooC.rend());
    IsSameFun<CRIT>::e(Foo.crend());
}
#pragma endregion Test Begin/End types

#else
void TestIter2() {}
#endif
