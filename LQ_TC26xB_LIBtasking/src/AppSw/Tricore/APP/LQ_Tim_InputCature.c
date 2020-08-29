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

#include <IfxGtm_PinMap.h>
#include <LQ_GPIO_LED.h>
#include <LQ_GTM.h>
#include <LQ_STM.h>
#include <LQ_UART.h>
#include <stdio.h>


/*************************************************************************
*  函数名称：void Test_GTM_ATOM_PWM(void)
*  功能说明：GTM测试函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月22日
*  备    注：P20_9作为PWM输出口，P15_0作为TIM输入口，两者短接后，串口P14.0发送到上位机
*  默认频率是125HZ，占空比50%
*************************************************************************/
void LQ_TIM_InputCature(void)
{
	char txt[16];
	float measuredPwmFreq_Hz, measuredPwmDutyCycle;
	TIM_InitConfig(IfxGtm_TIM1_3_TIN71_P15_0_IN);//P15_0 作为脉冲捕获管脚
	TOM_PWM_InitConfig(IfxGtm_TOM0_13_TOUT65_P20_9_OUT, 5000, 125);//初始化P20_9 作为PWM输出口 频率125Hz 占空比 百分之(5000/TOM_PWM_MAX)*100
	while(1)
	{
		TIM_GetPwm(IfxGtm_TIM1_3_TIN71_P15_0_IN, &measuredPwmFreq_Hz, &measuredPwmDutyCycle);
		sprintf(txt,"\nHz:%05f;dty:%f  ", measuredPwmFreq_Hz, measuredPwmDutyCycle);

		//串口发送到上位机
		UART_PutStr(UART0,txt);//数据输出到上位机，如：Hz:00124;dty:00050，如果输入悬空则为随机数
    	LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
		delayms(500);              //延时等待
	}
}
