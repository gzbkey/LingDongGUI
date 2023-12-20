#include "mh210x.h"
#include "mh.h"
#include "perf_counter.h"
#include "st7789v16bit.h"
#include "arm_2d.h"
#include "arm_2d_disp_adapter_0.h"
#include "ldScene0.h"

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wmissing-field-initializers"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wunused-variable"
#   pragma clang diagnostic ignored "-Wunused-parameter"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#elif __IS_COMPILER_ARM_COMPILER_5__
#elif __IS_COMPILER_GCC__
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wformat="
#   pragma GCC diagnostic ignored "-Wpedantic"
#endif
void rccInit(void)
{
	RCC_DeInit();

	RCC_HSEConfig(RCC_HSE_ON);
	while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET);
	
	RCC_PLLCmd(DISABLE);
	MH_RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_27,1);
	
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	RCC_PCLK1Config(RCC_HCLK_Div2);
	RCC_PCLK2Config(RCC_HCLK_Div1);
	
	RCC_LSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET);
}

void gpioDeInit(void)
{
#if 0
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable,ENABLE);
#else
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
#endif

    PWR_BackupAccessCmd(ENABLE);
    RCC_LSEConfig(RCC_LSE_OFF);
    BKP_TamperPinCmd(DISABLE);
    PWR_BackupAccessCmd(DISABLE);
}

int main(void)
{	
//    uint32_t usbstatus=0;
//    uint16_t len;
//    uint16_t t;
	RCC_ClocksTypeDef clocks;
	
	rccInit();
	RCC_GetClocksFreq(&clocks);
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    init_cycle_counter(false);

    gpioDeInit();
    
    st7789v_init();
    st7789v_clear(0xff);
    ST7789V_BG_ON;
    
    arm_irq_safe {
        arm_2d_init();
    }

    disp_adapter0_init();
    
    arm_2d_scene0_init(&DISP0_ADAPTER);
    
    while(1)
    {
        disp_adapter0_task();
    }
//    delay_ms(1800);
//	USB_Port_Set(0); 	//USB先断开
//	delay_ms(700);
//	USB_Port_Set(1);	//USB再次连接
// 	Set_USBClock();   
// 	USB_Interrupts_Config();    
// 	USB_Init();	    
//	DP_PUUP = 1;
//	while(1)
//	{
//		if(usbstatus!=bDeviceState)//USB连接状态发生了改变.
//		{
//			usbstatus=bDeviceState;//记录新的状态
//			if(usbstatus==CONFIGURED)
//			{
////				printf("USB连接成功\n");
//			}else
//			{
////				printf("USB断开连接\n");
//			}
//		}
//		if(USB_USART_RX_STA&0x8000)
//		{					   
//			len=USB_USART_RX_STA&0x3FFF;//得到此次接收到的数据长度
//			usb_printf("\r\n您发送的消息长度为:%d，消息内容为：\r\n",len);
//			for(t=0;t<len;t++)
//			{
//				USB_USART_SendData(USB_USART_RX_BUF[t]);//以字节方式,发送给USB 
//			}
//			usb_printf("\r\n\r\n");//插入换行
//			USB_USART_RX_STA=0;
//		}
////        else
////		{
////			times++;
////			if(times%200==0)usb_printf("请输入数据,以回车键结束\r\n");  
////			delay_ms(10);   
////		}	
//	}
}

__attribute__((used))    //!< 避免下面的处理程序被编译器优化掉
void SysTick_Handler(void)
{


}

void Disp0_DrawBitmap (uint32_t x,uint32_t y,uint32_t width,uint32_t height,const uint8_t *bitmap) 
{
    st7789v_colorFill(x,y,x+width-1,y+height-1,(uint16_t *)bitmap);
}



#if defined(__clang__)
#   pragma clang diagnostic pop
#endif
