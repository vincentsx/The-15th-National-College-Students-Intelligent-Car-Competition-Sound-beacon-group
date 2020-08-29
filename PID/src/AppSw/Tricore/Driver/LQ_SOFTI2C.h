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
#ifndef __LQ_SOFTIIC_H
#define __LQ_SOFTIIC_H

#include "Cpu/Std/Platform_Types.h"
#include "stdint.h"
#include "LQ_GPIO.h"

/* 注意 IIC总线规定，IIC空闲时 SCL和SDA都为高电平 最好外部上拉（一定不能下拉） */
/* 模拟 IIC需要注意，IIC地址左移一位 例如MPU6050 模拟就是地址 0xD0 */
/* 想换用别的IO 直接修改宏定义 SOFT_IIC_SCL_PIN 、 SOFT_IIC_SDA_PIN 即可 */
#define SOFT_IIC_SCL_PIN   P13_1   /*!< P13_1  作为 SCL */
#define SOFT_IIC_SDA_PIN   P13_2   /*!< P13_2  作为 SDA */

#define SDA_OUT        PIN_Dir(SOFT_IIC_SDA_PIN, 1);
#define SDA_IN         PIN_Dir(SOFT_IIC_SDA_PIN, 0);

#define IIC_SCL_INIT   PIN_InitConfig(SOFT_IIC_SCL_PIN, PIN_MODE_OUTPUT, 1);
#define IIC_SDA_INIT   PIN_InitConfig(SOFT_IIC_SDA_PIN, PIN_MODE_OUTPUT, 1);

#define IIC_SCL_H      PIN_Write(SOFT_IIC_SCL_PIN, 1);
#define IIC_SCL_L      PIN_Write(SOFT_IIC_SCL_PIN, 0);

#define IIC_SDA_H      PIN_Write(SOFT_IIC_SDA_PIN, 1);
#define IIC_SDA_L      PIN_Write(SOFT_IIC_SDA_PIN, 0);

#define IIC_SDA_READ   PIN_Read(SOFT_IIC_SDA_PIN)

void IIC_Start(void);			    //发送IIC开始信号
void IIC_Stop(void);	  	        //发送IIC停止信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号
uint8 IIC_WaitAck(void); 		    //IIC等待ACK信号
void IIC_SendByte(uint8 data_t);    //IIC发送一个字节
uint8 IIC_ReadByte(uint8 ack);      //IIC读取一个字节


void IIC_Init(void);
uint8 IIC_ReadByteFromSlave(uint8 I2C_Addr,uint8 reg,uint8 *buf);
uint8 IIC_ReadMultByteFromSlave(uint8 dev, uint8 reg, uint8 length, uint8 *data_t);
uint8 IIC_WriteByteToSlave(uint8 I2C_Addr,uint8 reg,uint8 buf);
uint8 IIC_WriteMultByteToSlave(uint8 dev, uint8 reg, uint8 length, uint8* data_t);

#endif
