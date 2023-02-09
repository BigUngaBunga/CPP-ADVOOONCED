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

    new int; cout << "det finns en minnesl�cka i main, avsiktligt!\n s� ni kan se att er minnesl�ckstest fungerar\n";
    std::cin.get();
}

