#include <iostream>
#include <vector>
#include <list>

#include "growing_mem_pool.h"
#include "growing_allocator.h"

int main() {
    GrowingMemPool pool(1000);
    GrowingAllocator<int> allocator{&pool};

    std::list<int, GrowingAllocator<int>> l{allocator};
    std::vector<int, GrowingAllocator<int>> v{allocator};

    for (int i = 0; i < 100; ++i) {
        l.push_back(i);
        v.push_back(i);
    }

    return EXIT_SUCCESS;
}
