#ifndef __LD_MEM_H__
#define __LD_MEM_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"

extern void *ldMalloc(uint32_t size);
extern void *ldCalloc(uint32_t num,uint32_t size);
extern void ldFree(void *p);
extern void *ldRealloc(void *ptr,uint32_t newSize);

#ifdef __cplusplus
}
#endif

#endif
