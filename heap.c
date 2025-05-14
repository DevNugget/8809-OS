#include "heap.h"

static heap_block_t* free_list = NULL;
static void* heap_start = NULL;
static void* heap_end = NULL;

void heap_init(void* start, size_t size) {
    heap_start = start;
    heap_end = start + size;
    
    free_list = (heap_block_t*)start;
    free_list->size = size - sizeof(heap_block_t);
    free_list->next = NULL;
    free_list->free = true;
}

void* kmalloc(size_t size) {
    heap_block_t* curr = free_list;

    while (curr) {
        if (curr->free && curr->size >= size) {
            if (curr->size > size + sizeof(heap_block_t)) {
                // Split block
                heap_block_t* new_block = (heap_block_t*)((uint8_t*)curr + sizeof(heap_block_t) + size);
                new_block->size = curr->size - size - sizeof(heap_block_t);
                new_block->next = curr->next;
                new_block->free = true;

                curr->size = size;
                curr->next = new_block;
            }

            curr->free = false;
            return (void*)((uint8_t*)curr + sizeof(heap_block_t));
        }

        curr = curr->next;
    }

    return NULL; // Out of memory
}

void kfree(void* ptr) {
    if (!ptr) return;

    heap_block_t* block = (heap_block_t*)((uint8_t*)ptr - sizeof(heap_block_t));
    block->free = true;

    // Coalesce adjacent free blocks
    heap_block_t* curr = free_list;
    while (curr) {
        if (curr->free && curr->next && curr->next->free) {
            curr->size += sizeof(heap_block_t) + curr->next->size;
            curr->next = curr->next->next;
        } else {
            curr = curr->next;
        }
    }
}
