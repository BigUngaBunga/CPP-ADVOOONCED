
//
//  main.cpp
//  DynamicAllocator
//

#include <iostream>
#include <chrono>
#include <cassert>
#include "Task.h"
#include "DynamicAllocator.hpp"

#define CustomTests

#define DefaultTests

#define Benchmark
#define TestDynamicAllocator
#define TestNewDelete

#ifndef DBG_NEW
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#define new DBG_NEW
#endif

void Task::RunTask5() const
{
#ifdef DBG_NEW
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    
    using IndexType = uint16_t;
    using SmollPool = DynamicAllocator<4, IndexType>;
    using Pool = DynamicAllocator<8, IndexType>;
    using BigPool = DynamicAllocator<16, IndexType>;

#ifdef DefaultTests
    // A few test types to create in the pool
    struct IAm2byte { int16_t x; };
    struct IAm4byte { int32_t x; };
    struct IAm8byte { int64_t x; };
    struct IAmPolymorphic
    {
        virtual ~IAmPolymorphic() noexcept
        {
            std::cout << __func__ << std::endl;
        }
    };

    // Pool with largest possible size w.r.t. index type
//    Pool pool {(IndexType)-1 - 1};

    // Capacity too large for the given index type,
    // since max value is reserved as a null value
//    Pool pool_ { (IndexType)-1 };

    constexpr auto log = Log2(12);
    constexpr auto ceil = Ceil(log);
    constexpr auto pow= Pow(2, ceil);

    const int capacity = 4;
    std::cout << "Creating pool with capacity " << capacity << "..." << std::endl;
    Pool pool{ capacity };

    pool.dump_pool();

#ifdef CustomTests
    SmollPool smallPool{ capacity };
    auto ein = smallPool.create<int>(1);
    auto zwei = smallPool.create<int>(2);
    auto drei = (int*)3;
    auto vier = smallPool.create<int>( 4);
    auto fünf = smallPool.create<int>(5);
    smallPool.destroy(zwei);
    smallPool.dump_pool();
    zwei = smallPool.create<int>(2);
    smallPool.dump_pool();
    smallPool.destroy(zwei);
    smallPool.dump_pool();

    if (false)
    {
        smallPool.create<double>(6.9); //Fails assert, type too big
        smallPool.destroy(drei); //Fails assert, outside of pool memory
    }

    std::cout << std::endl << "Emptying pool... " << std::endl;
    smallPool.destroy(fünf);
    smallPool.destroy(ein);
    smallPool.destroy(vier);
    smallPool.dump_pool();
    std::cout << std::endl;

#endif // CustomTests


    std::cout << "Creating 2 objects..." << std::endl;

    auto p1 = pool.create<IAm2byte>((int16_t)1);
    auto p2 = pool.create<IAm4byte>((int32_t)2);
    pool.dump_pool();
    assert(pool.count_free() == capacity - 2);

    std::cout << "Creating 2 objects..." << std::endl;
    auto p3 = pool.create<IAm8byte>((int64_t)3);
    auto p4 = pool.create<IAmPolymorphic>();

    pool.dump_pool();
    assert(pool.count_free() == capacity - 4);

    assert(p1->x == 1);
    assert(p2->x == 2);
    assert(p3->x == 3);

    std::cout << "Destroying 2 objects..." << std::endl;
    pool.destroy(p4);
    pool.destroy(p3);

    pool.dump_pool();
    assert(pool.count_free() == capacity - 2);

    std::cout << "Destroying 2 objects..." << std::endl;
    pool.destroy(p1);
    pool.destroy(p2);

    pool.dump_pool();
    assert(pool.count_free() == capacity);
#endif

#ifdef Benchmark

    const IndexType N = (IndexType)-1 - 1;
    int Nmult = 1000;
    struct A { int a, b, c; };
    A* ptrs[N];

#ifdef TestDynamicAllocator
    {
        // Start timings
        std::cout << "Running DynamicAllocator tests..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();

        {
            DynamicAllocator<sizeof(A), IndexType> pool{ N };

            for (int n = 0; n < Nmult; n++)
            {
                for (int i = 0; i < N; i++)
                    ptrs[i] = pool.create<A>(0, 1, 2);
                for (int i = 0; i < N; i++)
                    pool.destroy(ptrs[i]);
            }
        }

        // Show timings
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;
    }
#endif

#ifdef TestNewDelete
    {
        // Start timings
        std::cout << "Running new/delete tests ..." << std::endl;
        auto start = std::chrono::high_resolution_clock::now();

        for (int n = 0; n < Nmult; n++)
        {
            for (int i = 0; i < N; i++)
                ptrs[i] = new A{ 0, 1, 2 };
            for (int i = 0; i < N; i++)
                delete ptrs[i];
        }

        // Show timings
        auto end = std::chrono::high_resolution_clock::now();
        auto diff = end - start;
        std::cout << std::chrono::duration <double, std::milli>(diff).count() << " ms" << std::endl;
    }
#endif

#endif

    new int; 
    std::cout << "det finns en avsiktlig minnesläcka!";
}