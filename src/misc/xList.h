/*
 * Copyright (c) 2021-2024 Ou Jianbo (59935554@qq.com). All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

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
#include "ldMem.h"

#ifndef XMALLOC
#define XMALLOC                         ldMalloc
#endif
#ifndef XFREE
#define XFREE                           ldFree
#endif

typedef struct listNode {
    struct listNode *next, *prev;
    void *info;
}xListNode_t;

#define NEW_LIST(newLink)  xListNode_t newLink={ &(newLink), &(newLink), NULL }

#define list_entry(ptr, type, member)    (type *)((char*)(ptr) - offsetof(type, member))

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

#define list_for_each_prev_safe(pos, n, head) \
    for (pos = (head)->prev, n = pos->prev; \
         pos != (head); \
         pos = n, n = pos->prev)

xListNode_t* xListMallocNode(xListNode_t** pListChild);
void xListFreeNode(xListNode_t* pListChild);
xListNode_t* xListInfoAdd(xListNode_t* pList, void* pInfo);
void     * xListInfoDel(xListNode_t* pList);

//bool xListInfoTraverse(xListNode *pList,bool (traverseFunc)(xListInfo*));//bool (traverseFunc)(xListInfo* pEachInfo)
bool xListInfoPrevTraverse(xListNode_t *pList,void *pTarget,bool (traverseFunc)(xListNode_t *,void*));//bool (traverseFunc)(xListInfo* pEachInfo,void* pTarget);

#ifdef __cplusplus
}
#endif

#endif //_X_LIST_H_
