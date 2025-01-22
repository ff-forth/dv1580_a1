#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "memory_manager.h"

static
void block_info(struct MemBlock *block)
{
    printf("\nMemBlock: %p\n", block);
    printf("Header: %p\n", block->ptr);
    printf("size: %zu\n", block->size);
    printf("Next: %p\n", block->next);
}

static
struct MemBlock* block_init(void* ptr, size_t size, void* next)
{
    void* blockptr = malloc(sizeof(struct MemBlock));
    struct MemBlock* block = (struct MemBlock*)blockptr;
    
    block->size = size;
    block->ptr = ptr;
    block->next = next;
    
    return block;
}

// Initialize memory pool
void mem_init(size_t size)
{
    // Allocate space in the memory
    void* ptr = malloc(size);

    // Initialize MemPool
    MemPool.ptr = ptr;
    MemPool.size = size;
    MemPool.next = NULL;

}

void* mem_alloc(size_t size)
{
    // Check if enought space in the Memory pool
    if (size > MemPool.size)
    {
        fprintf(stderr, "Error: Too large, Memory pool size is %d.", MemPool.size);
        return -1;
    }

    // Check if Memory pool is empty
    if (MemPool.next == NULL)
    {
        MemPool.next = block_init(MemPool.ptr, size, NULL);
        return MemPool.next->ptr;
    }

    // Defind a block
    struct MemBlock* block = MemPool.next;
    
    // Check if it's space before the first block
    if (block->ptr != MemPool.ptr && (MemPool.ptr + size) <= block->ptr)
    {
        struct MemBlock *temp = MemPool.next;
        MemPool.next = block_init(MemPool.ptr, size, temp);
        return MemPool.next->ptr;
    }
    
    while (block->next != NULL)
    {   
        // Check if it's space between two blocks
        if ((block->ptr + block->size + size) <= block->next->ptr)
        {
            struct MemBlock *temp = block->next;
            block->next = block_init(block->ptr, size, temp);
            return block->next->ptr;
        }

        // Move to the next block
        block = block->next->ptr;
    };

    // Check if it's enough space at the end
    if ((block->ptr + block->size + size) <= (MemPool.ptr + MemPool.size))
    {
        block->next = block_init(block->ptr, size, NULL);
        return block->next->ptr;
    }
    
    return 0;
}


void mem_free(void* block)
{
    
}

void* mem_resize(void* block, size_t size)
{

}

void mem_deinit()
{

}
