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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_GPT12_H_
#define _LQ_GPT12_H_

#include "stdint.h"
#include <IfxGpt12.h>
#include <IfxGpt12_reg.h>
#include <Platform_Types.h>
/**
* @brief ENC 脉冲管脚
*/
typedef enum
{
  ENC2_InPut_P00_7 = 0x0200, ENC2_InPut_P33_7 = 0x0201,//00.7摄像头像素占用
  ENC3_InPut_P02_6 = 0x0300,                           //02.6摄像头数据口占用
  ENC4_InPut_P02_8 = 0x0400,                           //
  ENC5_InPut_P10_3 = 0x0501,                           //
  ENC6_InPut_P20_3 = 0x0600, ENC6_InPut_P10_2 = 0x0601 //
}ENC_InputPin_t;

/**
* @brief ENC 方向管脚
*/
typedef enum
{
  ENC2_Dir_P00_8 = 0x0200, ENC2_Dir_P33_6 = 0x0201,   //00.8摄像头占用
  ENC3_Dir_P02_7 = 0x0300,                            //02.7摄像头数据口占用
  ENC4_Dir_P00_9 = 0x0400, ENC4_Dir_P33_5 = 0x0401,   //00.9摄像头占用
  ENC5_Dir_P10_1 = 0x0501,
  ENC6_Dir_P20_0 = 0x0600
}ENC_DirPin_t;


#define ENC_GetTimer(pin)    (uint8)(pin >> 8)
void ENC_InitConfig(ENC_InputPin_t InputPin, ENC_DirPin_t DirPin);
int16_t ENC_GetCounter(ENC_InputPin_t InputPin);


#endif /* 0_APPSW_TRICORE_APP_LQ_GPT12_H_ */
