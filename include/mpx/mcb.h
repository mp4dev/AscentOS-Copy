#ifndef MCB_H
#define MCB_H

#include <stddef.h>

typedef enum {
  FREE,
  ALLOCATED
} flag;

typedef struct mcb {
    void* startAdd; // base address of the usable memory in block
    size_t size; // holds the size of block in bytes
    int flag; // 0=free 1=allocated
    struct mcb *next;
    struct mcb *prev;
} mcb;

extern mcb* mcbHead;

/**
 * @brief Allocates memory as a single large free block. Used for initializing MCB list
 * @param size The size of the heap
 */
void initialize_heap(size_t size);

/**
 * @brief Allocates memory for a block off the free list in a first fit manner and puts it on the allocated list
 * @param size The size of a block
 * @return A pointer to the address, startAdd, of an MCB
 */
void *allocate_memory(size_t size);

/**
 * @brief Frees a single element from the allocated list and merges adjacent free blocks into one large free block
 * @param address The address of the MCB you want to free
 * @return 0 on success or -1 on error
 */
int free_memory(void *address);



#endif
