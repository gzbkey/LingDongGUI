#include "ldConfig.h"
#include "ldWindow.h"
#include "ldGui.h"


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
    uint16_t rx;
    uint16_t ry;
    
    //添加触摸函数
    //touchState=touch_scan(&rx,&ry);
    *x=rx;
    *y=ry;
    if((touchState!=0)&&(((rx!=0xffff)&&(ry!=0xffff))||((rx!=0)&&(ry!=0))))
    {
        touchState=true;
    }
    else
    {
        touchState=false;
        *x=0xffff;
        *y=0xffff;
    }
    return touchState;
}
