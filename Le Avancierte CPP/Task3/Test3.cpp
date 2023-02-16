#include <cassert>
#include <typeinfo>
#include "Task.h"
#include "DebugPrinting.h"
#define LEVEL 9
#define VG true

#if LEVEL>=1
#include "Add.hpp"
void TestAdd() {
    assert(AddFun(3, 5) == 8);
    int x = Add<3, 5>::value;
    assert((Add<3, 5>::value) == 8);
    int result = Add<11, 27>::value;
    std::cout << "Adding 11 and 27 results in " << result;
    Println("");

}
#endif

#if LEVEL>=2
#include "Ackermann.hpp"
void TestAckermann() {
    int x = Ackermann<0, 1>::value; //should be 2
    x = Ackermann<1, 0>::value; //should be 2
    assert((Ackermann<0, 1>::value == 2));
    assert((Ackermann<1, 0>::value == 2));
    assert((Ackermann<3, 4>::value == 125));
    assert((Ackermann<4, 0>::value == 13));
    //assert((Ackermann<4, 1>::value == 65533));
    std::cout << "Ackermann of 3 and 5 results in " << Ackermann<3, 5>::value;
    Println("");
}
#endif

#include <type_traits>
#define AssertSame(X,Y) static_assert((std::is_same_v<X, Y>));

#if LEVEL>=3
#include "Simplify.hpp"

void TestSimplify() {
    constexpr auto value = getEnclosingModifier<int** const**>();
    constexpr auto value2 = getEnclosingModifier<int** const&>();
    constexpr auto value3 = getEnclosingModifier<int** const>();
    constexpr auto string = getTypeName<int* const* const>();
    constexpr auto string2 = getTypeName<std::string** const&>();
    constexpr auto string3 = getTypeName<float&>();
    
    using type = int** const**;
    Modifier modifier = getEnclosingModifier<type>();
    SimplifyType_t<type> a = 0;
    SimplifyType<type, getEnclosingModifier<type>()>::type b = 0;
    AssertSame(int, SimplifyType_t<int>);
    AssertSame(int, SimplifyType_t<const int>);
    AssertSame(int, SimplifyType_t<int const>);
    AssertSame(int, SimplifyType_t<int*>);
    AssertSame(int, SimplifyType_t<int*>);
    AssertSame(int, SimplifyType_t<int&>);
    AssertSame(int, SimplifyType_t<int[]>);
    AssertSame(const int, SimplifyType_t<const int*>);
    AssertSame(int** const*, SimplifyType_t<int** const**>);
    AssertSame(const int*, SimplifyType_t<const int* const>);
    AssertSame(const int, SimplifyType_t<const int&>);
}
#endif

#if LEVEL>=4
#include "Simplify.hpp"

void TestBase() {
    AssertSame(int, int);
    AssertSame(int, BaseType_t<int>);
    AssertSame(int, BaseType_t<const int>);
    AssertSame(int, BaseType_t<int*>);
    AssertSame(int, BaseType_t<int&>);
    AssertSame(int, BaseType_t<int[]>);
    AssertSame(int, BaseType_t<int***&>);
    AssertSame(int, BaseType_t<const int** const**&>);
    AssertSame(BaseType_t<const int*&>, BaseType_t<BaseType_t<const int>**>);

#if VG
    AssertSame(int, BaseType_t<int[6]>);
#if LEVEL>=5
    AssertSame(int, BaseType_t<const int[6]>);
#endif
#endif
}

#endif

#if LEVEL>=6 && VG
#include "RemoveAllConst.hpp"

void TestRemoveAllConst() {
    constexpr auto nextChar = getNextModifier<int const*&, int>();
    constexpr auto negstChar = getNextModifier<int const*&, int*>();
    constexpr auto neckstChar = getNextModifier<int const *&, int*&>();
#pragma warning(disable:4091)
    RAC_t<const int>;
    RAC_t<int const>;
    RAC_t<const int *const>;
    RAC_t<double const &>;
    RAC_t<const int*& >;
    RAC_t<const float* const* const>;
    RAC_t<const char** const** >;
    RAC_t<const int* const [] >;
#pragma warning(default:4091)

    AssertSame(int, RAC_t<int>);
    AssertSame(int, RAC_t<const int>);
    AssertSame(int*, RAC_t<int*>);
    AssertSame(int***, RAC_t<int const* const* const* const>);
}
#endif

#if LEVEL>=7
#include "Sum.hpp"
#include <vector>
void TestSum() {
    std::vector<int> v = { 1,2,3,-9 };
    int sum = Sum(v);
    assert(sum == -3);

    std::vector<float> v2 = { 1.3f,2.5f,3.2f,-6.0f };
    auto sum2 = Sum(v2);
    assert(sum2 == 1.0f);
}
#endif

#include <string>
using std::string;

#if LEVEL>=8
#include "SFINAE.hpp"
void TestSFINAE() {
    int i(3);
    constexpr bool noThrow = std::is_nothrow_copy_constructible_v<int>;
    assert(NoThrowCopyConstructible(i));
    auto str = string("hej");
    constexpr bool noThrow2 = std::is_nothrow_copy_constructible_v<string>;
    assert(!NoThrowCopyConstructible(str));
}
#endif

#if LEVEL>=9 && VG
#include "Citt.hpp"
void TestIterator() {
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
#endif


#include <iostream>

void Task::RunTask3() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#if LEVEL>=1
    TestAdd();
    FinishedTest("add function");
#endif
#if LEVEL>=2
    TestAckermann();
    FinishedTest("Ackermann");
#endif
#if LEVEL>=3
    TestSimplify();
    FinishedTest("Simplify");
#endif
#if LEVEL>=4
    TestBase();
    FinishedTest("Base type");
#endif
#if LEVEL>=6 && VG
    TestRemoveAllConst();
    FinishedTest("Remove all const");
#endif
#if LEVEL>=7
    TestSum();
    FinishedTest("Sum");
#endif
#if LEVEL>=8
    TestSFINAE();
    FinishedTest("SFINAE");
#endif
#if LEVEL>=9 && VG
    TestIterator();
    FinishedTest("Iterators");
    Println("Finished all tasks");
#endif
    Println("There should only be one 4 byte memory leak");
    new int;
}