/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】zyf/chiusir
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
基于iLLD_1_0_1_11_0底层程序
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <IfxCpu.h>
#include <IfxScuWdt.h>
#include <LQ_CAMERA.h>
#include <LQ_CCU6.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_I2C_VL53.h>
#include <LQ_ImageProcess.h>
#include <LQ_MotorControl.h>
#include <LQ_SOFTI2C.h>
#include <LQ_TFT18.h>
#include <LQ_VoiceProcess.h>
#include <Main.h>
#include <Platform_Types.h>
#include <stdint.h>
#include <stdio.h>

IfxCpu_mutexLock mutexTFTIsOk = 0;   /** TFT18使用标志位  */

/**
 * 定时器 5ms和50ms标志位
 */
volatile uint8_t cpu1Flage5ms = 0;
volatile uint8_t cpu1Flage50ms = 0;

/* 期望速度  */
volatile int16_t targetSpeed = 10;

/* 避障标志位 */
volatile uint8_t evadibleFlage = 0;

int core1_main (void)
{

    IfxCpu_enableInterrupts();
    /*
     * 关闭看门狗
     * */
    IfxScuWdt_disableCpuWatchdog (IfxScuWdt_getCpuWatchdogPassword ());

    //等待CPU0 初始化完成
    while(!IfxCpu_acquireMutex(&mutexCpu0InitIsOk));


    /* 按键状态  滤波用的  */
    uint8_t keyStatus = 0;


    /* 电机、编码器初始化 */
	MotorInit();
	EncInit();

	/* 定时器、按键初始化 */
	CCU6_InitConfig(CCU61, CCU6_Channel0, 5000);
	GPIO_KEY_Init();


    while(1)//主循环
    {
    	if(cpu1Flage5ms)
    	{
    		cpu1Flage5ms = 0;

    		/* 按键 设置期望速度  */
			switch(KEY_Read_All())
			{
			  case KEY0DOWN:
				  if(keyStatus != 1)
				  {
					  targetSpeed += 5;
				  }
				  keyStatus = 1;
			  break;
			  case KEY1DOWN:
				  if(keyStatus != 2)
				  {
					  targetSpeed = 0;
				  }
				  keyStatus = 2;
			  break;
			  case KEY2DOWN:
				  if(keyStatus != 3)
				  {
					  targetSpeed -= 5;
				  }
				  keyStatus = 3;
			  break;

			  default :
				  keyStatus = 0;
				  break;
			 }

			/* 调试PID */
			SpeedCtrl(0, targetSpeed, 0);




    	}



    }

}
