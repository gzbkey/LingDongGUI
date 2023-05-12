#include "ldGui.h"
#include "ldCommon.h"
#include "xList.h"
#include "xConnect.h"
#include "ldImage.h"



void ldGuiInit(void)
{
    xConnectInit();
}

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
    xListInfo *link_info;

        list_for_each_safe(temp_pos,safePos, pLink)
        {
            link_info = list_entry(temp_pos, xListInfo, parentNode);
            if(link_info!=NULL)
            {
                _widgetLoop(link_info->info,ptParent,bIsNewFrame);
                
                if(((ldCommon *)link_info->info)->childList!=NULL)
                {
                    _ldGuiLoop(((ldCommon *)link_info->info)->childList,ptParent,bIsNewFrame);
                }
            }
        }
}

void ldGuiLoop(const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    //遍历控件
    _ldGuiLoop(&ldWidgetLink,ptParent,bIsNewFrame);
}

