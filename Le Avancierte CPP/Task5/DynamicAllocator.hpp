#pragma once
template<size_t elementSize, class IndexType>
class DynamicAllocator {

private:
    using memoryType = unsigned char;
    memoryType* memory;
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
            //operator[](nextFree)
        }
    }

public:


    explicit DynamicAllocator(size_t capacity) : memory((memoryType*)malloc(elementSize * capacity)), capacity(capacity){
        std::cout << "Total size of memoryType is: " << alignof(memoryType) << std::endl;
        std::cout << "Total size of unsigned char is: " << alignof(unsigned char) << std::endl;
        std::cout << "Size of memory is: " << sizeof(memory) << " element size is: " << elementSize << std::endl;
        std::cout << "Total size of memory is: " << sizeof(memory) * capacity << std::endl;
        for (IndexType i = 0; i < capacity; i++)
            add_at<IndexType>(i, i == capacity - 1 ? terminationValue : i + 1);
        freeListHead = 0;
    }

    ~DynamicAllocator() {
        //TODO ta bort allt hanterat minne
    }

    template<class T, class... Args>
    T* create(Args... args) {
        //TODO lägg till föremål
        assert(sizeof(T) <= elementSize);
        assert(freeListHead != terminationValue);
        IndexType nextHead = at<IndexType>(freeListHead);
        auto pointer = add_at<T>(freeListHead, std::forward<Args>(args)...);
        freeListHead = nextHead;
        return pointer;
    }

    template<class T, class... Args>
    T* add_at(IndexType index, Args... args) {
        auto voidptr = new (memory + index) T(std::forward<Args>(args)...);
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
        for (size_t i = 0; i < capacity; i++)
        {
            auto value = at<IndexType>(i);
            std::cout << "value is: " << value << " at: " << i << "\n";
        }
    }


    template<class Return>
    Return at(const IndexType index) const {
        return static_cast<Return>(*(memory + index));
    }

    //template<>
    //void* at<void>(IndexType index) {
    //    return *(memory + index);
    //}
};