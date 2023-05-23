#include "ldGui.h"
#include "ldCommon.h"
#include "xList.h"
#include "xConnect.h"
#include "ldImage.h"



static void _widgetLoop(ldCommon *info,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    switch(info->widgetType)
    {
        case widgetTypeWindow:
        case widgetTypeImage:
        {
            ldImageLoop((ldImage*)info,ptParent,bIsNewFrame);
            break;
        }
        default:
            break;
    }
}

static void _ldGuiLoop(xListNode* pLink,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    xListNode *temp_pos,*safePos;

    list_for_each_safe(temp_pos,safePos, pLink)
    {
        if(temp_pos->info!=NULL)
        {
            _widgetLoop(temp_pos->info,ptParent,bIsNewFrame);
            
            if(((ldCommon *)temp_pos->info)->childList!=NULL)
            {
                _ldGuiLoop(((ldCommon *)temp_pos->info)->childList,ptParent,bIsNewFrame);
            }
        }
    }
}

void ldGuiLoop(const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    //遍历控件
    _ldGuiLoop(&ldWidgetLink,ptParent,bIsNewFrame);
}

