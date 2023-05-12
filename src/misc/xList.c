#include "xList.h"
#include "ldCommon.h"

void xListAddTail(xListNode *pList,xListNode *newNode)
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

void xListDelete(xListNode *target)
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





xListInfo *xListInfoAdd(xListNode* pList, void* pInfo)
{
    if(pList==NULL)
    {
        return NULL;
    }
    xListInfo * newList = (xListInfo *)XMALLOC(sizeof(xListInfo));
    if(newList!=NULL)
    {
        newList->info=pInfo;
        xListAddTail(pList,&(newList->parentNode));
        return newList;
    }
    else
    {
        XFREE(newList);
    }
    return NULL;
}

xListNode* xListNewNode(xListNode** pListChild)
{
    xListNode* pNode=NULL;
    if(*pListChild==NULL)
    {
        pNode=(xListNode *)XMALLOC(sizeof(xListNode));
        if(pNode!=NULL)
        {
            pNode->next=pNode;
            pNode->prev=pNode;
            *pListChild=pNode;
        }
    }
    return pNode;
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

bool xListInfoPrevTraverse(xListNode *pList,void *pTarget,bool (traverseFunc)(xListInfo*,void*))
{
    xListNode *temp_pos,*safePos;
    xListInfo *link_info;

    //判断链表是否为空
    if((pList->next!=pList)&&(pList->prev!=pList))
    {
        list_for_each_prev_safe(temp_pos,safePos, pList)
        {
            link_info = list_entry(temp_pos, xListInfo, parentNode);
            if(link_info!=NULL)
            {
                if(traverseFunc(link_info,pTarget)==true)
                {
                    return true;
                }
            }
        }
    }
    return false;
}

