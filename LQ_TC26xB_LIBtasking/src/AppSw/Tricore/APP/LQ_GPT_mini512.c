/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】ZYF/chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年4月10日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】Hightec4.9.3/Tasking6.3及以上版本
【Target 】 TC264DA
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
________________________________________________________________

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <LQ_GPIO_LED.h>
#include <LQ_GPT12_ENC.h>
#include <LQ_OLED096.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>
#include <LQ_UART.h>
#include <stdint.h>
#include <stdio.h>

volatile        int16_t encValue1  = 0;
volatile		int16_t encValue2  = 0;
volatile		int16_t encValue3  = 0;
volatile		int16_t encValue4  = 0;
volatile		int16_t encValue5  = 0;

/*************************************************************************
*  函数名称：void LQ_GPT_4mini512(void)
*  功能说明：测试程序，UART和OLED显示
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：
*************************************************************************/
void LQ_GPT_4mini512(void)
{
  char txt[32];

  ENC_InitConfig(ENC2_InPut_P33_7, ENC2_Dir_P33_6);
  //ENC_InitConfig(ENC3_InPut_P02_6, ENC3_Dir_P02_7);//摄像头冲突，不建议用
  ENC_InitConfig(ENC4_InPut_P02_8, ENC4_Dir_P33_5);
  ENC_InitConfig(ENC5_InPut_P10_3, ENC5_Dir_P10_1);
  ENC_InitConfig(ENC6_InPut_P20_3, ENC6_Dir_P20_0);
  OLED_Init();
  OLED_P6x8Str(0, 0, "Test Encoder");
  while(1)
  {
    encValue1 = ENC_GetCounter(ENC2_InPut_P33_7);
    UART_PutStr(UART0,"\n");
    sprintf(txt, "Enc1: %05d; ", encValue1);
    UART_PutStr(UART0,txt);
    OLED_P6x8Str(0, 2, txt);
    encValue3 = ENC_GetCounter(ENC4_InPut_P02_8);
    sprintf(txt, "Enc3: %05d; ", encValue3);
    OLED_P6x8Str(0, 3, txt);
    UART_PutStr(UART0,txt);

    encValue4 = ENC_GetCounter(ENC5_InPut_P10_3);
    sprintf(txt, "Enc4: %05d; ", encValue4);
    OLED_P6x8Str(0, 4, txt);
    UART_PutStr(UART0,txt);

    encValue5 = ENC_GetCounter(ENC6_InPut_P20_3);
    sprintf(txt, "Enc5: %05d;", encValue5);
    OLED_P6x8Str(0, 5, txt);
    UART_PutStr(UART0,txt);
    /*
        encValue2 = ENC_GetCounter(ENC3_InPut_P02_6);
        sprintf(txt, "Enc2: %05d; ", encValue2);
        OLED_P6x8Str(0, 6, txt);
        UART_PutStr(UART0,txt);
    */
	LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
	delayms(200);              //延时等待
  }
}
/*************************************************************************
*  函数名称：void LQ_GPT_4mini512TFT(void)
*  功能说明：测试程序，UART和TFT1.8显示
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：
*************************************************************************/
void LQ_GPT_4mini512TFT(void)
{
  char txt[32];

  ENC_InitConfig(ENC2_InPut_P33_7, ENC2_Dir_P33_6);
  //ENC_InitConfig(ENC3_InPut_P02_6, ENC3_Dir_P02_7);//摄像头冲突，不建议用
  ENC_InitConfig(ENC4_InPut_P02_8, ENC4_Dir_P33_5);
  ENC_InitConfig(ENC5_InPut_P10_3, ENC5_Dir_P10_1);
  ENC_InitConfig(ENC6_InPut_P20_3, ENC6_Dir_P20_0);
  TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLUE);   //蓝色屏幕
  TFTSPI_P8X16Str(0, 0, "Test Encoder",u16WHITE,u16BLACK);		//字符串显示
  while(1)
  {
    encValue1 = ENC_GetCounter(ENC2_InPut_P33_7);
    UART_PutStr(UART0,"\n");
    sprintf(txt, "Enc1: %05d; ", encValue1);
    UART_PutStr(UART0,txt);
    TFTSPI_P8X16Str(0, 2, txt,u16WHITE,u16BLACK);		//字符串显示
    encValue3 = ENC_GetCounter(ENC4_InPut_P02_8);
    sprintf(txt, "Enc3: %05d; ", encValue3);
    TFTSPI_P8X16Str(0, 3, txt,u16WHITE,u16BLACK);		//字符串显示
    UART_PutStr(UART0,txt);

    encValue4 = ENC_GetCounter(ENC5_InPut_P10_3);
    sprintf(txt, "Enc4: %05d; ", encValue4);
    TFTSPI_P8X16Str(0, 4, txt,u16WHITE,u16BLACK);		//字符串显示
    UART_PutStr(UART0,txt);

    encValue5 = ENC_GetCounter(ENC6_InPut_P20_3);
    sprintf(txt, "Enc5: %05d;", encValue5);
    TFTSPI_P8X16Str(0, 5, txt,u16WHITE,u16BLACK);		//字符串显示
    UART_PutStr(UART0,txt);
    /*
        encValue2 = ENC_GetCounter(ENC3_InPut_P02_6);
        sprintf(txt, "Enc2: %05d; ", encValue2);
        OLED_P6x8Str(0, 6, txt);
        UART_PutStr(UART0,txt);
    */
	LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
	delayms(200);              //延时等待
  }
}
