#include "ldGui.h"
#include "ldCommon.h"
#include "xList.h"
#include "xConnect.h"
#include "ldUser.h"
#include "xBtnAction.h"
#include "ldConfig.h"
#include "ldImage.h"
#include "ldButton.h"
#include "ldText.h"

uint8_t pageNumNow=0;
uint8_t pageTarget=0;

int64_t sysTimer=0;

#define TOUCH_NO_CLICK           0
#define TOUCH_CLICK              1


static int16_t prevX,prevY,nowX,nowY;
static void *prevWidget;

void ldGuiClickedAction(uint8_t touchSignal,int16_t x,int16_t y)
{
    ldCommon_t *widget;
    ldPoint_t globalPos;
    ldPoint_t pos;
//    ldGeometry tempGeometry;
    xListNode *pNode;

    switch(touchSignal)
    {
    case BTN_NO_OPERATION:
    {
        break;
    }
    case BTN_PRESS:
    {
        widget=NULL;
//        if(temporaryTopWidget!=NULL)
//        {
//            pos.x=x;
//            pos.y=y;
//            globalPos=llListGetGlobalPos(((llGeneral*)temporaryTopWidget)->parentWidget);
//            tempGeometry=((llGeneral*)temporaryTopWidget)->geometry;
//            tempGeometry.x+=globalPos.x;
//            tempGeometry.y+=globalPos.y;
//            if(llPointInRect(pos,tempGeometry))
//            {
//                widget=temporaryTopWidget;
//            }
//        }
        if(widget==NULL)
        {
            pNode=ldGetWidgetInfoByPos(x,y);
            if(pNode!=NULL)
            {
                widget=pNode->info;
                LOG_DEBUG("click widget id:%d\n",widget->nameId);
            }
        }
        prevX=x;
        prevY=y;
        prevWidget=widget;//准备数据,释放时候使用

        if(widget!=NULL)
        {
            xEmit(widget->nameId,touchSignal);
        }
        break;
    }
    case BTN_HOLD_DOWN:
    {
        if((prevX!=x)||(prevY!=y))
        {
            widget=prevWidget;//不可以把static变量作为函数变量调用
            if(widget!=NULL)
            {
                nowX=x;
                nowY=y;
                xEmit(widget->nameId,SIGNAL_TOUCH_HOLD_MOVE);
            }
            prevX=x;
            prevY=y;
        }
        break;
    }
    case BTN_RELEASE:
    {
        widget=prevWidget;
        if(widget!=NULL)
        {
            xEmit(widget->nameId,touchSignal);
        }
        break;
    }
    default:
        break;
    }
}

void ldGuiTouchProcess(void)
{
    int16_t x;
    int16_t y;
    bool nowState;
    static bool prevState=TOUCH_NO_CLICK;
    uint8_t touchSignal=BTN_NO_OPERATION;

    nowState = ldCfgTouchGetPoint(&x,&y);

    if(nowState==TOUCH_CLICK)
    {
        if(prevState==TOUCH_NO_CLICK)//按下,                下降沿触发
        {
            touchSignal=BTN_PRESS;
        }
        else// prevState==TOUCH_CLICK 按住                低电平
        {
            touchSignal=BTN_HOLD_DOWN;
        }
    }
    else// nowState==TOUCH_NO_CLICK 无按下
    {
        if(prevState==TOUCH_NO_CLICK)//无按下                高电平
        {
            touchSignal=BTN_NO_OPERATION;
        }
        else// prevState==TOUCH_CLICK 按下,上升沿触发       上降沿触发
        {
            touchSignal=BTN_RELEASE;
        }
    }
    prevState=nowState;
    ldGuiClickedAction(touchSignal,x,y);
}



static void _widgetLoop(ldCommon_t *info,const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    switch(info->widgetType)
    {
    case widgetTypeWindow:
    case widgetTypeImage:
    {
        ldImageLoop((ldImage_t*)info,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeButton:
    {
        ldButtonLoop((ldButton_t*)info,ptParent,bIsNewFrame);
        break;
    }
    case widgetTypeText:
    {
        ldTextLoop((ldText_t*)info,ptParent,bIsNewFrame);
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
            
            if(((ldCommon_t *)temp_pos->info)->childList!=NULL)
            {
                _ldGuiLoop(((ldCommon_t *)temp_pos->info)->childList,ptParent,bIsNewFrame);
            }
        }
    }
}

void ldGuiInit(void)
{
    xEmitInit();
    ldUserPageInitFunc[pageNumNow]();
    LOG_INFO("[sys] page %d init\n",pageNumNow);
}

void ldGuiLogicLoop(void)
{
    ldUserPageLoopFunc[pageNumNow]();
}

void ldGuiLoop(const arm_2d_tile_t *ptParent,bool bIsNewFrame)
{
    //遍历控件
    _ldGuiLoop(&ldWidgetLink,ptParent,bIsNewFrame);

    //检查按键
    if(ldTimeOut(10,&sysTimer,true))
    {
        xBtnTick(10);
    }
    
    //检查触摸
    ldGuiTouchProcess();
    
    xConnectProcess();
}

void ldGuiQuit(void)
{
    ldUserPageQuitFunc[pageNumNow]();
    LOG_INFO("[sys] page %d quit\n",pageNumNow);
}

void ldGuiJumpPage(uint8_t pageNum)
{
    pageTarget=pageNum;
}
