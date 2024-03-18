#include "ldConfig.h"
#include "arm_2d.h"

/**
 * @brief   获取触摸坐标
 * 
 * @param   x 返回的x坐标
 * @param   y 返回的y坐标
 * @return  true 有效触摸
 * @return  false 无效触摸
 */
bool ldCfgTouchGetPoint(int16_t *x,int16_t *y)
{
    bool touchState=false;
    int16_t rx;
    int16_t ry;
    
    //添加触摸函数
//    touchState=vtMouseGetPoint(&rx,&ry);

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

#if USE_VIRTUAL_RESOURCE == 1
void __disp_adapter0_vres_read_memory( intptr_t pObj,
                                       void *pBuffer,
                                       uintptr_t pAddress,
                                       size_t nSizeInByte)
{
    (void)(pObj);
//    w25qxxRead(pBuffer,pAddress,nSizeInByte);
}

uintptr_t __disp_adapter0_vres_get_asset_address(
    uintptr_t pObj,
    arm_2d_vres_t *ptVRES)
{
    return pObj;
}
#endif

void Disp0_DrawBitmap (uint32_t x,uint32_t y,uint32_t width,uint32_t height,const uint8_t *bitmap) 
{
    //对接屏幕驱动的彩色填充函数
    //参考1
    //函数原型 void lcd_colorFill(uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint16_t *color)
    //填写 lcd_colorFill(x,y,x+width-1,y+height-1,(uint16_t *)bitmap);
    //参考2
    //函数原型 void lcd_colorFill(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t *color)
    //填写 lcd_colorFill(x,y,width,height,(uint16_t *)bitmap);
    
}
