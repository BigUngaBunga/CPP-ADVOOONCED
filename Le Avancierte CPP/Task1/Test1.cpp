#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif
#endif  // _DEBUG
#include <crtdbg.h>

#include "Task.h"
#include "TestLevel.h"
#include "Vector.hpp"
#include "VectIterTest.h"

#include <type_traits>
#include <iostream>
#include <cassert>

using std::cout;

void TestBasic();
void TestVector();

void Task::RunTask1() {
#ifdef DBG_NEW
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    TestBasic();
    TestVector();
    TestIter();

    new int; cout << "det finns en minnesläcka i main, avsiktligt!\n så ni kan se att er minnesläckstest fungerar\n";
    std::cin.get();
}

