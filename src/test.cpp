#include <vector>
#include <list>

#include "growing_mem_pool.h"
#include "growing_allocator.h"
#include "manual_capacity_vector.h"

int main(int argc, char const *argv[]) {
    // We test how to use a custom allocator in a std container and in
    // a custom container.

    // Here we are using the default allocator (it calls new and delete directly).
    {
        std::vector<int> vec1;
        std::list<int> list1;
    }

    // This is equivalent to:
    {
        std::allocator<int> alloc;
        std::vector<int, std::allocator<int>> vec1(alloc);
        std::list<int, std::allocator<int>> list1(alloc);
    }

    // Now we use a custom allocator that will avoid calling new and delete
    // as often.
    // The memory pool will hold chunks of at least 1024 elements.
    GrowingMemPool mem_pool(1024);
    // This is just a pointer to the memory pool. We need both a memory pool and
    // a pointer to it to avoid problems with move/copy shenanigans.
    GrowingAllocator<int> growing_alloc(&mem_pool);

    // We create several vectors that use the same **MEMORY POOL** (the growing_allocator
    // class is just a pointer).
    {
        ManualCapacityVector<int, GrowingAllocator<int>> vec1(growing_alloc);
        ManualCapacityVector<int, GrowingAllocator<int>> vec2(growing_alloc);
        ManualCapacityVector<int, GrowingAllocator<int>> vec3(growing_alloc);

        // Now we resize the vector, but we only allocate memory **ONCE**.
        // Also, since we are not passing any value to initialize the elements,
        // ManualCapacityVector will not call the default constructor (the OS will
        // likely give us zeroed memory, so we resize two times to see this)
        vec1.realloc(200);
        vec1.resize(100);
        vec1[5] = 999;
        vec1.resize(0);
        vec1.resize(100);

        vec2.realloc(100);
        vec2.resize(100);

        vec3.realloc(100);
        vec3.resize(100);
        for (int i = 0; i < 100; i++) {
            std::cerr << vec1[i] << " ";
        }
        std::cerr << "\n";

        // If we want to initialize the elements, we can pass a value.
        vec1.resize(200, 42);
        for (int i = 0; i < 200; i++) {
            std::cerr << vec1[i] << " ";
        }
        std::cerr << "\n";
    }

    {
        // The custom allocator can also be used with std containers.
        // When using lists, the custom allocator is specially useful since
        // otherwise, the list will call new() for each node.
        std::list<int, GrowingAllocator<int>> list1(growing_alloc);
        list1.push_back(1);
        list1.push_back(2);
        list1.push_back(3);
        list1.push_back(4);
        list1.push_back(5);
    }

    return 0;
}
