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

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ

*  备    注：TC264只有15个固定的GPIO支持外部中断 这15个固定GPIO分为4组，每组只能选择其中一个作为外部中断管脚使用
*           0：P15_4  P33_7  P15_5                             1：P14_3  P15_8
*           2：P10_2  P02_1  P00_4  P20_0  P11_10              3：P10_3  P14_1  P02_0  P20_9  P15_1
*			外部中断服务函数在LQ_GPIO.c中  中断优先级配置在LQ_GPIO.h中 可以自行修改
视频地址：https://www.bilibili.com/video/BV1xp4y1C7Hu?from=search&seid=15269881299247521732
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <LQ_GPIO.h>
#include <LQ_GPIO_LED.h>
#include <LQ_STM.h>

/*************************************************************************
*  函数名称：void Test_GPIO_Extern_Int(void)
*  功能说明：测试外部中断
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：核心板上的LED固定时间P10.6翻转，LED灯闪烁，
*            当P15.8检测到下降沿时触发中断，P10.5翻转，LED闪烁，中断函数在LQ_GPIO.C中
*            可以把P15.8用杜邦线接到P10.6，触发中断后亮灯依次闪烁
*************************************************************************/
void Test_GPIO_Extern_Int(void)
{
	PIN_Exti(P15_8, PIN_IRQ_MODE_FALLING);//配置P15_8 下降沿触发中断
	while(1)
	{
    	LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
		delayms(500);              //延时等待
	}
}

