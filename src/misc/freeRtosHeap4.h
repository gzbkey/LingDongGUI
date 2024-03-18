#ifndef _FREE_RTOS_HEAP_4_H_
#define _FREE_RTOS_HEAP_4_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"

void *pvPortMalloc( size_t xWantedSize );
void vPortFree( void *pv );
void *pvPortRealloc( uint8_t *srcaddr,size_t xWantedSize );

#ifdef __cplusplus
}
#endif








#endif //_FREE_RTOS_HEAP_4_H_
