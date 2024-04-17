#ifndef _X_LIST_H_
#define _X_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include <stdio.h>

#ifndef XCALLOC
#define XCALLOC                     malloc
#endif
#ifndef XFREE
#define XFREE                       free
#endif

struct listNode {
    struct listNode *next, *prev;
    void *info;//ldCommon*
};

typedef struct listNode xListNode;

#define NEW_LIST(newLink)  xListNode newLink={ &(newLink), &(newLink), NULL }

#define list_entry(ptr, type, member)    (type *)((char*)(ptr) - offsetof(type, member))

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

#define list_for_each_prev_safe(pos, n, head) \
    for (pos = (head)->prev, n = pos->prev; \
         pos != (head); \
         pos = n, n = pos->prev)

xListNode* xListMallocNode(xListNode** pListChild);
void xListFreeNode(xListNode* pListChild);
xListNode* xListInfoAdd(xListNode* pList, void* pInfo);
void     * xListInfoDel(xListNode* pList);

//bool xListInfoTraverse(xListNode *pList,bool (traverseFunc)(xListInfo*));//bool (traverseFunc)(xListInfo* pEachInfo)
bool xListInfoPrevTraverse(xListNode *pList,void *pTarget,bool (traverseFunc)(xListNode *,void*));//bool (traverseFunc)(xListInfo* pEachInfo,void* pTarget);

#ifdef __cplusplus
}
#endif

#endif //_X_LIST_H_
