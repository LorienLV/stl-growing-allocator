# STL Growing Allocator

This repository contains an implementation of a C++ custom allocator for the STL. The allocator uses a memory pool that allocates new chunks of memory when required, but it does not allow deallocation. The allocator recycles available portions of the most recently allocated chunk before allocating a new one. This strategy is useful when you know the upper bound of the required memory and you do not need to recover memory. For example, you can speed up filling a `std::list``.

## Example

An example of how to use the allocator can be found in `test.cpp`