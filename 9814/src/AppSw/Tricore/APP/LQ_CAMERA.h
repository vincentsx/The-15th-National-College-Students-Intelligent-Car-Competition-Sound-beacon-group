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

#ifndef __LQ_CAMERA_H
#define __LQ_CAMERA_H

#include "LQ_MT9V034.h"


#define LQMT9V034   //默认为神眼摄像头
#define USETFT1_8
//#define USEOLED    //使用OLED或者TFT1.8模块，默认为TFT1.8



#define IMAGEH  MT9V034_IMAGEH   /*!< 摄像头采集高度 */
#define IMAGEW  MT9V034_IMAGEW   /*!< 摄像头采集宽度 */

/* 使用数组宽高 修改这里即可 */
#ifdef USEOLED
	#define LCDH    60  /*!< OLED显示高度（用户使用）高度 */
	#define LCDW    80  /*!< OLED显示宽度（用户使用）宽度 */
#else
	#define LCDH    120  /*!< TFT显示高度（用户使用）高度 */
	#define LCDW    160  /*!< TFT显示宽度（用户使用）宽度 */
#endif

/** 图像原始数据存放 */
extern volatile unsigned char Image_Data[IMAGEH][IMAGEW];

/** 压缩后之后用于存放屏幕显示数据  */
extern unsigned char Image_Use[LCDH][LCDW];

/** 二值化后用于OLED显示的数据 */
extern unsigned char Pixle[LCDH][LCDW];


/*!
  * @brief    串口上报上位机
  *
  * @param    无
  *
  * @return   无
  *
  * @note     上位机的帧头可能有所区别 
  *
  * @see      CAMERA_Reprot();
  *
  * @date     2019/9/24 星期二
  */
void CAMERA_Reprot(void);



/*!
  * @brief    摄像头测试例程
  *
  * @param    fps:  帧率 
  *
  * @return   无
  *
  * @note     摄像头的一些参数，在LQ_MT9V034.c中的宏定义中修改
  *
  * @see      CAMERA_Init(50);   //初始化MT9V034  50帧 注意使用白色带与非门版转接座
  *
  * @date     2019/10/22 星期二
  */
void CAMERA_Init(unsigned char fps);


/**
  * @brief    获取需要使用的图像数组
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      Get_Use_Image();
  *
  * @date     2019/6/25 星期二
  */
void Get_Use_Image(void);


/**
  * @brief    二值化
  *
  * @param    mode  ：  0：使用大津法阈值    1：使用平均阈值
  *
  * @return   无
  *
  * @note     无
  *
  * @example  Get_01_Value(0); //使用大津法二值化
  *
  * @date     2019/6/25 星期二
  */
void Get_01_Value(unsigned char mode);



/*!
  * @brief    大津法求阈值大小 
  *
  * @param    tmImage ： 图像数据
  *
  * @return   阈值
  *
  * @note     参考：https://blog.csdn.net/zyzhangyue/article/details/45841255
  * @note     https://www.cnblogs.com/moon1992/p/5092726.html
  * @note     https://www.cnblogs.com/zhonghuasong/p/7250540.html     
  * @note     Ostu方法又名最大类间差方法，通过统计整个图像的直方图特性来实现全局阈值T的自动选取，其算法步骤为：
  * @note     1) 先计算图像的直方图，即将图像所有的像素点按照0~255共256个bin，统计落在每个bin的像素点数量
  * @note     2) 归一化直方图，也即将每个bin中像素点数量除以总的像素点
  * @note     3) i表示分类的阈值，也即一个灰度级，从0开始迭代	1
  * @note     4) 通过归一化的直方图，统计0~i 灰度级的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例w0，并统计前景像素的平均灰度u0；统计i~255灰度级的像素(假设像素值在此范围的像素叫做背景像素) 所占整幅图像的比例w1，并统计背景像素的平均灰度u1；
  * @note     5) 计算前景像素和背景像素的方差 g = w0*w1*(u0-u1) (u0-u1)
  * @note     6) i++；转到4)，直到i为256时结束迭代
  * @note     7) 将最大g相应的i值作为图像的全局阈值
  * @note     缺陷:OSTU算法在处理光照不均匀的图像的时候，效果会明显不好，因为利用的是全局像素信息。
  * @note     解决光照不均匀  https://blog.csdn.net/kk55guang2/article/details/78475414
  * @note     https://blog.csdn.net/kk55guang2/article/details/78490069
  * @note     https://wenku.baidu.com/view/84e5eb271a37f111f0855b2d.html
  *
  * @see      GetOSTU(Image_Use);//大津法阈值
  *
  * @date     2019/6/25 星期二
  */ 
int GetOSTU(unsigned char tmImage[LCDH][LCDW]);


/*!
  * @brief    摄像头测试例程
  *
  * @param
  *
  * @return
  *
  * @note     测试MT9V034  注意需要使用  带与非门版（白色）转接座
  *
  * @example
  *
  * @date     2019/10/22 星期二
  */
void Test_CAMERA(void);


/*!
  * @brief    基于soble边沿检测算子的一种边沿检测
  *
  * @param    imageIn    输入数组
  *           imageOut   输出数组      保存的二值化后的边沿信息
  *           Threshold  阈值
  *
  * @return
  *
  * @note
  *
  * @example
  *
  * @date     2020/5/15
  */
void lq_sobel(uint8_t imageIn[LCDH][LCDW], uint8_t imageOut[LCDH][LCDW], uint8_t Threshold);


/*!
  * @brief    基于soble边沿检测算子的一种自动阈值边沿检测
  *
  * @param    imageIn    输入数组
  *           imageOut   输出数组      保存的二值化后的边沿信息
  *
  * @return
  *
  * @note
  *
  * @example
  *
  * @date     2020/5/15
  */
void lq_sobelAutoThreshold(uint8_t imageIn[LCDH][LCDW], uint8_t imageOut[LCDH][LCDW]);

#endif



