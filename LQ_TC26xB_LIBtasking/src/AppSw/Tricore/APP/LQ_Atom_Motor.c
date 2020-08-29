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

#include <LQ_Atom_Motor.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_GPIO_LED.h>
#include <LQ_GTM.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>
#include <LQ_UART.h>
#include <stdio.h>



/*************************************************************************
*  函数名称：void LQ_Atom_8chPWM(void)
*  功能说明：GTM测试函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：不同频率下的8路呼吸灯，用杜邦线接到LED灯可以看到灯亮度渐变
*************************************************************************/
void LQ_Atom_8chPWM(void)
{
	unsigned short duty=0;

	ATOM_PWM_InitConfig(ATOMPWM0, 0, 10);
	ATOM_PWM_InitConfig(ATOMPWM1, 0, 100);
	ATOM_PWM_InitConfig(ATOMPWM2, 0, 1000);
	ATOM_PWM_InitConfig(ATOMPWM3, 0, 10000);
	ATOM_PWM_InitConfig(ATOMPWM4, 0, 50);
	ATOM_PWM_InitConfig(ATOMPWM5, 0, 500);
	ATOM_PWM_InitConfig(ATOMPWM6, 0, 5000);
	ATOM_PWM_InitConfig(ATOMPWM7, 0, 50000);

	while(1)
	{
		for(duty=0;duty<10;duty++)
		{
			ATOM_PWM_SetDuty(ATOMPWM0, duty * 100, 10);
			ATOM_PWM_SetDuty(ATOMPWM1, duty * 200, 100);
			ATOM_PWM_SetDuty(ATOMPWM2, duty * 300, 1000);
			ATOM_PWM_SetDuty(ATOMPWM3, duty * 400, 10000);
			ATOM_PWM_SetDuty(ATOMPWM4, duty * 500, 50);
			ATOM_PWM_SetDuty(ATOMPWM5, duty * 600, 500);
			ATOM_PWM_SetDuty(ATOMPWM6, duty * 700, 5000);
			ATOM_PWM_SetDuty(ATOMPWM7, duty * 800, 50000);
			delayms(500);
		}

		//Flash_LED();//蓝灯呼吸P10.5
    	LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
		delayms(500);              //延时等待
	}
}
/*************************************************************************
*  函数名称：void Test_GTM_ATOM_8chPWM(void)
*  功能说明：GTM测试函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动四个电机
*************************************************************************/
void LQ_Atom_Motor_8chPWM(void)
{
	unsigned short duty=5000;
	char txt[16];
//
//
//	TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
//	TFTSPI_CLS(u16BLUE);   //蓝色屏幕
//	TFTSPI_P8X16Str(0, 3, "Test Motor",u16WHITE,u16BLUE);		//字符串显示
	GPIO_KEY_Init();

	ATOM_PWM_InitConfig(ATOMPWM0, 5000, 12500);
	ATOM_PWM_InitConfig(ATOMPWM1, 5000, 12500);
	ATOM_PWM_InitConfig(ATOMPWM2, 5000, 12500);
	ATOM_PWM_InitConfig(ATOMPWM3, 5000, 12500);
	ATOM_PWM_InitConfig(ATOMPWM4, 5000, 12500);
	ATOM_PWM_InitConfig(ATOMPWM5, 5000, 12500);
	ATOM_PWM_InitConfig(ATOMPWM6, 5000, 12500);
	ATOM_PWM_InitConfig(ATOMPWM7, 5000, 12500);

	while(1)
	{
		if(KEY_Read(KEY0)==0)//按下KEY0键，占空比减小
		{
			if(duty>500)
				duty-=500;
		}
		if(KEY_Read(KEY2)==0)//按下KEY2键，占空比加大
		{
			if(duty<ATOM_PWM_MAX)//满占空比为10000
				duty+=500;
		}
		if(KEY_Read(KEY1)==0)//按下KEY1键，占空比中值
		{
			duty=5000;
		}

		ATOM_PWM_SetDuty(ATOMPWM0, duty, 12500);//驱动四个电机
		ATOM_PWM_SetDuty(ATOMPWM1, 5000, 12500);
		ATOM_PWM_SetDuty(ATOMPWM2, duty, 12500);
		ATOM_PWM_SetDuty(ATOMPWM3, 5000, 12500);
		ATOM_PWM_SetDuty(ATOMPWM4, duty, 12500);
		ATOM_PWM_SetDuty(ATOMPWM5, 5000, 12500);
		ATOM_PWM_SetDuty(ATOMPWM6, duty, 12500);
		ATOM_PWM_SetDuty(ATOMPWM7, 5000, 12500);

		sprintf(txt, "PWM: %05d;", duty);
		TFTSPI_P8X16Str(0, 5, txt,u16WHITE,u16BLUE);		//字符串显示
//		UART_PutStr(UART0,txt);
//    	LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
//		delayms(200);              //延时等待
	}
}
/*************************************************************************
*  函数名称：void LQ_ATom_Servo_2chPWM(void)
*  功能说明：GTM测试函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：驱动2个舵机，切记舵机不能卡死哦，先标定确认后再拧上舵盘
*************************************************************************/
void LQ_ATom_Servo_2chPWM(void)
{
	unsigned short duty=1570;
	char txt[16];

	TFTSPI_Init(1);        //LCD初始化  0:横屏  1：竖屏
	TFTSPI_CLS(u16BLUE);   //蓝色屏幕
	TFTSPI_P8X16Str(0, 3, "Test Servo",u16WHITE,u16BLACK);		//字符串显示
	GPIO_KEY_Init();

	ATOM_PWM_InitConfig(ATOMSERVO1, 1500, 100);//舵机频率为100HZ，初始值为1.5ms中值
	ATOM_PWM_InitConfig(ATOMSERVO2, 1500, 100);//舵机理论范围为：0.5ms--2.5ms，大多舵机实际比这个范围小

	while(1)
	{
		if(KEY_Read(KEY0)==0)//按下KEY0键，占空比减小，舵机顺时针转动
		{
			if(duty>1370)
			duty-=10;
		}
		if(KEY_Read(KEY2)==0)//按下KEY2键，占空比加大，舵机逆时针转动
		{
			if(duty<1770)
			duty+=10;
		}
		if(KEY_Read(KEY1)==0)//按下KEY1键，舵机回位中值
		{
			duty=1570;
		}

		ATOM_PWM_SetDuty(ATOMSERVO2, duty, 100);//驱动两个舵机
		ATOM_PWM_SetDuty(ATOMSERVO1, duty, 100);

		sprintf(txt, "PWM: %05d;", duty);
		TFTSPI_P8X16Str(0, 5, txt,u16WHITE,u16BLACK);		//字符串显示
		UART_PutStr(UART0,txt);

    	LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
		delayms(200);              //延时等待
	}
}
