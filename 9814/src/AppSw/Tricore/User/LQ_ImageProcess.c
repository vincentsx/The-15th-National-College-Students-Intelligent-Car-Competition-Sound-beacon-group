/*!
  * @file     LQ_ImageProcess.c
  *
  * @brief    
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
  * @date     2020年6月9日
  */ 

#include <LQ_CAMERA.h>
#include <LQ_DMA.h>
#include <LQ_GPIO_LED.h>
#include <LQ_TFT18.h>




/*!
  * @brief    图像数据处理
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
void ImageProcess(void)
{
	if(Camera_Flag == 2)
	{

		/* 提取部分使用的数据 */
		Get_Use_Image();

		/* 清除摄像头采集完成标志位  如果不清除，则不会再次采集数据 */
		Camera_Flag = 0;

		/* 二值化 */
		Get_01_Value(2);

//		//TFT1.8动态显示摄像头原始图像
//		TFTSPI_Road(0, 0, LCDH, LCDW, (unsigned char *)Image_Use);
//
//		// 显示二值化图像
//		TFTSPI_BinRoad(0, 0, LCDH, LCDW, (unsigned char *)Pixle);



	}
}
