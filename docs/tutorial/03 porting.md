## 移植

本文档以单片机为标准，说明移植过程

## Keil

安装好MDK-ARM，这里使用的版本是5.38。建议使用最新版本

### 移植前的准备
 * 安装arm-2d的pack
    * [下载](https://github.com/ARM-software/Arm-2D/releases/)
 * 安装perf_counter的pack
    * [下载](https://github.com/GorgonMeducer/perf_counter/releases/)
 * 安装ldgui的pack
    * [github下载](https://github.com/gzbkey/LingDongGUI/releases/)
    * [gitee下载](https://gitee.com/gzbkey/LingDongGUI/releases/)
 * 安装python,安装时注意勾选添加到系统环境变量的选项
    * [下载](https://www.python.org/downloads/)
 * 准备带屏幕的开发板，可以正常显示图片的keil项目(lcd_project)
 * 屏幕接口
    ```c 
    void Disp0_DrawBitmap (uint32_t x, 
                           uint32_t y, 
                           uint32_t width, 
                           uint32_t height, 
                           const uint8_t *bitmap)
    ```
 * 下载ldgui源码

    🏠️主仓库: https://gitee.com/gzbkey/LingDongGUI

    🏠️镜像仓库: https://github.com/gzbkey/LingDongGUI

|ℹ️ 关于github下载慢的问题|
|:----|
|推荐使用Watt Toolkit加速|

### 先让arm-2d跑起来

1. 在lcd_project中加入arm-2d、perf_counter、DSP、CMSIS，keil中选择Project -> Manage -> Run-Time Environment

    ![keilPackSelect](./images/03/arm2d%20Manage%20Run-Time%20Environment.png)

2. 选择ac6编译器，并且选择gnu11

    ![ac6Setting](./images/03/ac6%20setting.png)

3. 如果使用ac5编译器，则需要选择c99和gnu支持，但是不建议使用ac5

    ![ac5Setting](./images/03/ac5%20setting.png)

4. 确保keil的CMSIS版本不得低于5.7.0，查看方式，Project -> Manage -> Select Software Packs

    ![cmsisVersion](./images/03/cmsis%20version.png)

5. 树目录中的Acceleration，找到arm_2d_disp_adapter_0.h。

    编辑器的左下角选择 Configuration Wizard，进入图形配置界面，根据实际情况配置

    颜色位数（Screen Colour Depth）

    横向分辨率（Width of the screen）

    纵向分辨率（Height of the Screen）

    部分刷新缓冲块的宽度（Width of the PFB Block），配置为width/2

    部分刷新缓冲块的高度（Height of the PFB Block），配置为8

6. 树目录中的Acceleration，找到arm_2d_cfg.h。
    
    编辑器的左下角选择 Configuration Wizard，进入图形配置界面，配置Extra下的colour depth、width和height

7. 使用microLib需要添加__aeabi_assert

    不使用microLib需要关闭半主机模式，并自定义c库
    ```c 
    #include "stdio.h"
    #include "ctype.h"
    #include "stdlib.h"

    #if defined(__MICROLIB)
    void __aeabi_assert(const char *chCond, const char *chLine, int wErrCode) 
    {
        ARM_2D_UNUSED(chCond);
        ARM_2D_UNUSED(chLine);
        ARM_2D_UNUSED(wErrCode);
        while(1) {
            __NOP();
        }
    }
    #else

    #if (__ARMCC_VERSION >= 6010050)
    __asm(".global __use_no_semihosting\n\t");
    __asm(".global __ARM_use_no_argv\n\t");
    #else
    #pragma import(__use_no_semihosting)

    struct __FILE
    {
        int handle;
        /* Whatever you require here. If the only file you are using is */
        /* standard output using printf() for debugging, no file handling */
        /* is required. */
    };
    #endif


    typedef int FILEHANDLE;
    FILEHANDLE _sys_open(const char *name,int openmode)
    {
     return 0;
    }

    int _sys_close(FILEHANDLE fh)
    {
        return 0;
    }

    int _sys_write(FILEHANDLE fh, const unsigned char *buf, unsigned len, int mode)
    {
        return 0;
    }

    int _sys_read(FILEHANDLE fh, unsigned char*buf, unsigned len, int mode)
    {
        return 0;
    }

    int _sys_istty(FILEHANDLE fh)
    {
        return 0;
    }

    int _sys_seek(FILEHANDLE fh, long pos)
    {
        return 0;
    }

    int _sys_ensure(FILEHANDLE fh)
    {
        return 0;
    }

    long _sys_flen(FILEHANDLE fh)
    {
        return 0;
    }

    void _sys_exit(int status)
    {
        //while(1);
    }

    int _sys_tmpnam(char *name, int fileno, unsigned maxlength)
    {
        return 0;
    }

    void _ttywrch(int ch)
    {
    }

    int remove(const char *filename)
    {
        return 0;
    }

    char *_sys_command_string(char *cmd, int len)
    {
        return NULL;
    }

    void __aeabi_assert(const char *chCond, const char *chLine, int wErrCode) 
    {
        (void)chCond;
        (void)chLine;
        (void)wErrCode;
        
        while(1) 
        {
        }
    }

    #endif

    ```

8. main文件加入如下代码
    ```c 
    #include "arm_2d.h"
    #include "arm_2d_disp_adapters.h"
    #include "perf_counter.h"

    __attribute__((used))
    void SysTick_Handler(void)
    {
    }

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

    int main(void) 
    {
        system_init();     // 包括 LCD 在内的系统初始化

        init_cycle_counter(false);

        arm_irq_safe
        {
            arm_2d_init(); // 初始化 arm-2d
        }

        disp_adapter0_init();
        while(1)
        {
            disp_adapter0_task();
        }
    }
    ```

9. 运行效果

    ![arm2d-demo](./images/03/arm2d%20demo.gif)

### 加入ldgui

1. 在lcd_project中加入ldgui，keil中选择Project -> Manage -> Run-Time Environment

    ![](./images/03/ldgui%20Manage%20Run-Time%20Environment.png)

2. keil中选择Project -> Manage -> Run-Time Environment，Acceleration - Arm-2D Helper中，Scene设置为0

3. arm_2d_disp_adapter_0.h中添加ldgui配置头文件
    ```c
    #include "ldConfig.h" 
    ```

4. 新建ldConfig.c
    ```c
    #include "ldConfig.h"

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
    ```

5. 新建ldConfig.h
    ```c 
    #ifndef _LD_CONFIG_H_
    #define _LD_CONFIG_H_

    #ifdef __cplusplus
    extern "C" {
    #endif

    #include "stdint.h"
    #include "stdbool.h"
    #include "arm_2d_cfg.h"
    #include "lcd.h"

    // base config
    #define LD_CFG_COLOR_DEPTH                        (16)   // 8 16 32
    #define LD_CFG_SCEEN_WIDTH                        (320)
    #define LD_CFG_SCEEN_HEIGHT                       (240)
    #define LD_CFG_PFB_WIDTH                          LD_CFG_SCEEN_WIDTH
    #define LD_CFG_PFB_HEIGHT                         (10)
    #define LD_MEM_SIZE                               (12*1024) //BYTE
    #define LD_PAGE_MAX                               (2)
    #define USE_DIRTY_REGION                          1
    #define USE_VIRTUAL_RESOURCE                      0
    #define USE_OPACITY                               0
    #define USE_TLSF                                  1
    #define USE_RADIA_MENU_SCALE                      1

    //debug config
    #define LD_DEBUG                                  0
    #define __DISP0_CFG_DEBUG_DIRTY_REGIONS__         0

    // 以下不用修改
    #define __DISP0_CFG_DISABLE_NAVIGATION_LAYER__    1
    #define __DISP0_CFG_PFB_BLOCK_WIDTH__             LD_CFG_PFB_WIDTH
    #define __DISP0_CFG_PFB_BLOCK_HEIGHT__            LD_CFG_PFB_HEIGHT
    #define __DISP0_CFG_COLOUR_DEPTH__                LD_CFG_COLOR_DEPTH
    #define __DISP0_CFG_SCEEN_WIDTH__                 LD_CFG_SCEEN_WIDTH
    #define __DISP0_CFG_SCEEN_HEIGHT__                LD_CFG_SCEEN_HEIGHT
    #define __DISP0_CFG_VIRTUAL_RESOURCE_HELPER__     USE_VIRTUAL_RESOURCE

    #if __GLCD_CFG_COLOUR_DEPTH__ != LD_CFG_COLOR_DEPTH
    #error parameter configuration error. (arm_2d_cfg.h) __GLCD_CFG_COLOUR_DEPTH__ not equal to LD_CFG_COLOR_DEPTH
    #endif

    bool ldCfgTouchGetPoint(int16_t *x,int16_t *y);

    #ifdef __cplusplus
    }
    #endif

    #endif //_LD_CONFIG_H_
    ```

6. xLog.h关闭打印功能

    ```c
    #define SET_LOG_LEVEL            LOG_LEVEL_NONE
    ```

    如果需要使用打印功能，请自定义printf

7. 假设用户文件目录为user，则将[createUiFile.py](../../tools/createUiFile.py)复制到user目录
8. 运行createUiFile.py(自动生成)，输入需要生成的页面名称。如果需要同时生成多个页面，则直接编辑pageList.txt，在运行脚本，输入回车即可自动生成
9. 将文件导入项目中，main.c中添加页面文件的头文件
10. 在main函数中使用宏定义LD_ADD_PAGE，设置页面列表
    ~~~c
    #include "uiHome.h"
    #include "uiZigbee.h"
    #include "uiWifi.h"

    int main(void)
    {
        sysInit();

        LD_ADD_PAGE(uiHome);//Home页面序号为0
        LD_ADD_PAGE(uiZigbee);//Zigbee页面序号为1
        LD_ADD_PAGE(uiWifi);//Wifi页面序号为2

        arm_irq_safe {
            arm_2d_init();
        }

        disp_adapter0_init();

        arm_2d_scene0_init(&DISP0_ADAPTER);

        while(1)
        {
            disp_adapter0_task();
        }
    }
    ~~~

### 关于程序体积

* 请善用keil的优化等级

    ![](./images/03/keil%20optimization.png)

