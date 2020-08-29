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

基于iLLD_1_0_1_11_0底层程序
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef _LQ_SPI_H_
#define _LQ_SPI_H_

#include "Cpu/Std/Platform_Types.h"
#include "stdint.h"
#include <IfxAsclin.h>
#include <IfxAsclin_PinMap.h>
#include <IfxAsclin_Spi.h>
#include <IfxCpu.h>
#include <IfxCpu_Irq.h>
#include <IfxPort.h>
#include <IfxSrc_cfg.h>


/** SPI0接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  SPI0_RX_PRIORITY    160

/** SPI0发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  SPI0_TX_PRIORITY    161

/** SPI0错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  SPI0_ER_PRIORITY    162

/** SPI0中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  SPI0_VECTABNUM    0


/** SPI1接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  SPI1_RX_PRIORITY    170

/** SPI1发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  SPI1_TX_PRIORITY    171

/** SPI1错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  SPI1_ER_PRIORITY    172

/** SPI1中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  SPI1_VECTABNUM    0


/** SPI2接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  SPI2_RX_PRIORITY    180

/** SPI2发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  SPI2_TX_PRIORITY    181

/** SPI2错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  SPI2_ER_PRIORITY    182

/** SPI2中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  SPI2_VECTABNUM    0


/** SPI3接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  SPI3_RX_PRIORITY    190

/** SPI3发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  SPI3_TX_PRIORITY    191

/** SPI3错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  SPI3_ER_PRIORITY    192

/** SPI3中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  SPI3_VECTABNUM    0

/**
 * SPI CLK管脚枚举
 */
typedef enum
{
	SPI0_CLK_P14_0 = 0x0000, SPI0_CLK_P15_2 = 0x0001,
	SPI1_CLK_P15_0 = 0x0100, SPI1_CLK_P20_10= 0x0101, SPI1_CLK_P33_11= 0x0102, SPI1_CLK_P33_12= 0x0103,
	SPI2_CLK_P02_4 = 0x0200, SPI2_CLK_P10_6 = 0x0201, SPI2_CLK_P14_2 = 0x0202, SPI2_CLK_P33_7 = 0x0203, SPI2_CLK_P33_9 = 0x0204,
	SPI3_CLK_P00_0 = 0x0300, SPI3_CLK_P00_2 = 0x0301, SPI3_CLK_P15_6 = 0x0302, SPI3_CLK_P15_8 = 0x0303, SPI3_CLK_P20_0 = 0x0304, SPI3_CLK_P21_5 = 0x0305,
}SPI_CLK_t;

/**
 * SPI MISO/MRST 管脚枚举
 */
typedef enum
{
	SPI0_MISO_P14_1 = 0x0000, SPI0_MISO_P15_3 = 0x0001,
	SPI1_MISO_P15_1 = 0x0100, SPI1_MISO_P15_5 = 0x0101, SPI1_MISO_P20_9 = 0x0102,SPI1_MISO_P11_10 = 0x0104, SPI1_MISO_P33_13 = 0x0105, SPI1_MISO_P02_3 = 0x0106,
	SPI2_MISO_P14_3 = 0x0200, SPI2_MISO_P02_1 = 0x0201, SPI2_MISO_P10_6 = 0x0203, SPI2_MISO_P33_8  = 0x0204,SPI2_MISO_P02_0 = 0x0206,
	SPI3_MISO_P15_7 = 0x0300, SPI3_MISO_P20_3 = 0x0302,SPI3_MISO_P00_1  = 0x0304, SPI3_MISO_P21_6  = 0x0305,
	SPI3_MISO_NULL  = 0x0004
}SPI_MISO_t;

/**
 * SPI MOSI/MTSR 管脚枚举
 */
typedef enum
{
	SPI0_MOSI_P14_0 = 0x0000, SPI0_MOSI_P14_1 = 0x0001, SPI0_MOSI_P15_2 = 0x0002, SPI0_MOSI_P15_3 = 0x0003,
	SPI1_MOSI_P02_2 = 0x0100, SPI1_MOSI_P11_12= 0x0101, SPI1_MOSI_P14_10= 0x0102, SPI1_MOSI_P15_0 = 0x0103, SPI1_MOSI_P15_1 = 0x0104, SPI1_MOSI_P15_4 = 0x0105, SPI1_MOSI_P15_5 = 0x0106, SPI1_MOSI_P20_10= 0x0107, SPI1_MOSI_P33_12 = 0x0108,
	SPI2_MOSI_P10_5 = 0x0200, SPI2_MOSI_P14_2 = 0x0201, SPI2_MOSI_P14_3 = 0x0202, SPI2_MOSI_P33_8 = 0x0203, SPI2_MOSI_P33_9 = 0x0204,
	SPI3_MOSI_P00_0 = 0x0300, SPI3_MOSI_P00_1 = 0x0301, SPI3_MOSI_P15_6 = 0x0302, SPI3_MOSI_P15_7 = 0x0303, SPI3_MOSI_P20_0 = 0x0304, SPI3_MOSI_P20_3 = 0x0305,
	SPI3_MOSI_NULL  = 0x0004
}SPI_MOSI_t;

/**
 * SPI 片选管脚枚举
 */
typedef enum
{
	SPI0_CS_Null  = 0x0000,
	SPI1_CS_P14_3 = 0x0100,  SPI1_CS_P20_8 = 0x0101, SPI1_CS_P33_10= 0x0102,
	SPI2_CS_P02_3 = 0x0200,  SPI2_CS_P10_5 = 0x0201, SPI2_CS_P33_6 = 0x0202,
	SPI3_CS_P00_3 = 0x0300,  SPI3_CS_P14_3 = 0x0301, SPI3_CS_P21_2 = 0x0302, SPI3_CS_P21_6 = 0x0303,
}SPI_CS_t;

/**
 * SPI 枚举
 */
typedef enum
{
	SPI0,
	SPI1,
	SPI2,
	SPI3
}SPI_t;


#define SPI_GetModule(pin)    (uint8)(pin >> 8)
#define SPI_GetIndex(pin)    (uint8)(pin & 0x0f)

void SPI_InitConfig(SPI_CLK_t clkPin, SPI_MISO_t misoPin, SPI_MOSI_t mosiPin, SPI_CS_t csPin, uint32_t baudrate);
uint8 SPI_ReadWriteNByte(SPI_t spi, uint8_t *txData, uint8_t *rxData, uint32_t len);



#endif /* QSPI_DEMO_H_ */
