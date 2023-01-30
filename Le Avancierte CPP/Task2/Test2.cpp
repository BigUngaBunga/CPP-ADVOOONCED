#include "TestLevel.h"
#include "Task.h"

#include <iostream>
using std::cout;


void TestBasicPointer();
void TestG();
void TestVGWeak();
void TestVG();

void Task::RunTask2() {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    std::locale::global(std::locale("swedish"));

    TestBasicPointer();
#ifdef G_BETYG
    TestG();
    cout << "Test for G finished\n";
#endif
#ifdef  VG_BETYG
    TestVG();
    TestVGWeak();
    cout << "Test for VG finished\n";
#endif //  VG_BETYG
    new int; cout << "you should get a memory leak (new int)\n";
    //    std::cin.get();
}

