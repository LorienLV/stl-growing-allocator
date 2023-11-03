# STL Growing Allocator

This repository contains a custom C++ allocator for the STL (Standard Template Library) that utilizes a memory pool. This allocator allocates new memory chunks as needed and employs a strategy that doesn't allow memory deallocation. Instead, it recycles available portions of the most recently allocated chunk before allocating a new one. This approach is particularly beneficial when you have a good estimate of the upper bound of required memory, and memory recovery is unnecessary. For instance, it can significantly enhance the performance of filling a `std::list`.

The usage of the `GrowingMemPool` and `GrowingAllocator` provided in this repository is similar to using an STL PMR (Polymorphic Memory Resource) `polymorphic_allocator` and `monotonic_buffer_resource`, as described in [std::pmr::monotonic_buffer_resource](https://en.cppreference.com/w/cpp/memory/monotonic_buffer_resource). However, the key distinction is that GrowingMemPool works with a specified number of elements, while monotonic_buffer_resource operates in bytes, making it more challenging to pre-allocate exactly the required data.

## Example

An example of how to use the allocator can be found in `test.cpp`