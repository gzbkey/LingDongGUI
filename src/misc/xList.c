/*
 * Copyright 2021-2024 Ou Jianbo 59935554@qq.com
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

/**
 * @file    xList.c
 * @author  Ou Jianbo(59935554@qq.com)
 * @brief   简单的链表库
 */
#include "xList.h"
#include "ldCommon.h"

static void _xListAddTail(xListNode *pList,xListNode *newNode)
{
    xListNode * prevObj;
    xListNode * nextObj;

    //找出前后对象,中间插入,自身为下一个对象
    prevObj=pList->prev;
    nextObj=pList;

    //插入新对象
    nextObj->prev = newNode;
    newNode->next = nextObj;
    newNode->prev = prevObj;
    prevObj->next= newNode;
}

static void _xListDelete(xListNode *target)
{
    xListNode * prevObj;
    xListNode * nextObj;

    //找出前后对象
    prevObj=target->prev;
    nextObj=target->next;

    //前后指针相连
    nextObj->prev = prevObj;
    prevObj->next = nextObj;
}




//返回新节点地址
xListNode *xListInfoAdd(xListNode* pList, void* pInfo)
{
    if(pList==NULL)
    {
        return NULL;
    }
    xListNode * newList = (xListNode *)XCALLOC(sizeof(xListNode));
    if(newList!=NULL)
    {
        newList->info=pInfo;
        _xListAddTail(pList,newList);
        return newList;
    }
    else
    {
        XFREE(newList);
    }
    return NULL;
}

//返回还没释放的info的指针
void *xListInfoDel(xListNode* pList)
{
    void* pInfo;
    if(pList==NULL)
    {
        return NULL;
    }
    pInfo=pList->info;
    _xListDelete(pList);
    XFREE(pList);
    return pInfo;
}

xListNode* xListMallocNode(xListNode** pListChild)
{
    xListNode* pNode=NULL;
    if(*pListChild==NULL)
    {
        pNode=(xListNode *)XCALLOC(sizeof(xListNode));
        if(pNode!=NULL)
        {
            pNode->next=pNode;
            pNode->prev=pNode;
            pNode->info=NULL;
            *pListChild=pNode;
        }
    }
    return pNode;
}

void xListFreeNode(xListNode* pListChild)
{
    if(pListChild==NULL)
    {
        return ;
    }
    XFREE(pListChild);
}

//bool xListInfoTraverse(xListNode *pList,bool (traverseFunc)(xListInfo*))
//{
//    xListNode *temp_pos,*safePos;
//    xListInfo *link_info;

//    //判断链表是否为空
//    if((pList->next!=pList)&&(pList->prev!=pList))
//    {
//        list_for_each_safe(temp_pos,safePos, pList)
//        {
//            link_info = list_entry(temp_pos, xListInfo, parentNode);
//            if(link_info!=NULL)
//            {
//                if(traverseFunc(link_info)==true)
//                {
//                    return true;
//                }
//            }
//        }
//    }
//    return false;
//}

//bool xListInfoPrevTraverse(xListNode *pList,void *pTarget,bool (traverseFunc)(xListInfo*,void*))
//{
//    xListNode *temp_pos,*safePos;
//    xListInfo *link_info;

//    //判断链表是否为空
//    if((pList->next!=pList)&&(pList->prev!=pList))
//    {
//        list_for_each_prev_safe(temp_pos,safePos, pList)
//        {
//            link_info = list_entry(temp_pos, xListInfo, parentNode);
//            if(link_info!=NULL)
//            {
//                if(traverseFunc(link_info,pTarget)==true)
//                {
//                    return true;
//                }
//            }
//        }
//    }
//    return false;
//}

bool xListInfoPrevTraverse(xListNode *pList,void *pTarget,bool (traverseFunc)(xListNode*,void*))
{
    xListNode *temp_pos,*safePos;

    //判断链表是否为空
    if((pList->next!=pList)&&(pList->prev!=pList))
    {
        list_for_each_prev_safe(temp_pos,safePos, pList)
        {
            if(temp_pos->info!=NULL)
            {
                if(traverseFunc(temp_pos,pTarget)==true)
                {
                    return true;
                }
            }
        }
    }
    return false;
}



