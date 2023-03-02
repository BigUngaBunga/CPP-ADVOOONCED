#pragma once

#include "ConstantMath.hpp"

template<size_t elementSize>
struct alignas(elementSize) Index {};

template<size_t elementSize, class IndexType, 
    size_t chunkSize = (size_t)Pow(2,
    Ceil<double>(Sqrt<double>(elementSize, elementSize)))>
class DynamicAllocator {

private:

    using chunkType = Index<chunkSize>;
    chunkType* chunk;

    IndexType freeListHead = 0;
    IndexType terminationValue = -1;

    //DEBUG
    size_t capacity;
    //DEBUG

    template<class F>
    void freelist_visitor(F&& f) const{
        IndexType nextFree = freeListHead;
        while (nextFree != terminationValue)
        {
            f();
            nextFree = at<IndexType>(nextFree);
        }
    }

public:


    explicit DynamicAllocator(size_t capacity) : capacity(capacity), 
        chunk(new chunkType[capacity]){

        std::cout << "Size of chunk is: " << alignof(chunkType);
        std::cout << " element size is: " << elementSize << std::endl;
        std::cout << "Total size of memory is: " << alignof(chunkType) * capacity << std::endl;
        
        for (IndexType i = 0; i < capacity; i++)
            add_at<IndexType>(i, i == capacity - 1 ? terminationValue : i + 1);
    }

    ~DynamicAllocator() {
        //TODO ta bort allt hanterat minne
    }

    template<class T, class... Args>
    T* create(Args... args) {
        assert(sizeof(T) <= elementSize);
        assert(freeListHead != terminationValue);
        IndexType nextHead = at<IndexType>(freeListHead);
        auto pointer = add_at<T>(freeListHead, std::forward<Args>(args)...);
        freeListHead = nextHead;
        return pointer;
    }

    template<class T, class... Args>
    T* add_at(IndexType index, Args... args) {
        return static_cast<T*>(new (chunk + index) T(std::forward<Args>(args)...));
    }

    template<class T>
    void destroy(T* object) {
        //TODO kolla så att föremål finns i minnet
        IndexType index = (chunkType*)object - chunk;//TODO beräkna index
        //IndexType index = 0;
        object->~T();
        add_at<IndexType>(index, freeListHead);
        freeListHead = index;
    }

    //void destroy_at(IndexType index) {
    //    
    //    
    //}

    int count_free() const {
        int freeCounter = 0;
        freelist_visitor([&](){++freeCounter; });
        return freeCounter;
    }

    void dump_pool() const {
        //TODO skriv ut allt innehåll
        std::cout << "Printing pool content" << std::endl;
        for (size_t i = 0; i < capacity; i++)
        {
            auto value = at<IndexType>(i);
            std::cout << "value is: " << value << " at: " << i << "\n";
        }
    }


    template<class Return>
    Return at(const IndexType index) const {
        return *(Return*)(chunk + index);
    }
};