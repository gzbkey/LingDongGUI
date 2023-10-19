#include "ldConfig.h"
#include "ldWindow.h"
#include "ldGui.h"
#include "Virtual_TFT_Port.h"
#include "ldCommon.h"
#include "xList.h"
#include "ldImage.h"

/***************************************************************************//**
 * @fn         bool ldCfgTouchGetPoint(int16_t *x,int16_t *y)
 * @brief      获取触摸坐标
 * @param      *x,*y  触摸坐标
 * @return     bool 是否有触摸
 * @version    V0.1
 * @date       
 * @details    
 ******************************************************************************/
bool ldCfgTouchGetPoint(int16_t *x,int16_t *y)
{
    bool touchState=false;
    int16_t rx;
    int16_t ry;
    
    //添加触摸函数
    touchState=vtMouseGetPoint(&rx,&ry);

    if((touchState!=0)&&(((rx!=-1)&&(ry!=-1))||((rx!=0)&&(ry!=0))))
    {
        if(rx<0)
        {
            rx=0;
        }
        if(ry<0)
        {
            ry=0;
        }
        if(rx>LD_CFG_SCEEN_WIDTH)
        {
            rx=LD_CFG_SCEEN_WIDTH;
        }
        if(ry>LD_CFG_SCEEN_HEIGHT)
        {
            ry=LD_CFG_SCEEN_HEIGHT;
        }
        *x=rx;
        *y=ry;
        touchState=true;
    }
    else
    {
        touchState=false;
        *x=-1;
        *y=-1;
    }
    return touchState;
}
