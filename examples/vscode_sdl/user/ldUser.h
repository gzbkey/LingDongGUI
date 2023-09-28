#ifndef _LD_USER_H_
#define _LD_USER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "ldCommon.h"
#include "xList.h"
#include "ldImage.h"

#define LD_PAGE_MAX              (2)

extern void (*ldUserPageInitFunc[LD_PAGE_MAX])(void);
extern void (*ldUserPageLoopFunc[LD_PAGE_MAX])(void);
extern void (*ldUserPageQuitFunc[LD_PAGE_MAX])(void);


#ifdef __cplusplus
}
#endif

#endif //_LD_USER_H_
