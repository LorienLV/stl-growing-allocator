#pragma once

#include <list>
#include <iostream>
#include <cstddef>

#define DEBUG 1

/**
 * A memory pull that only creates new chunks, it does not allow deallocation.
 *
 */

class GrowingMemPool {
public:
    /**
     * Construct a memory pool with one chunk.
     *
     * @param chunk_bytes The minimum number of bytes that
     * the pool will allocate when requested.
     */
    GrowingMemPool(std::size_t chunk_bytes) noexcept
        : chunk_bytes(chunk_bytes) {

        first_chunk = new Chunk{chunk_bytes,
                                0,
                                static_cast<std::byte *>(::operator new(chunk_bytes)),
                                nullptr};
        curr_chunk = first_chunk;
    };

    /**
     * Deallocate all the memory.
     *
     */
    ~GrowingMemPool() noexcept {
        Chunk *chunk = first_chunk;
        while (chunk != nullptr) {
            Chunk *next = chunk->next;

            ::operator delete(static_cast<void *>(chunk->data));
            delete chunk;

            chunk = next;
        }
    };

    /**
     * Clear the pool without deallocating the memory. The old memory will be used
     * for new allocations if possible.
     *
     */
    void clear() {
        curr_chunk = first_chunk;
        curr_chunk->used_bytes = 0;
    }

    // Do not allow copy.
    GrowingMemPool(const GrowingMemPool &) = delete;
    GrowingMemPool &operator=(const GrowingMemPool &) = delete;

    /**
     * Allocate memory for num_elements elements of size element_bytes.
     * Use previously allocated unused bytes if possible.
     *
     * @param num_elements The number of elements to allocate.
     * @param element_bytes The size of each element.
     * @return void* A pointer to the allocated memory.
     */
    void *allocate(std::size_t num_elements, std::size_t element_bytes) {
        const std::size_t new_bytes = num_elements * element_bytes;

        if (new_bytes > chunk_bytes) {
            throw std::runtime_error(
                "The memory pool does not support allocations larger than the "
                "minimum chunk size.");
        }

        void *ret = nullptr;

        // We need a new chunk.
        if (curr_chunk->total_bytes - curr_chunk->used_bytes < new_bytes) {
            // Allocate new chunk since there is no more chunks to reuse.
            if (curr_chunk->next == nullptr) {
                ret = ::operator new(chunk_bytes);
                Chunk *new_chunk = new Chunk{chunk_bytes,
                                             0,
                                             static_cast<std::byte *>(ret),
                                             nullptr};

                curr_chunk->next = new_chunk;

#if DEBUG
                std::cerr << "Created new chunk with " << chunk_bytes << " bytes\n";
#endif
            }
            curr_chunk = curr_chunk->next;
            curr_chunk->used_bytes = new_bytes;
        }
        else {
            ret = static_cast<void *>(curr_chunk->data + curr_chunk->used_bytes);
            curr_chunk->used_bytes += new_bytes;

#if DEBUG
            std::cerr << "Using " << new_bytes << " bytes in chunk\n";
#endif
        }

        return ret;
    }

private:
    struct Chunk {
        std::size_t total_bytes;
        std::size_t used_bytes;
        std::byte *data;

        Chunk *next;
    };

    Chunk *first_chunk;
    Chunk *curr_chunk;

    const std::size_t chunk_bytes;
};