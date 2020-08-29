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
*  备    注：TC264只有15个固定的GPIO支持外部中断 这15个固定GPIO分为4组，每组只能选择其中一个作为外部中断管脚使用
*           0：P15_4  P33_7  P15_5                             1：P14_3  P15_8
*           2：P10_2  P02_1  P00_4  P20_0  P11_10              3：P10_3  P14_1  P02_0  P20_9  P15_1
*			外部中断服务函数在LQ_GPIO.c中  中断优先级配置在LQ_GPIO.h中 可以自行修改
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef _LQ_GPIO_H_
#define _LQ_GPIO_H_

#include "Cpu/Std/Platform_Types.h"
#include "Port/Std/IfxPort.h"
#include "stdint.h"
#include <IfxCpu.h>
#include <IfxCpu_Irq.h>
#include <IfxPort_regdef.h>
#include <IfxScu_PinMap.h>
#include <IfxScuEru.h>
#include <IfxSrc.h>
#include <IfxSrc_reg.h>
#include <IfxSrc_regdef.h>

// GPIO 枚举端口编号
typedef enum //
{
    P00_0 = 0xA000,  P00_1 = 0xA001,  P00_2 = 0xA002,  P00_3 = 0xA003,  P00_4 = 0xA004,  P00_5 = 0xA005,  P00_6 = 0xA006,  P00_7 = 0xA007,
	P00_8 = 0xA008,  P00_9 = 0xA009,                                    P00_12= 0xA00C,


	P02_0 = 0xA200,  P02_1 = 0xA201,  P02_2 = 0xA202,  P02_3 = 0xA203,  P02_4 = 0xA204,  P02_5 = 0xA205,  P02_6 = 0xA206,  P02_7 = 0xA207,
	P02_8 = 0xA208,


	                 P10_1 = 0xB001,  P10_2 = 0xB002,  P10_3 = 0xB003,                   P10_5 = 0xB005,  P10_6 = 0xB006,


					                  P11_2 = 0xB102,  P11_3 = 0xB103,                                    P11_6 = 0xB106,
					 P11_9 = 0xB109,  P11_10= 0xB10A,  P11_11= 0xB10B,	P11_12= 0xB10C,


	P13_0 = 0xB300,  P13_1 = 0xB301,  P13_2 = 0xB302,  P13_3 = 0xB303,


	P14_0 = 0xB400,  P14_1 = 0xB401,  P14_2 = 0xB402,  P14_3 = 0xB403,  P14_4 = 0xB404,  P14_5 = 0xB405,   P14_6 = 0xB406,


	P15_0 = 0xB500,  P15_1 = 0xB501,  P15_2 = 0xB502,  P15_3 = 0xB503,  P15_4 = 0xB504,  P15_5 = 0xB505,   P15_6 = 0xB506,  P15_7 = 0xB507,
	P15_8 = 0xB508,


	P20_0 = 0xC000,                   P20_2 = 0xC002,  P20_3 = 0xC003,                                     P20_6 = 0xC006,  P20_7 = 0xC007,
	P20_8 = 0xC008,  P20_9 = 0xC009,  P20_10= 0xC00A,  P20_11= 0xC00B,  P20_12= 0xC00C,  P20_13= 0xC00D,   P20_14= 0xC00E,



									  P21_2 = 0xC102,  P21_3 = 0xC103,  P21_4 = 0xC104,  P21_5 = 0xC105,   P21_6 = 0xC106,  P21_7 = 0xC107,


	P22_0 = 0xC200,  P22_1 = 0xC201,  P22_2 = 0xC202,  P22_3 = 0xC203,


	                 P23_1 = 0xC301,


	P32_0 = 0xD200,                                                      P32_4 = 0xD204,



	                                                                     P33_4 = 0xD304,  P33_5 = 0xD305,   P33_6 = 0xD306,  P33_7 = 0xD307,
	P33_8 = 0xD308,  P33_9 = 0xD309,  P33_10= 0xD30A,  P33_11= 0xD30B,   P33_12= 0xD30C,  P33_13= 0xD30D,


}GPIO_Name_t;


/* 获取GPIO对应 MODULE 宏定义L */
#define PIN_GetModule(GPIO_NAME) (Ifx_P*)(0xF0030000u | (GPIO_NAME & 0xFF00))

/* 获取GPIO对应 管脚序列号 宏定义Q */
#define PIN_GetIndex(GPIO_NAME)   (uint8)(GPIO_NAME & 0x000F)



/* GPIO模式 */
#define PIN_MODE_OUTPUT          IfxPort_Mode_outputPushPullGeneral    /*!< 推挽输出  */
#define PIN_MODE_OUTPUT_OD       IfxPort_Mode_outputOpenDrainGeneral   /*!< 开漏输出  */
#define PIN_MODE_INPUT           IfxPort_Mode_inputNoPullDevice        /*!< 浮空输入  */
#define PIN_MODE_INPUT_PULLUP    IfxPort_Mode_inputPullUp              /*!< 上拉输入  */
#define PIN_MODE_INPUT_PULLDOWN  IfxPort_Mode_inputPullDown            /*!< 下拉输入  */


/* GPIO 中断触发模式 */
#define PIN_IRQ_MODE_RISING             IfxPort_InputMode_pullDown     /*!< 上升沿（下拉）触发中断 */
#define PIN_IRQ_MODE_FALLING            IfxPort_InputMode_pullUp       /*!< 下降沿（上拉）触发中断 */
#define PIN_IRQ_MODE_RISING_FALLING     IfxPort_InputMode_noPullDevice /*!< 双边沿（开漏）触发中断 */

/** GPIO外部中断 组0中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define PIN_INT0_PRIORITY    100

/** GPIO外部中断 组0中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define PIN_INT0_VECTABNUM    0

/** GPIO外部中断 组1中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define PIN_INT1_PRIORITY     101

/** GPIO外部中断 组1中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define PIN_INT1_VECTABNUM    0

/** GPIO外部中断 组2中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define PIN_INT2_PRIORITY     40

/** GPIO外部中断 组2中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define PIN_INT2_VECTABNUM    3

/** GPIO外部中断 组3中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define PIN_INT3_PRIORITY     253

/** GPIO外部中断 组3中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define PIN_INT3_VECTABNUM    1

void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, uint8 output);
uint8 PIN_Read(GPIO_Name_t pin);
void PIN_Write(GPIO_Name_t pin, uint8 output);
void PIN_Dir(GPIO_Name_t pin, uint8 mode);
void PIN_Reverse(GPIO_Name_t pin);
void PIN_Exti(GPIO_Name_t pin, IfxPort_InputMode mode);
void PIN_ExtiEnable(GPIO_Name_t pin,  boolean bool);

#endif /* 0_APPSW_TRICORE_APP_LQ_GPIO_H_ */
