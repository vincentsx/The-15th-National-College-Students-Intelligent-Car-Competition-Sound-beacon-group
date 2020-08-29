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
#ifndef __LQ_RDA5807_H
#define __LQ_RDA5807_H

#include "LQ_GPIO.h"
#include "stdint.h"


//RDA 的寄存器地址
#define RDA_R00		0x00  //读出16个位的ID =0x5800
#define RDA_R02		0x02  //DHIZ[15],DMUTE[14]静音,MONO[13]声道,BASS[12]重低音,SEEKUP[9],SEEK[8],SKMODE[7],CLK_MODE[6:4]时钟源选择,SOFTRESET[1]软复位,ENABLE[0]电源使能
#define RDA_R03		0x03  //CHAN[15:6],TUNE[4],BAND[3:2],SPACE[1:0] 设置频率 带宽 步长
#define RDA_R04		0x04  //STCIEN[14],DE[11],I2Senable[6],
#define RDA_R05		0x05  //INT_MODE[15],SEEKTH[14:8](设定自动搜索信号强度阀值),LNA_PORT_SEL[7:6]=0b10,LNA_ICSEL_BIT[5:4],VOLUME[3:0]音量;
#define RDA_R0A		0x0A  //STC[14]seek complete SF[13]seek fail readchan[9:0]当前频道
#define RDA_R0B		0x0B  //RSSI[15:9],FM TRUE[8]当前频道是一个节目台

//IIC器件地址0010000B，加上读写标志，即0x22（写），0x23（读），这种方式可以单独对相应的寄存器操作，并且可以直接读写每一个寄存器。
#define RDA_READ       0x23  //读RDA5807M，0x23
#define RDA_WRITE	   0x22  //写RDA5807M，0x22

#define SOFT_FMI2C_SCL_PIN   P00_1   /*!< P00_1  作为 SCL */
#define SOFT_FMI2C_SDA_PIN   P00_2   /*!< P00_2  作为 SDA */

#define FMSDA_OUT        PIN_Dir(SOFT_FMI2C_SDA_PIN, 1)
#define FMSDA_IN         PIN_Dir(SOFT_FMI2C_SDA_PIN, 0)

#define FMI2C_SCL_INIT   PIN_InitConfig(SOFT_FMI2C_SCL_PIN, PIN_MODE_OUTPUT, 1)
#define FMI2C_SDA_INIT   PIN_InitConfig(SOFT_FMI2C_SDA_PIN, PIN_MODE_OUTPUT, 1)

#define FMI2C_SCL_H      PIN_Write(SOFT_FMI2C_SCL_PIN, 1)
#define FMI2C_SCL_L      PIN_Write(SOFT_FMI2C_SCL_PIN, 0)

#define FMI2C_SDA_H      PIN_Write(SOFT_FMI2C_SDA_PIN, 1)
#define FMI2C_SDA_L      PIN_Write(SOFT_FMI2C_SDA_PIN, 0)

#define FMI2C_SDA_READ   PIN_Read(SOFT_FMI2C_SDA_PIN)


void Test_RDA5807(void);
void RDA5807_Init(void);
void RDA5807_Enable(unsigned char flag);
void RDA5807_SetFreq(unsigned short freq);
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////RDA5807M 底层IIC驱动/////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
void FMI2C_Init(void);
void RDA5807_WriteReg(unsigned char regAddr,unsigned short val);  //芯片写寄存器
unsigned short RDA5807_ReadReg(unsigned char regAddr);			 //芯片读寄存器
#endif
