#include <stddef.h>
#include "memory.h"
#include "mpx/mcb.h"
#include <mpx/vm.h>

mcb* mcbHead = NULL;


void initialize_heap(size_t size)
{
    mcb* newMCB = (mcb*)kmalloc(size, 0, NULL); // Calls kmalloc to allocate mem on heap and make first MCB

    mcbHead = newMCB;

    newMCB->flag = 0;
    newMCB->size = size - sizeof(mcb);

    newMCB->next = NULL;
    newMCB->prev = NULL;

    newMCB->startAdd = (void*)((char*)newMCB + sizeof(mcb));
}


void* allocate_memory(size_t size)
{
    // Assigns cur to the front of the list
    mcb* cur = mcbHead;

    // iterates through list
    while (cur != NULL)
    {
        // Checks in a first fit manner that the first block size can be allocated
        if (cur->flag == 0 && cur->size >= size)
        {
            // Only splits the block if the remainder can hold it
            if (cur->size > size + sizeof(mcb))
            {
                mcb* newMCB = (mcb*)((char*)cur + sizeof(mcb) + size);

                // Makes the new mcb free and assigns the correct size
                newMCB->flag = 0;
                newMCB->size = cur->size - size - sizeof(mcb);


                newMCB->next = cur->next;
                newMCB->prev = cur;

                // Accounts for end of the list
                if (cur->next != NULL)
                {
                    cur->next->prev = newMCB;
                }
                
                cur->next = newMCB; 
            }

            // Changes the current MCB to allocated and moves it to the allocated section
            cur->flag = 1;
            cur->size = size;
            cur->startAdd = (void*)((char*)cur + sizeof(mcb));

            if (cur != mcbHead)
            {
                // Skips an element forward for removal
                if (cur->prev != NULL)
                    cur->prev->next = cur->next;

                // Skips an element backwards for removal
                if (cur->next != NULL)
                    cur->next->prev = cur->prev;

                cur->prev = NULL;
                cur->next = mcbHead;
                mcbHead->prev = cur;
                mcbHead = cur;
            }

            return cur->startAdd;
        }

        cur = cur->next;
    }

    return NULL;
}

int free_memory(void* addr)
{
    mcb* cur = mcbHead;

    // Iterates over the list until while condition is not met
    while (cur != NULL)
    {
        // Checks if the address of the current element is the same as what we're looking for
        if (cur->startAdd == addr && cur->flag == 1)
        {
            cur->flag = 0;

            // merges blocks forward
            if (cur->next != NULL && cur->next->flag == 0)
            {
                mcb* next = cur->next;

                cur->size += sizeof(mcb) + next->size;
                cur->next = next->next;

                if (next->next != NULL)
                    next->next->prev = cur;
            }

            // merges blocks backwards
            if (cur->prev != NULL && cur->prev->flag == 0)
            {
                mcb* prev = cur->prev;

                prev->size += sizeof(mcb) + cur->size;
                prev->next = cur->next;

                if (cur->next != NULL)
                    cur->next->prev = prev;
            }
            // success
            return 0;
        }

        cur = cur->next;
    }
    // failure
    return -1;
}
