#ifndef _XLIST_H_
#define _XLIST_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdlib.h"
#include <stdio.h>

struct listNode {
    struct listNode *next, *prev;
};

typedef struct listNode xListNode;

#define NEW_LIST(newLink)  xListNode newLink={ &(newLink), &(newLink) }

typedef struct
{
    xListNode parentNode;
    void *info;//ldCommon*
}xListInfo;


#define list_entry(ptr, type, member)    (type *)((char*)(ptr) - offsetof(type, member))

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

#define list_for_each_prev_safe(pos, n, head) \
    for (pos = (head)->prev, n = pos->prev; \
         pos != (head); \
         pos = n, n = pos->prev)

#define XLIST_GET_ROOT(pList)  ((xListInfo*)(pList->next))->info;
xListNode* xListNewNode(xListNode** pListChild);
void xListDelete(xListNode *target);
xListInfo* xListInfoAdd(xListNode* pList, void* pInfo);

//bool xListInfoTraverse(xListNode *pList,bool (traverseFunc)(xListInfo*));//bool (traverseFunc)(xListInfo* pEachInfo)
bool xListInfoPrevTraverse(xListNode *pList,void *pTarget,bool (traverseFunc)(xListInfo *,void*));//bool (traverseFunc)(xListInfo* pEachInfo,void* pTarget);

#ifdef __cplusplus
}
#endif

#endif //_XLIST_H_
