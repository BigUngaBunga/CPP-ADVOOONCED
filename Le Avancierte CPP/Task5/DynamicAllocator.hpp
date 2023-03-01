#pragma once
template<size_t elementSize, class IndexType>
class DynamicAllocator {

private:
    unsigned char* memory;
    IndexType freeListHead = 0;
    IndexType terminationValue = -1;

    template<class F>
    void freelist_visitor(F&& f) const{
        IndexType nextFree = freeListHead;
        while (nextFree != terminationValue)
        {
            f();
            nextFree = static_cast<IndexType>(*(memory + nextFree));
        }
    }

public:


    explicit DynamicAllocator(size_t capacity) {
        alignas(elementSize) memory = new unsigned char[elementSize * capacity];
        std::cout << "Size of memory is: " << sizeof(memory) << " element size is: " << elementSize << std::endl;
        std::cout << "Total size of memory is: " << sizeof(memory) * capacity << std::endl;
        for (IndexType i = 0; i < capacity; i++)
        {
            if (i == capacity - 1)
                new (memory + i) IndexType(terminationValue);
            else
                new (memory + i) IndexType(i + 1);
        }
    }

    ~DynamicAllocator() {
        //TODO ta bort allt hanterat minne
    }

    template<class T, class... Args>
    T* create(Args... args) {
        //TODO lägg till föremål
        assert(sizeof(T) <= elementSize);
        assert(freeListHead != terminationValue);
        IndexType nextHead = static_cast<IndexType>(*memory + freeListHead);
        auto voidptr = new (memory + freeListHead) T(std::forward<Args>(args)...);
        freeListHead = nextHead;
        return static_cast<T*>(voidptr);
    }

    template<class T>
    void destroy(T* object) {
        //TODO ta bort om föremål finns i minnet
    }

    int count_free() const {
        int freeCounter = 0;
        freelist_visitor([&](){++freeCounter; });
        return freeCounter;
    }

    void dump_pool() const {
        //TODO skriv ut allt innehåll
        std::cout << "Printing pool content" << std::endl;

    }

    void* operator[](const IndexType index) {
        return (*memory + index);
    }
};