/*!
  * @file     LQ_VoiceProcess.c
  *
  * @brief    信标声音处理
  *
  * @company  北京龙邱智能科技
  *
  * @author   LQ-005
  *
  * @note     Tab键 4个空格
  *
  * @version  V1.0
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     2020年4月27日
  */ 




#include <IfxCpu.h>
#include <LQ_ADC.h>
#include <LQ_CCU6.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>
#include <Main.h>
#include <Platform_Types.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>




/**
 * adc采集数据长度
 */
#define ADC_DATA_LEN    2048

/*
 * adc采集完成标志位
 */
volatile uint8  AdcFinishFlag = 0;

/*
 * adc双buff缓冲区  缓冲完成的区域序号
 */
volatile uint8  AdcBuffIndex = 0;

/*
 * adc数组下标
 */
volatile uint32 adcCount = 0;

/*
 * ADC数据   adc0 1 2 3 采集麦克风信号
 *
 *     2     1
 *
 *     3     0
 */
__attribute__ ((aligned(256)))  volatile int16_t g_adc0Data[2][ADC_DATA_LEN];
__attribute__ ((aligned(256)))  volatile int16_t g_adc1Data[2][ADC_DATA_LEN];
__attribute__ ((aligned(256)))  volatile int16_t g_adc2Data[2][ADC_DATA_LEN];
__attribute__ ((aligned(256)))  volatile int16_t g_adc3Data[2][ADC_DATA_LEN];


/*
 * 互相关结果
 */
float g_acor1[2][30];
float g_acor2[2][30];
float g_acor3[2][30];
float g_acor4[2][30];

/*
 * 小车角度
 */
float g_Angle   = 0;






/*!
  * @brief    信标一些初始化函数
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2020/4/28
  */
void VoiceInit(void)
{
	ADC_InitConfig(ADC0, 1000000); //初始化
	ADC_InitConfig(ADC1, 1000000); //初始化
	ADC_InitConfig(ADC2, 1000000); //初始化
	ADC_InitConfig(ADC3, 1000000); //初始化

	CCU6_InitConfig(CCU60, CCU6_Channel0, 100);  //100us进入一次中断  中断中采集adc数据

}

/*!
  * @brief    ccu6 采集中断服务函数   声音信号采集
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2020/4/28
  */
void VoiceGetSample(void)
{
	static uint8 adcIndex = 0;

	if(adcCount >= ADC_DATA_LEN)
	{
		adcCount = 0;
		AdcFinishFlag = 1;

		/* 切换buff缓冲区 */
		if(adcIndex == 0)
		{
			adcIndex = 1;
			AdcBuffIndex = 0;
		}
		else
		{
			adcIndex = 0;
			AdcBuffIndex = 1;
		}
	}

//		g_adc1Data[adcIndex][adcCount] = ADC_ReadAverage(ADC1, 3);
//		g_adc2Data[adcIndex][adcCount] = ADC_ReadAverage(ADC2, 3);
//		g_adc3Data[adcIndex][adcCount] = ADC_ReadAverage(ADC3, 3);
//		g_adc0Data[adcIndex][adcCount] = ADC_ReadAverage(ADC0, 3);

		g_adc0Data[adcIndex][adcCount] = ADC_Read(ADC0);
		g_adc1Data[adcIndex][adcCount] = ADC_Read(ADC1);
		g_adc2Data[adcIndex][adcCount] = ADC_Read(ADC2);
		g_adc3Data[adcIndex][adcCount] = ADC_Read(ADC3);

		adcCount++;

}



/*!
  * @brief    归一化处理
  *
  * @param    x   ： 要处理数据
  * @param    len ： 要处理数据长度
  *
  * @return   无
  *
  * @note     无
  *
  * @see
  *
  * @date     2020/4/28
  */
void Normal(int16_t *x, uint16 len)
{
	float sum = 0;
	int   i;

	for(i = 0; i < len; i++)
	{
		sum += x[i];
	}

	sum = sum / len;


	for(i = 0; i < len; i++)
	{
		x[i] -= sum;
	}
}

/*!
  * @brief    互相关
  *
  * @param    acor1：  y0 y1 互相关结果
  * @param    acor2：  y1 y2互相关结果
  * @param    acor3：  y0 y2互相关结果
  * @param    acor4：  y1 y3互相关结果
  * @param    y0   ： 互相关数据 y0
  * @param    y1   ： 互相关数据 y1
  * @param    y2   ： 互相关数据 y2
  * @param    y3   ： 互相关数据 y3
  * @param    len  ： 互相关数据长度
  *
  * @return   无
  *
  * @note     无
  *
  * @see
  *
  * @date     2020/4/28
  */
void Xcorr(float *acor1, float *acor2, float *acor3, float *acor4, int16_t *y0, int16_t *y1, int16_t *y2, int16_t *y3, uint16 len)
{
	float sum3 = 0;
	float sum4 = 0;
	float sum1 = 0;
	float sum2 = 0;
	int delay, i, j;
	for(delay = -15; delay < 15; delay++)
//	for(delay = -len + 1; delay < len; delay++)
	{
		sum1 = 0;
		sum2 = 0;
		sum3 = 0;
		sum4 = 0;
		for(i = 0; i < len; i++)
		{
			j = i + delay;
			if((j < 0))
			{
				sum1 += 0.0001f * y0[j + len] * y1[i];
				sum2 += 0.0001f * y1[j + len] * y2[i];
				sum3 += 0.0001f * y0[j + len] * y2[i];
				sum4 += 0.0001f * y1[j + len] * y3[i];
			}
			else if ((j >= len))
			{
				sum1 += 0.0001f * y0[j - len] * y1[i];
				sum2 += 0.0001f * y1[j - len] * y2[i];
				sum3 += 0.0001f * y0[j - len] * y2[i];
				sum4 += 0.0001f * y1[j - len] * y3[i];
			}
			else
			{
				sum1 += 0.0001f * y0[j] * y1[i];
				sum2 += 0.0001f * y1[j] * y2[i];
				sum3 += 0.0001f * y0[j] * y2[i];
				sum4 += 0.0001f * y1[j] * y3[i];
			}
		}
		acor1[15 + delay] = (float)sum1;
		acor2[15 + delay] = (float)sum2;
		acor3[15 + delay] = (float)sum3;
		acor4[15 + delay] = (float)sum4;
	}
}



/*!
  * @brief    得到最大相关位置
  *
  * @param    acor1： 互相关结果
  * @param    acor2： 互相关结果
  * @param    acor3： 互相关结果
  * @param    acor4： 互相关结果
  * @param    len  ： 数据长度
  * @param    index： 存放相关结果最大值下标
  *
  * @return   无
  *
  * @note     无
  *
  * @see
  *
  * @date     2020/4/28
  */
void SeekMaxAcor(float * acor1, float * acor2, float *acor3, float *acor4, int16_t len, int16_t *index)
{
	int16_t i = 0;

	index[0] = 0;
	index[1] = 0;
	index[2] = 0;
	index[3] = 0;
	for(i = 1; i < len; i++)
	{
		if(acor1[i] > acor1[index[0]])
		{
			index[0] = i;
		}
		if(acor2[i] > acor2[index[1]])
		{
			index[1] = i;
		}
		if(acor3[i] > acor3[index[2]])
		{
			index[2] = i;
		}
		if(acor4[i] > acor4[index[3]])
		{
			index[3] = i;
		}
	}

	index[0] = len/2 - index[0];
	index[1] = len/2 - index[1];
	index[2] = len/2 - index[2];
	index[3] = len/2 - index[3];
}


/*!
  * @brief    麦克风数据处理  获取角度信息
  *
  * @param    无
  *
  * @return   最大相关位置
  *
  * @note     无
  *
  * @see
  *
  * @date     2020/4/28
  */
void VoiceProcess(void)
{

//	if(AdcFinishFlag)
//	{
//		/* 数据处理    */
//		Normal((int16_t *)g_adc0Data[AdcBuffIndex], ADC_DATA_LEN);
//		Normal((int16_t *)g_adc1Data[AdcBuffIndex], ADC_DATA_LEN);
//		Normal((int16_t *)g_adc2Data[AdcBuffIndex], ADC_DATA_LEN);
//		Normal((int16_t *)g_adc3Data[AdcBuffIndex], ADC_DATA_LEN);
//
//		for(int i = 0; i < ADC_DATA_LEN; i ++)
//		{
//          /* 上报匿名上位机  看原始数据波形 */
//			ANO_DT_send_int16(g_adc0Data[AdcBuffIndex][i], g_adc1Data[AdcBuffIndex][i], g_adc2Data[AdcBuffIndex][i], g_adc3Data[AdcBuffIndex][i], 0, 0, 0, 0);
//		}
//
//		AdcFinishFlag = 0;
//	}


	if(AdcFinishFlag)
	{
		char txt[32];

		/* 存放相关峰值下标 */
		int16_t acorIndex[4];

		/* 记录时间 */
		uint32_t nowTime = STM_GetNowUs(STM0);

		/* 数据处理    */
		Normal((int16_t *)g_adc0Data[AdcBuffIndex], ADC_DATA_LEN);
		Normal((int16_t *)g_adc1Data[AdcBuffIndex], ADC_DATA_LEN);
		Normal((int16_t *)g_adc2Data[AdcBuffIndex], ADC_DATA_LEN);
		Normal((int16_t *)g_adc3Data[AdcBuffIndex], ADC_DATA_LEN);

		/* 互相关 */
        Xcorr((float *)&g_acor1[AdcBuffIndex], (float *)&g_acor2[AdcBuffIndex], (float *)&g_acor3[AdcBuffIndex], (float *)&g_acor4[AdcBuffIndex], (int16_t *)&g_adc0Data[AdcBuffIndex], (int16_t *)&g_adc1Data[AdcBuffIndex], (int16_t *)&g_adc2Data[AdcBuffIndex], (int16_t *)&g_adc3Data[AdcBuffIndex], ADC_DATA_LEN);

		/** 获取最大相关峰值  */
		SeekMaxAcor((float *)&g_acor1[AdcBuffIndex], (float *)&g_acor2[AdcBuffIndex], (float *)&g_acor3[AdcBuffIndex], (float *)&g_acor4[AdcBuffIndex], 30, acorIndex);

		/* 四组相关数据 最大最小结果下标 */
		uint8_t IndexMax = 0, IndexMin = 0;

		/** 找到绝对值最小和最大的值下标 */
		for(uint8_t i = 1; i < 4; i++)
		{
			if(abs(acorIndex[i]) >= abs(acorIndex[IndexMax]))
			{
				IndexMax = i;
			}
			if(abs(acorIndex[i]) <= abs(acorIndex[IndexMin]))
			{
				IndexMin = i;
			}
		}

		/* 判断大致方位 */
		if(IndexMin == 0)
		{
			if(acorIndex[1] > 0)
			{
				g_Angle = 0;
			}
			else
			{
				g_Angle = 180;
			}
		}

		else if(IndexMin == 1)
		{
			if(acorIndex[0] > 0)
			{
				g_Angle = 270;
			}
			else
			{
				g_Angle = 90;
			}
		}

		else if(IndexMin == 2)
		{
			if(acorIndex[3] > 0)
			{
				g_Angle = 45;
			}
			else
			{
				g_Angle = 225;
			}
		}

		else if(IndexMin == 3)
		{
			if(acorIndex[2] > 0)
			{
				g_Angle = 315;
			}
			else
			{
				g_Angle = 135;
			}
		}


		nowTime = STM_GetNowUs(STM0) - nowTime;

		if(IfxCpu_acquireMutex(&mutexTFTIsOk))
		{
			/* 显示距离 */
			sprintf(txt, "%-3d %-3d %-3d %-3d", acorIndex[0], acorIndex[1], acorIndex[2], acorIndex[3]);
			TFTSPI_P8X16Str(0, 0, txt, u16WHITE, u16BLUE);		//字符串显示


			sprintf(txt, "index: % 5d", nowTime);               //显示运算时间
			TFTSPI_P8X16Str(0, 1, txt, u16WHITE, u16BLUE);		//字符串显示

			sprintf(txt, "angle: %-5.2f", g_Angle);             //显示角度信息
			TFTSPI_P8X16Str(0, 2, txt, u16WHITE, u16BLUE);		//字符串显示

			TFTSPI_P16x16Str(1,5,(unsigned char*)"绽放的紫罗兰",u16PURPLE,u16BLUE);
			TFTSPI_P8X16Str(0, 6, "2020", u16PURPLE, u16BLUE);
			TFTSPI_P16x16Str(2,6,(unsigned char*)"年全国大学生",u16PURPLE,u16BLUE);
			TFTSPI_P16x16Str(1,7,(unsigned char*)"智能汽车竞赛",u16PURPLE,u16BLUE);
			TFTSPI_P16x16Str(0,8,(unsigned char*)"八一农大",u16PURPLE,u16BLUE);
			TFTSPI_P16x16Str(4,8,(unsigned char*)"声音信标",u16PURPLE,u16BLUE);
			IfxCpu_releaseMutex(&mutexTFTIsOk);
		}


		AdcFinishFlag = 0;
	}
}

