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
volatile int16_t targetSpeed = 15;

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

    /* 用户代码  */
    char txt[32];
    char txt1[32];

    /* 按键状态  滤波用的  */
    uint8_t keyStatus = 0;

    /* VL53 测距使用变量  */
    unsigned char vl53Dis_buff[2];
    uint16 vl53Dis = 0, vL53Last_dis = 0;

    /* 电机、编码器初始化 */
	MotorInit();
	EncInit();

	/* 定时器、按键初始化 */
	CCU6_InitConfig(CCU61, CCU6_Channel0, 5000);
	GPIO_KEY_Init();

	/* VL53使用的IIC接口初始化 */
	IIC_Init();

    /* 摄像头初始化 */
    //CAMERA_Init(50);

	/* VL53开始测距 */
	VL53_Write_Byte(VL53ADDR, VL53L0X_REG_SYSRANGE_START, 0x02);



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
					  targetSpeed += 2;
				  }
				  keyStatus = 1;
			  break;
			  case KEY1DOWN:
				  if(keyStatus != 2)
				  {
					  targetSpeed = 15;
				  }
				  keyStatus = 2;
			  break;
			  case KEY2DOWN:
				  if(keyStatus != 3)
				  {
					  targetSpeed -= 2;
				  }
				  keyStatus = 3;
			  break;

			  default :
				  keyStatus = 0;
				  break;
			 }

			/* 50ms 获取一次 VL53数据 */
			if(cpu1Flage50ms >= 10)
			{
				cpu1Flage50ms = 0;

				/* 获取测量数据 */
				VL53_Read_nByte(VL53ADDR, VL53_REG_DIS, 2, vl53Dis_buff);

				/* 转换数据 */
				vl53Dis = (vl53Dis_buff[0]<<8) | (vl53Dis_buff[1]);

				/* 本次测距数据有误 则使用上次数据  */
				if(vl53Dis == 20)
				{
					vl53Dis = vL53Last_dis;
				}

				vL53Last_dis = vl53Dis;

				static uint8_t vl53Count = 0;

				/* 小车前15cm内有障碍  置位避障标志位  */
				if(vl53Dis < 150)

				{
					vl53Count++;
					if(vl53Count > 10)
					{
						evadibleFlage = 1;
					}

				}
				else
				{
					vl53Count = 0;
					evadibleFlage = 0;
				}

				if(IfxCpu_acquireMutex(&mutexTFTIsOk))
				{
					sprintf(txt,"%-5d", vl53Dis);//显示距离
					TFTSPI_P8X16Str(0, 3, "VL-53:", u16WHITE, u16BLUE);
					TFTSPI_P8X16Str(6, 3, txt, u16WHITE, u16BLUE);
					sprintf(txt1, "expect: %d", targetSpeed);             //期望速度
					TFTSPI_P8X16Str(0, 4, txt1, u16WHITE, u16BLUE);		//字符串显示
					//TFTSPI_P8X8Str(15,15, txt,u16RED,u16BLUE);
					IfxCpu_releaseMutex(&mutexTFTIsOk);
				}


			}



//			sprintf(txt, "angle: %-5.2f", g_Angle);             //显示角度信息
//			TFTSPI_P8X8Str(0, 15, txt, u16WHITE, u16BLUE);		//字符串显示


			/* 图像处理  */
//			ImageProcess();


    	}



    }

}
