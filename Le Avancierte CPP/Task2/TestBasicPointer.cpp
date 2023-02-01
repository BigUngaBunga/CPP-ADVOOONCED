#include "TestLevel.h"
#include "SharedPtr.hpp"
#include "WeakPtr.hpp"
#include "DebugPrinting.h"
#include <iostream>
using std::cout;
using std::endl;

#include <cassert>

//size_t ptrOffset, cntOffset;
//
//bool InvSharedPtr() {
//    if (sizeof(SharedPtr<char>) / sizeof(int*) > 2)
//        cout << "Your SharedPtr is bigger than two pointers???";
//

void Error(const char* msg) {
    cout << msg << std::endl;
    __debugbreak();
}

struct TestChar {
    int _state = 1;
    char _c;
    ~TestChar() {
        if (_state != 1)
            if (_state == -1)
                Error("Troligen har försöker du deleta ett objekt för andra gången!");
            else if (_state == 0)
                Error("Troligen försöker du deleta ett objekt som du inte har konstruerat!");
            else
                Error("Kanske så försöker du deleta ett objekt som du inte har konstruerat eller så är något annat fel");
        _state = -1;
    }
    TestChar(char c = 'x') {
        _c = c;
    }
    TestChar& operator=(const TestChar&) = delete;
};

#define TestShared SharedPtr<TestChar>
#define TestWeak WeakPtr<TestChar>


void TestBasicPointer() {
#if LEVEL>=1
    {
        TestChar* tp = new TestChar('a');
        TestShared p1(tp);
        using X = decltype(&TestShared::get);
        using Y = std::invoke_result_t<X, TestShared>;
        static_assert(std::is_same_v < Y, TestChar*>, "");
        assert(p1.get() == tp);
        assert(p1.Invariant());
        assert(p1.use_count() == 1);
        TestShared p0;
        assert(p0.get() == nullptr);
        assert(p0.Invariant());
        assert(p0.use_count() == 0);
        FinishedTest(1);
    }
#endif
#if LEVEL>=2
    {
        TestShared p0(nullptr);
        assert(p0.get() == nullptr);
        assert(p0.Invariant());
        assert(p0.use_count() == 0);
    }
    {
        TestShared p1(new TestChar());
        TestShared p2(p1);
        assert(p1.use_count() == p2.use_count() && p1.use_count() == 2);
        assert(p1.get() == p2.get());
        TestShared p3(std::move(p1));
        assert(p1.use_count() == 0 && p2.use_count() == 2);
        assert(p3.use_count() == 2 && p1.get() == nullptr);
        FinishedTest(2);
    }
#endif
#if LEVEL>=3
    {
        TestShared p1(new TestChar('a'));
        assert((*p1)._c == 'a');
        assert(p1->_c == 'a');
        assert(p1);
        TestShared p0;
        assert(!p0);
        FinishedTest(3);
    }
#endif
#if LEVEL>=4
    {
        TestShared p1(new TestChar('a'));
        TestShared p2(new TestChar('b'));
        p1 = p2;
        assert(p1.get() == p2.get());
        p1 = p1;
        assert(p1.get() == p2.get());
        assert(p1.use_count() == 2);
        TestShared p3(new TestChar('c'));
        p1 = std::move(p3);
        assert(!p3);
        assert(p1->_c == 'c');
        p1 = std::move(p1);
        assert(p1);
        assert(p1->_c == 'c');
        p1.reset();
        assert(p1.use_count() == 0 && !p1);
        FinishedTest(4);
    }
#endif
#if LEVEL>=5
    {
        TestShared p1(new TestChar('a'));
        TestShared p2(new TestChar('b'));
        TestShared p3(new TestChar('A'));
        assert(p1 == p1);
        assert(p1 < p2 || p2 < p1);
        FinishedTest(5);
    }
#endif
#if LEVEL>=6
    {
        TestShared p1(new TestChar('a'));
        TestShared p2(new TestChar('b'));
        swap(p1, p2);
        assert(!(p1 == p2));
        assert(p1->_c == 'b');
        assert(p2->_c == 'a');
        FinishedTest(6);
    }
#endif
#if LEVEL>=7
    {
        //auto p1 = MakeShared<TestChar>();

        auto p1 = MakeShared<TestChar>();
        auto p2 = MakeShared<TestChar>('b');
        assert(p1->_c == 'x' && p2->_c == 'b');
        FinishedTest(7);
    }
#endif
#if LEVEL>=11
    {
        TestWeak w0;
        assert(w0.expired());
        assert(w0.Invariant());
        FinishedTest(11);
    }
#endif
#if LEVEL>=12
    {
        TestShared s1(new TestChar('a'));
        TestWeak w1(s1);
        assert(!w1.expired());
        assert(w1.Invariant());
        TestShared s2 = w1.lock();
        assert(s1 == s2);
        TestWeak w0;
        TestShared s3 = w0.lock();
        assert(w0.expired());
        FinishedTest(12);
    }
#endif
#if LEVEL>=13
    {
        TestShared s1(new TestChar('a'));
        TestWeak w1(s1);
        TestWeak w2(w1);
        assert(s1 == w2.lock());
        FinishedTest(13);
    }
#endif
#if LEVEL>=14
    {
        TestShared s1(new TestChar('a'));
        TestWeak w1(s1);
        w1.reset();
        assert(w1.expired());
        TestWeak w2(s1);
        swap(w1, w2);
        assert(w2.expired());
        assert(s1 == w1.lock());
        FinishedTest(14);
    }
#endif
}