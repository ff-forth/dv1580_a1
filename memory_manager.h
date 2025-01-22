// memory_manager.h
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

struct MemBlock
{
    void *ptr;
    size_t size;
    struct MemBlock *next;
}MemPool;

static void block_info(struct MemBlock *block);
struct MemBlock* block_init(void* ptr, size_t size, void* next);

void mem_init(size_t size);
void* mem_alloc(size_t size);
void mem_free(void* block);
void* mem_resize(void* block, size_t size);
void mem_deinit();

#endif
