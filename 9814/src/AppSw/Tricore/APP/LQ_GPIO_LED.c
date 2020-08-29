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

#include <LQ_GPIO.h>
#include <LQ_GPIO_LED.h>
#include <LQ_STM.h>

/*************************************************************************
*  函数名称：void LED_Init(void)
*  功能说明：GPIO初始化函数 LED灯所用P10.6、P10.5、P15.4和P15.6初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
void GPIO_LED_Init(void)
{
	  // 初始化,输入口，高电平
	  PIN_InitConfig(LED0p, PIN_MODE_OUTPUT, 0);
	  PIN_InitConfig(LED1p, PIN_MODE_OUTPUT, 0);
	  PIN_InitConfig(LED2p, PIN_MODE_OUTPUT, 0);
	  PIN_InitConfig(LED3p, PIN_MODE_OUTPUT, 0);
}

/*************************************************************************
*  函数名称：void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
*  功能说明：LED控制
*  参数说明：LEDn_e LEDno编号,LEDs_e sta状态亮/灭/翻转
*  函数返回：按键状态，0/1
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
void LED_Ctrl(LEDn_e LEDno, LEDs_e sta)
{
    switch(LEDno)
    {
    case LED0:
      if(sta==ON)        PIN_Write(LED0p,0);
      else if(sta==OFF) PIN_Write(LED0p,1);
      else if(sta==RVS) PIN_Reverse(LED0p);
    break;

    case LED1:
      if(sta==ON)        PIN_Write(LED1p,0);
      else if(sta==OFF) PIN_Write(LED1p,1);
      else if(sta==RVS) PIN_Reverse(LED1p);
    break;

    case LED2:
      if(sta==ON)        PIN_Write(LED2p,0);
      else if(sta==OFF) PIN_Write(LED2p,1);
      else if(sta==RVS) PIN_Reverse(LED2p);
    break;

    case LED3:
      if(sta==ON)        PIN_Write(LED3p,0);
      else if(sta==OFF) PIN_Write(LED3p,1);
      else if(sta==RVS) PIN_Reverse(LED3p);
    break;
    case LEDALL:
      if(sta==ON)
      {
    	  PIN_Write(LED0p,0);
    	  PIN_Write(LED1p,0);
    	  PIN_Write(LED2p,0);
    	  PIN_Write(LED3p,0);
      }
      else if(sta==OFF)
      {
    	  PIN_Write(LED0p,1);
    	  PIN_Write(LED1p,1);
    	  PIN_Write(LED2p,1);
    	  PIN_Write(LED3p,1);
      }
      else if(sta==RVS)
      {
    	  PIN_Reverse(LED0p);
    	  PIN_Reverse(LED1p);
    	  PIN_Reverse(LED2p);
    	  PIN_Reverse(LED3p);
      }
    break;
    default:
    break;
    }
}
/*************************************************************************
*  函数名称：void Test_GPIO_LED(void)
*  功能说明：测试程序
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：核心板上的LED灯闪烁--四个LED同时闪烁
*************************************************************************/
void Test_GPIO_LED(void)
 {
	GPIO_LED_Init();
 	while(1)
 	{
 		LED_Ctrl(LEDALL,RVS);        //四个LED同时闪烁
 		delayms(100);                //延时等待
 	}
 }
