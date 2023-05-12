#include "ldCommon.h"
#include "xList.h"

NEW_LIST(ldWidgetLink);

__WEAK void *ldMalloc(uint32_t size)
{
    return malloc(size);
}

__WEAK void ldFree(void *p)
{
    free(p);
}

__WEAK void *ldRealloc(void *ptr,uint32_t newSize)
{
    return realloc(ptr,newSize);
}

static bool ldGetInfoByName(xListNode *inList,xListInfo ** out_info,uint16_t nameId)
{
    xListNode *tempPos,*safePos;
    xListInfo *pListInfo;
    ldCommon *pTempWidgetInfo;

    if(inList==NULL)
    {
        return false;
    }
    
    //判断链表是否为空
    if((inList->next!=inList)&&(inList->prev!=inList))
    {
        list_for_each_prev_safe(tempPos,safePos, inList)
        {
            pListInfo = list_entry(tempPos, xListInfo, parentNode);
            if(pListInfo!=NULL)
            {
                pTempWidgetInfo=(ldCommon*)(pListInfo->info);
                if(nameId==pTempWidgetInfo->nameId)
                {
                    *out_info=pListInfo;
                    return true;
                }
                else
                {
                        if(ldGetInfoByName(pTempWidgetInfo->childList,out_info,nameId)==true)
                        {
                            return true;
                        }
                }

            }
        }
    }
    return false;
}


xListInfo* ldGetWidgetInfoById(uint16_t nameId)
{
    xListInfo * pListInfo;
    if(ldGetInfoByName(&ldWidgetLink,&pListInfo,nameId)==true)
    {
        return pListInfo;
    }
    return NULL;
}


static bool ldGetInfoByPos(xListNode *inList,xListInfo ** out_info,int16_t x,int16_t y)
{
    xListNode *tempPos,*safePos;
    xListInfo *pListInfo,*pListInfo2;
    ldCommon *pTempWidgetInfo;
    ldPoint posT;
    int16_t widthT,heightT;

    if(inList==NULL)
    {
        return false;
    }
    
    //判断链表是否为空
    if((inList->next!=inList)&&(inList->prev!=inList))
    {
        list_for_each_prev_safe(tempPos,safePos, inList)
        {
            pListInfo = list_entry(tempPos, xListInfo, parentNode);
            if(pListInfo!=NULL)
            {
                pTempWidgetInfo=(ldCommon*)(pListInfo->info);
                
                //获取全局坐标，用绝对值坐标进行比较
            posT= ldGetGlobalPos(pTempWidgetInfo);
            widthT=pTempWidgetInfo->geometry.width;
            heightT=pTempWidgetInfo->geometry.height;
                
                if(pTempWidgetInfo->childList==NULL)//无子控件
                {
                    if((x>posT.x)&&(x<(posT.x+widthT))&&(y>posT.y)&&(y<(posT.y+heightT)))
                    {
                       *out_info= pListInfo;//返回控件widget
                       return true;
                    }
                }
                else//有子控件
                {
            if(pTempWidgetInfo->isHidden==false)//无隐藏
            {
                //判断在窗体范围内
                if((x>posT.x)&&(x<(posT.x+widthT))&&(y>posT.y)&&(y<(posT.y+heightT)))
                {
                    if(ldGetInfoByPos(pTempWidgetInfo->childList,&pListInfo2,x,y)==true)
                    {
                        *out_info= pListInfo2;
                        return true;
                    }
                    else
                    {
                        *out_info=pListInfo;
                        return true;
                    }
                }
            }
            
            }
        }
        }
    }
    return false;
}


xListInfo* ldGetWidgetInfoByPos(int16_t x,int16_t y)
{
    xListInfo * pListInfo;
    if(ldGetInfoByPos(&ldWidgetLink,&pListInfo,x,y)==true)
    {
        return pListInfo;
    }
    return NULL;
}

//获取全局坐标
ldPoint ldGetGlobalPos(ldCommon *widget)
{
    ldPoint pos={0,0},posParent;

    if(widget==NULL)
    {
        return pos;
    }

    pos.x=widget->geometry.x;
    pos.y=widget->geometry.y;

    posParent.x=0;
    posParent.y=0;

    if(widget->parentType!=widgetTypeNone)
    {
        if(widget->parentType!=widgetTypeBackground)
        {
            posParent=ldGetGlobalPos(widget->parentWidget);
        }
        else
        {
            posParent.x=((ldCommon*)widget->parentWidget)->geometry.x;
            posParent.y=((ldCommon*)widget->parentWidget)->geometry.y;
        }
    }

    pos.x+=posParent.x;
    pos.y+=posParent.y;

    return pos;
}


ARM_NONNULL(2)
bool ldTimeOut(uint16_t ms, int64_t *plTimer,bool isReset)
{
    assert(NULL != plTimestamp);
    int64_t lPeriod;
    int64_t lTimestamp = arm_2d_helper_get_system_timestamp();

    if(1==*plTimer)
    {
        return false;
    }
    
    lPeriod=arm_2d_helper_convert_ms_to_ticks(ms);
    if (0 == *plTimer) {
        *plTimer = lPeriod;
        *plTimer += lTimestamp;
        
        return false;
    }

    if (lTimestamp >= *plTimer)
    {
        if(isReset)
        {
            *plTimer = lPeriod + lTimestamp;
        }
        else
        {
            *plTimer =1;
        }
            return true;
    }
    return false;
}

