#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct heap_block {
    size_t size;
    struct heap_block* next;
    bool free;
} heap_block_t;

void heap_init(void* start, size_t size);
void* kmalloc(size_t size);
void kfree(void* ptr);


#endif