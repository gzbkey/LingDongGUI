#ifndef _FREE_RTOS_HEAP_4_H_
#define _FREE_RTOS_HEAP_4_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include "stdint.h"

typedef struct xHeapStats
{
    size_t xAvailableHeapSpaceInBytes;      /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
    size_t xSizeOfLargestFreeBlockInBytes;  /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t xSizeOfSmallestFreeBlockInBytes; /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t xNumberOfFreeBlocks;             /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
    size_t xMinimumEverFreeBytesRemaining;  /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
    size_t xNumberOfSuccessfulAllocations;  /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
    size_t xNumberOfSuccessfulFrees;        /* The number of calls to vPortFree() that has successfully freed a block of memory. */
} HeapStats_t;

void * pvPortMalloc( size_t xWantedSize );
void vPortFree( void * pv );
void vPortGetHeapStats( HeapStats_t * pxHeapStats );
void *pvPortRealloc( uint8_t *srcaddr,size_t xWantedSize );

#ifdef __cplusplus
}
#endif








#endif //_FREE_RTOS_HEAP_4_H_
