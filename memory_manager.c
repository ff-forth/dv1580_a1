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

// block_find finds the block and returns ptr of the previous block
struct MemBlock* block_find(void* block)
{
    // Defind the block
    struct MemBlock* prevBlock = &MemPool;

    // Find the block
    while (prevBlock->next != NULL)
    {
        if (prevBlock->next->ptr == block)
        {
            return prevBlock;
        }
    
        // Continue to the next block
        prevBlock = prevBlock->next;
    }

    // fprintf(stderr, "block_find failed, can not find block %p in the memory pool.\n", block);
    return prevBlock;
};

// mem_init initializes memory pool
void mem_init(size_t size)
{
    // Allocate space in the memory
    void* ptr = malloc(size);
    if (!ptr) 
    {
        fprintf(stderr, "mem_init failed, can not allocate memory.\n");
        return;
    }

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
        // fprintf(stderr, "mem_alloc error: Too small, block size is %zu\n", size);
        return NULL;
    }

    // Check if enough space in the Memory pool
    if (size > MemPool.size)
    {
        // fprintf(stderr, "mem_alloc error: Too large, Memory pool size is %zu.\n", MemPool.size);
        return NULL;
    }

    // Check if Memory pool is empty
    if (!MemPool.next)
    {
        MemPool.next = block_init(MemPool.ptr, size, NULL);
        return MemPool.next->ptr;
    }

    // Defind a block
    struct MemBlock* currentBlock = MemPool.next;
    
    // Check if it's a space before the first block
    if (currentBlock->ptr != MemPool.ptr && (MemPool.ptr + size) <= currentBlock->ptr)
    {
        struct MemBlock *temp = MemPool.next;
        MemPool.next = block_init(MemPool.ptr, size, temp);
        return MemPool.next->ptr;
    }
    
    // Check if it's space between two blocks
    while (currentBlock->next)
    {   
        if ((currentBlock->ptr + currentBlock->size + size) <= currentBlock->next->ptr)
        {
            struct MemBlock *temp = currentBlock->next;
            currentBlock->next = block_init(currentBlock->ptr + currentBlock->size, size, temp);
            return currentBlock->next->ptr;
        }

        // Move to the next block
        currentBlock = currentBlock->next;
    };

    // Check if it's enough space at the end
    if ((currentBlock->ptr + currentBlock->size + size) <= (MemPool.ptr + MemPool.size) && (currentBlock->ptr + currentBlock->size) != 0)
    {
        currentBlock->next = block_init(currentBlock->ptr + currentBlock->size, size, NULL);
        return currentBlock->next->ptr;
    }

    // fprintf(stderr, "mem_alloc failed, can not allocate a space size %zu.\n", size);
    return NULL;
}

// Free the allocated space in the memory pool
void mem_free(void* block)
{
    // Check if block ptr is null
    if (!block) 
    {
        // fprintf(stderr, "mem_free failed, block ptr is null.\n");
        return;
    }

    // Check if MemPool is empty
    if (!MemPool.next) 
    {
        // fprintf(stderr, "mem_free failed, MemPool is empty.\n");
        return;
    }
    
    // Defind the previous block to the block
    struct MemBlock* prevBlock = block_find(block);

    // Check if block exists
    if (!prevBlock->next) return;
    
    // Remove the block
    struct MemBlock *temp = prevBlock->next->next;
    free(prevBlock->next);
    if (!prevBlock->next) 
    {
        fprintf(stderr, "mem_free failed, free the block %p failed.\n", block);
    }
    prevBlock->next = temp;

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
        return block;
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
    struct MemBlock* mblock = MemPool.next;
    while(mblock != NULL)
    {
        struct MemBlock* next = mblock->next;
        free(mblock->ptr);
        mblock = next;
    }

    // Free the pool
    free(MemPool.ptr);
}
