#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "memory_manager.h"

// block_info prints information of the block
void block_info(struct MemBlock *mblock)
{
    printf("\nMemBlock: %p\n", mblock);
    printf("Ptr: %p\n", mblock->ptr);
    printf("size: %zu\n", mblock->size);
    printf("Next: %p\n", mblock->next);
}

// pool_info prints informations of all block in the pool
void pool_info()
{
    struct MemBlock* mblock = &MemPool;
    
    while(mblock != NULL)
    {
        block_info(mblock);
        mblock = mblock->next;
    }
    
}

// block_init creates a MemBlock in in the memory pool
// and returns ptr of the created block
struct MemBlock* block_init(void* ptr, size_t size, void* next)
{
    void* blockptr = malloc(sizeof(struct MemBlock));
    struct MemBlock* block = (struct MemBlock*)blockptr;
    
    block->ptr = ptr;
    block->size = size;
    block->next = next;

    return block;
}

// block_find finds the block and returns ptr of the searched block
struct MemBlock* block_find(void* block)
{
    // Defind the block
    struct MemBlock* mblock = &MemPool;

    // Find the block and remove
    while (mblock->next != NULL)
    {
        if (mblock->next->ptr == block)
        {
            return mblock;
        }
    
        // Continue to the next block
        mblock = mblock->next;
    }

    fprintf("block_find failed, can not find block %p in the memory pool.\n", block);
    return mblock;
};

// mem_init initializes memory pool
void mem_init(size_t size)
{
    // Allocate space in the memory
    void* ptr = malloc(size);

    // Initialize MemPool
    MemPool.ptr = ptr;
    MemPool.size = size;
    MemPool.next = NULL;

}

// mem_alloc allocates space in the memory pool
void* mem_alloc(size_t size)
{
    // Check if size of MemBlock is greater than 0
    if (size <= 0)
    {
        fprintf(stderr, "mem_alloc error: Too small, block size is %zu\n", size);
        return NULL;
    }

    // Check if enough space in the Memory pool
    if (size > MemPool.size)
    {
        fprintf(stderr, "mem_alloc error: Too large, Memory pool size is %zu.\n", MemPool.size);
        return NULL;
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
    
    // Check if it's space between two blocks
    while (block->next != NULL)
    {   
        if ((block->ptr + block->size + size) <= block->next->ptr)
        {
            struct MemBlock *temp = block->next;
            block->next = block_init(block->ptr, size, temp);
            return block->next->ptr;
        }

        // Move to the next block
        block = block->next;
    };

    // Check if it's enough space at the end
    if ((block->ptr + block->size + size) <= (MemPool.ptr + MemPool.size) && (block->ptr + block->size) != 0)
    {
        block->next = block_init(block->ptr + block->size, size, NULL);
        return block->next->ptr;
    }

    fprintf(stderr, "mem_alloc failed, can not allocate a space size %zu.\n", size);
    return NULL;
}

// Free the allocated space in the memory pool
void mem_free(void* block)
{
    // Check if block ptr is null
    if (block == NULL)
    {
        fprintf(stderr, "mem_free error: block ptr is null.\n");
        return;
    }

    // Check if memory pool is empty
    if (MemPool.next == NULL)
    {
        fprintf(stderr, "mem_free error: Memory pool is empty.\n");
        return;
    }
    
    // Defind the previous block to the block
    struct MemBlock* mblock = block_find(block);

    // Check if block exists
    if (mblock->next == NULL)
    {
        fprintf(stderr, "mem_free failed, Block %p does not exist.\n");
        return;
    }
    
    // Remove the block
    struct MemBlock *temp = mblock->next->next;
    free(mblock->next);
    mblock->next = temp;

}

// mem_resize resizes the block size and returns the new ptr
void* mem_resize(void* block, size_t size)
{
    // Find the previous block to the block
    struct MemBlock* mblock = block_find(block);
    
    // Store all block's info
    mblock = mblock->next;
    void *mstart = mblock->ptr; 
    size_t msize = mblock->size;

    // Check if new size is smaller
    if (size <= msize)
    {
        mblock->size = size;
    }
    
    // Free the old block
    mem_free(block);
    
    // Move the data to the newBlock allocation
    block = memcpy(mem_alloc(size), mstart, msize);

    // Check if resize have done successfuly
    if (block == NULL)
    {
        fprintf(stderr,"mem_resize failed, can not find enough space.");
        block = memcpy(mem_alloc(msize), mstart, msize);
    }
    
    return block;
}

// mem_deinit frees all memory of the pool
void mem_deinit()
{
    // Free all mblock
    while(MemPool.next != NULL)
    {
        mem_free(MemPool.next->ptr);
    }

    // Free the pool
    free(MemPool.ptr);
}
