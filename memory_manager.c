#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct MemBlock
{
    void *header_ptr;
    void *end_ptr;
    struct MemBlock *next_mem;
    size_t size;
};

struct MemPool
{
    void *header_ptr;
    void *end_ptr;
    size_t size;
    struct MemBlock *first_block;
    struct MemBlock *last_block;
    int no_block;
} MemPool;

static
void block_init(struct MemBlock* block, void* ptr, size_t size){
    block->header_ptr = ptr;
    block->size = size;
    block->end_ptr = ptr + size;
    block->next_mem = NULL;
}

// Initialize memory pool
void mem_init(size_t size){
    MemPool.header_ptr = malloc(size);
    MemPool.end_ptr = MemPool.header_ptr + size;
    MemPool.size = size;
    MemPool.first_block = NULL;
    MemPool.no_block = 0;

    // printf("Create a pool size %zu at %p - %p\n", 
    // MemPool.size,MemPool.header_ptr,MemPool.end_ptr);
}

void* mem_alloc(size_t size){
    if (size <= 0 || size > MemPool.size) return NULL;
    
    MemPool.no_block++;
    struct MemBlock *new_mem = malloc(sizeof(struct MemBlock));
    struct MemBlock *cur_mem = MemPool.first_block;

    void *head = MemPool.header_ptr;

    for (int i = 0; i < MemPool.no_block; i++)
    {   
        if(cur_mem == NULL ){
            block_init(new_mem, head, size);
            MemPool.first_block = new_mem;
            return new_mem->header_ptr;
        }
        
        if(head + size <= cur_mem->header_ptr)
        {
            block_init(new_mem, head, size);
            new_mem->next_mem = cur_mem->next_mem;
            cur_mem->next_mem = new_mem;
            return new_mem->header_ptr;
        }
        else
        {
            head = cur_mem->end_ptr;
        }
        
        if(cur_mem->next_mem == NULL && head + size <= MemPool.end_ptr)
        {
            block_init(new_mem, head, size);
            cur_mem->next_mem = new_mem;
            return new_mem->header_ptr;
        }
        else if (cur_mem->next_mem != NULL)
        {
            cur_mem = cur_mem->next_mem;
        }
    }
    free(new_mem);
    MemPool.no_block--;
    return NULL;
}


void mem_free(void* block){
    struct MemBlock *cur_mem = MemPool.first_block;

    for (int i = 0; i < MemPool.no_block; i++)
    {
        if (block == NULL)
        {
            break;
        }
        else if (MemPool.first_block->header_ptr == block)
        {
            MemPool.first_block = cur_mem->next_mem;
            MemPool.no_block--;
            free(cur_mem);
            break;
        }
        else if (cur_mem->next_mem->header_ptr == block)
        {
            struct MemBlock *temp = cur_mem->next_mem;
            cur_mem->next_mem = cur_mem->next_mem->next_mem;
            MemPool.no_block--;
            free(temp);
            break;
        }
        else
        {
            cur_mem = cur_mem->next_mem;
        }
    }
}

void* mem_resize(void* block, size_t size){
    struct MemBlock *mem = MemPool.first_block;
    for(int i = 0; i < MemPool.no_block; i++){
        if (mem->header_ptr == block){
            if (mem->next_mem == NULL){
                if(mem->header_ptr + size <= MemPool.end_ptr){
                    mem->end_ptr = mem->header_ptr + size;
                    return mem->header_ptr;
                }
                return block;
            }
            else if (mem->header_ptr + size <= mem->next_mem->header_ptr){
                mem->end_ptr = mem->header_ptr + size;
                return mem->header_ptr;
            }
            else{
                void *temp = NULL;
                memcpy(temp, mem->header_ptr, mem->size);
                mem_free(mem->header_ptr);
                void *new_mem = mem_alloc(size);
                memcpy(new_mem, mem->header_ptr, mem->size);
                return new_mem;
            }
        }
        else{
            mem = mem->next_mem;
        }
    }
    return block;
}

void mem_deinit(){
    while(MemPool.first_block != NULL){
        mem_free(MemPool.first_block->header_ptr);
    }
    free(MemPool.header_ptr);
    
    MemPool.header_ptr = NULL;
    MemPool.size = 0;
    MemPool.end_ptr = NULL;
    MemPool.first_block = NULL;
    MemPool.no_block = 0;
}

