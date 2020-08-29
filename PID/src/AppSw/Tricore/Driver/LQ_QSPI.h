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

基于iLLD_1_0_1_11_0底层程序   TC264有4个QSPI
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef _LQ_QSPI_H_
#define _LQ_QSPI_H_

#include <Platform_Types.h>
#include <stdint.h>
#include <SpiIf.h>
#include <Ifx_Types.h>
#include <IfxCpu_Irq.h>
#include <IfxPort.h>
#include <IfxQspi.h>
#include <IfxQspi_cfg.h>
#include <IfxQspi_PinMap.h>
#include <IfxQspi_SpiMaster.h>
#include <IfxSrc_cfg.h>

/***********************************************************************************************/
/***********************************************************************************************/
/*****************    QSPI中断优先级 和 指向内核配置 用户可以自行修改*******************************/
/***********************************************************************************************/
/***********************************************************************************************/
/** QSPI0接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  QSPI0_RX_PRIORITY    200

/** QSPI0发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  QSPI0_TX_PRIORITY    201

/** QSPI0错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  QSPI0_ER_PRIORITY    202

/** QSPI0中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  QSPI0_VECTABNUM    0


/** QSPI1接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  QSPI1_RX_PRIORITY    203

/** QSPI1发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  QSPI1_TX_PRIORITY    204

/** QSPI1错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  QSPI1_ER_PRIORITY    205

/** QSPI1中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  QSPI1_VECTABNUM    0


/** QSPI2接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  QSPI2_RX_PRIORITY    206

/** QSPI2发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  QSPI2_TX_PRIORITY    207

/** QSPI2错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  QSPI2_ER_PRIORITY    208

/** QSPI2中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  QSPI2_VECTABNUM    0


/** QSPI3接收中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  QSPI3_RX_PRIORITY    209

/** QSPI3发送中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  QSPI3_TX_PRIORITY    210

/** QSPI3错误中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  QSPI3_ER_PRIORITY    211

/** QSPI3中断服务函数优先级 中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  QSPI3_VECTABNUM    0

/**
 * QSPI CLK管脚枚举
 */
typedef enum
{
	QSPI0_CLK_P20_11= 0x0000, QSPI0_CLK_P20_13= 0x0001,
	QSPI1_CLK_P10_2 = 0x0100, QSPI1_CLK_P11_6 = 0x0101,
	QSPI2_CLK_P15_3 = 0x0202, QSPI2_CLK_P15_6 = 0x0203, QSPI2_CLK_P15_8 = 0x0204,
	QSPI3_CLK_P02_7 = 0x0300, QSPI3_CLK_P22_3 = 0x0304, QSPI3_CLK_P33_11= 0x0305,
	QSPI_CLK_NULL   = 0x0003
}QSPI_CLK_t;

/**
 * QSPI MISO/MRST 管脚枚举
 */
typedef enum
{
	QSPI0_MISO_P20_12= 0x0000,
	QSPI1_MISO_P10_1 = 0x0100, QSPI1_MISO_P11_3 = 0x0101,
	QSPI2_MISO_P15_4 = 0x0200, QSPI2_MISO_P15_7 = 0x0201,   QSPI2_MISO_P15_2  = 0x0204,
	QSPI3_MISO_P02_5 = 0x0300, QSPI3_MISO_P33_13 = 0x0303,  QSPI3_MISO_P22_1  = 0x0304,
	QSPI_MISO_NULL   = 0x0003
}QSPI_MISO_t;

/**
 * QSPI MOSI/MTSR 管脚枚举
 */
typedef enum
{
	QSPI0_MOSI_P20_12= 0x0000, QSPI0_MOSI_P20_14= 0x0001,
	QSPI1_MOSI_P10_1 = 0x0100, QSPI1_MOSI_P10_3 = 0x0101, QSPI1_MOSI_P11_9 = 0x0103,
	QSPI2_MOSI_P15_5 = 0x0202, QSPI2_MOSI_P15_6 = 0x0203,
	QSPI3_MOSI_P02_6 = 0x0300, QSPI3_MOSI_PP10_6= 0x0301, QSPI3_MOSI_P22_0 = 0x0302,                                                    QSPI3_MOSI_P33_12= 0x0305,
	QSPI_MOSI_NULL   = 0x0003
}QSPI_MOSI_t;

/**
 * QSPI 片选管脚枚举
 */
typedef enum
{
	QSPI0_CS_P20_8 = 0x0000,  QSPI0_CS_P20_9 = 0x0001, QSPI0_CS_P20_13= 0x0002, QSPI0_CS_P11_10= 0x0003, QSPI0_CS_P11_11= 0x0004, QSPI0_CS_P11_2 = 0x0005, QSPI0_CS_P20_10= 0x0006, QSPI0_CS_P33_5 = 0x0007, QSPI0_CS_P20_6 = 0x0008, QSPI0_CS_P20_3 = 0x0009, QSPI0_CS_P15_0 = 0x000D,
	QSPI1_CS_P20_8 = 0x0100,  QSPI1_CS_P20_9 = 0x0101, QSPI1_CS_P20_13= 0x0102, QSPI1_CS_P11_10= 0x0103, QSPI1_CS_P11_11= 0x0104, QSPI1_CS_P11_2 = 0x0105, QSPI1_CS_P33_10= 0x0106, QSPI1_CS_P33_5 = 0x0107, QSPI1_CS_P10_4 = 0x0108, QSPI1_CS_P10_5 = 0x0109, QSPI1_CS_P10_0 = 0x010A,
	QSPI2_CS_P15_2 = 0x0200,  QSPI2_CS_P14_2 = 0x0201, QSPI2_CS_P14_6 = 0x0202, QSPI2_CS_P14_3 = 0x0203, QSPI2_CS_P15_1 = 0x0205, QSPI2_CS_P33_13= 0x0206, QSPI2_CS_P20_10= 0x0207, QSPI2_CS_P20_6 = 0x0208, QSPI2_CS_P20_3 = 0x0209,
	QSPI3_CS_P02_4 = 0x0300,  QSPI3_CS_P02_0 = 0x0301, QSPI3_CS_P02_1 = 0x0302, QSPI3_CS_P02_2 = 0x0303, QSPI3_CS_P02_3 = 0x0304, QSPI3_CS_P02_8 = 0x0305, QSPI3_CS_P00_8 = 0x0306, QSPI3_CS_P00_9 = 0x0307, QSPI3_CS_P10_5 = 0x0308, QSPI3_CS_P33_10= 0x030B, QSPI3_CS_P22_2= 0x030C, QSPI3_CS_P23_1= 0x030D,
}QSPI_CS_t;

/**
 * QSPI 枚举
 */
typedef enum
{
	QSPI0,
	QSPI1,
	QSPI2,
	QSPI3
}QSPI_t;


#define QSPI_GetModule(pin)    (uint8)(pin >> 8)
#define QSPI_GetIndex(pin)    (uint8)(pin & 0x0f)

void QSPI_InitConfig(QSPI_CLK_t clkPin, QSPI_MISO_t misoPin, QSPI_MOSI_t mosiPin, QSPI_CS_t cQSPIn, uint32_t baudrate, uint8_t mode);
uint8 QSPI_ReadWriteNByte(QSPI_t QSPI, uint8_t *txData, uint8_t *rxData, uint16 len);



#endif /* QSPI_DEMO_H_ */
