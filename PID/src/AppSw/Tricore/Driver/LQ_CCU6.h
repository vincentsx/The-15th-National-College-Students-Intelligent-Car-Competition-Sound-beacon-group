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
*  备    注：TC264 有两个CCU6模块  每个模块有两个独立定时器  触发定时器中断
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#ifndef SRC_APPSW_TRICORE_DRIVER_LQ_CCU6_H_
#define SRC_APPSW_TRICORE_DRIVER_LQ_CCU6_H_

#include "Cpu/Std/Platform_Types.h"
#include "stdint.h"
#include <Bsp.h>
#include <IfxCcu6.h>
#include <IfxCcu6_cfg.h>
#include <IfxCcu6_reg.h>
#include <IfxCcu6_regdef.h>
#include <IfxCcu6_Timer.h>
#include <IfxCpu.h>
#include <IfxCpu_Irq.h>
#include <IfxScuCcu.h>



/**
 * 	CCU6模块枚举
 */
typedef enum
{
	CCU60,
	CCU61
}CCU6_t;

/**
 * 	CCU6通道枚举
 */
typedef enum
{
	CCU6_Channel0,
	CCU6_Channel1,
}CCU6_Channel_t;

/** CCU6定时器中断 CCU60  channel0 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  CCU60_CH0_PRIORITY    10

/** CCU6定时器中断 CCU60  channel1 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  CCU60_CH1_PRIORITY    81

/** CCU6定时器中断 CCU60   中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  CCU60_VECTABNUM       0



/** CCU6定时器中断 CCU61  channel0 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  CCU61_CH0_PRIORITY    82

/** CCU6定时器中断 CCU61  channel1 中断服务函数优先级   范围：1-255   数字越大 优先级越高  注意优先级不要重复 */
#define  CCU61_CH1_PRIORITY    83

/** CCU6定时器中断 CCU61  中断归哪个内核管理？ 范围：0：CPU0   1：CPU1   3：DMA*/
#define  CCU61_VECTABNUM       1


/*************************************************************************
*  函数名称：CCU6_InitConfig CCU6
*  功能说明：定时器周期中断初始化
*  参数说明：ccu6    ： ccu6模块            CCU60 、 CCU61
*  参数说明：channel ： ccu6模块通道  CCU6_Channel0 、 CCU6_Channel1
*  参数说明：us      ： ccu6模块  中断周期时间  单位us
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：
*************************************************************************/
void CCU6_InitConfig(CCU6_t ccu6, CCU6_Channel_t channel, uint32 us);

void CCU6_DisableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel);

void CCU6_EnableInterrupt(CCU6_t ccu6, CCU6_Channel_t channel);

#endif /* SRC_APPSW_TRICORE_DRIVER_LQ_CCU6_H_ */
