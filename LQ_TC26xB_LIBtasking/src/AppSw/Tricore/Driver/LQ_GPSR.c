/*!
  * @file     LQ_GPSR.c
  *
  * @brief    每个CPU有4个软件中断
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
  * @date     2020年5月6日
  */ 

#include <IfxInt_reg.h>
#include <IfxCpu_Irq.h>
#include <IfxSrc_reg.h>
#include <LQ_GPSR.h>
#include <Platform_Types.h>

/** cpu软件中断优先级 */
const uint8 CpuSoftIrqPriority[8] = {CPU0_SOFT_IRQ0_PRIORITY, CPU0_SOFT_IRQ1_PRIORITY, CPU0_SOFT_IRQ2_PRIORITY, CPU0_SOFT_IRQ3_PRIORITY,
		CPU1_SOFT_IRQ0_PRIORITY,CPU1_SOFT_IRQ1_PRIORITY, CPU1_SOFT_IRQ2_PRIORITY,CPU1_SOFT_IRQ3_PRIORITY};





/**
  * @brief    CPU0_SOFT_IRQ0Handler中断服务函数
  * @note     cpu0 软件中断0中断服务函数
  */
void CPU0_SOFT_IRQ0Handler(void)
{
	SRC_GPSR00.B.SRR = 0;
	/* 用户代码 */
}

void CPU0_SOFT_IRQ1Handler(void)
{
	SRC_GPSR01.B.SRR = 0;
	/* 用户代码 */
}

void CPU0_SOFT_IRQ2Handler(void)
{
	SRC_GPSR02.B.SRR = 0;
	/* 用户代码 */
}

void CPU0_SOFT_IRQ3Handler(void)
{
	SRC_GPSR03.B.SRR = 0;
	/* 用户代码 */
}

void CPU1_SOFT_IRQ0Handler(void)
{
	SRC_GPSR10.B.SRR = 0;
	/* 用户代码 */
}

void CPU1_SOFT_IRQ1Handler(void)
{
	SRC_GPSR11.B.SRR = 0;
	/* 用户代码 */
}

void CPU1_SOFT_IRQ2Handler(void)
{
	SRC_GPSR12.B.SRR = 0;
	/* 用户代码 */
}

void CPU1_SOFT_IRQ3Handler(void)
{
	SRC_GPSR13.B.SRR = 0;
	/* 用户代码 */
}



/* cpu软中断 */
IFX_INTERRUPT(CPU0_SOFT_IRQ0Handler, 0, CPU0_SOFT_IRQ0_PRIORITY);
IFX_INTERRUPT(CPU0_SOFT_IRQ1Handler, 0, CPU0_SOFT_IRQ1_PRIORITY);
IFX_INTERRUPT(CPU0_SOFT_IRQ2Handler, 0, CPU0_SOFT_IRQ2_PRIORITY);
IFX_INTERRUPT(CPU0_SOFT_IRQ3Handler, 0, CPU0_SOFT_IRQ3_PRIORITY);
IFX_INTERRUPT(CPU1_SOFT_IRQ0Handler, 1, CPU1_SOFT_IRQ0_PRIORITY);
IFX_INTERRUPT(CPU1_SOFT_IRQ1Handler, 1, CPU1_SOFT_IRQ1_PRIORITY);
IFX_INTERRUPT(CPU1_SOFT_IRQ2Handler, 1, CPU1_SOFT_IRQ2_PRIORITY);
IFX_INTERRUPT(CPU1_SOFT_IRQ3Handler, 1, CPU1_SOFT_IRQ3_PRIORITY);


/** cpu软中断服务函数地址 */
const void *CpuSoftIrqFuncPointer[8] = {&CPU0_SOFT_IRQ0Handler, &CPU0_SOFT_IRQ1Handler, &CPU0_SOFT_IRQ2Handler,
									&CPU0_SOFT_IRQ3Handler, &CPU1_SOFT_IRQ0Handler, &CPU1_SOFT_IRQ1Handler,
									&CPU1_SOFT_IRQ2Handler, &CPU1_SOFT_IRQ3Handler};


/*************************************************************************
*  函数名称：GPSR_InitConfig(IfxSrc_Tos cpu, uint8 softIrq)
*  功能说明：软件中的初始化
*  参数说明：cpu     ： IfxSrc_Tos_cpu0   IfxSrc_Tos_cpu1
*  参数说明：softIrq ： 每个CPU有4个软件中断, irq序号
*  函数返回：无
*  修改时间：2020年5月6日
*  备    注：
*************************************************************************/
void GPSR_InitConfig(IfxSrc_Tos cpu, SOFT_IRQ index)
{
	/* 关闭中断 */
	boolean        interruptState = IfxCpu_disableInterrupts();

	uint8 temp = cpu * 4 + index;
	SRC_GPSR00.B.SRR = 0;
	SRC_GPSR01.B.SRR = 0;
	SRC_GPSR02.B.SRR = 0;
	SRC_GPSR03.B.SRR = 0;

	SRC_GPSR10.B.SRR = 0;
	SRC_GPSR11.B.SRR = 0;
	SRC_GPSR12.B.SRR = 0;
	SRC_GPSR13.B.SRR = 0;

	/* 开启中断 */
	volatile Ifx_SRC_SRCR *src = &MODULE_SRC.GPSR.GPSR[(int)cpu].SR0;
	IfxSrc_init(src, (int)cpu, (uint32)CpuSoftIrqPriority[temp]);
	IfxSrc_enable(src);

	src = &MODULE_SRC.GPSR.GPSR[(int)cpu].SR1;
	IfxSrc_init(src, (int)cpu, (uint32)CpuSoftIrqPriority[temp]);
	IfxSrc_enable(src);

	src = &MODULE_SRC.GPSR.GPSR[(int)cpu].SR2;
	IfxSrc_init(src, (int)cpu, (uint32)CpuSoftIrqPriority[temp]);
	IfxSrc_enable(src);

	src = &MODULE_SRC.GPSR.GPSR[(int)cpu].SR3;
	IfxSrc_init(src, (int)cpu, (uint32)CpuSoftIrqPriority[temp]);
	IfxSrc_enable(src);

	/* 安装中断服务函数 */
	IfxCpu_Irq_installInterruptHandler((void*)CpuSoftIrqFuncPointer[temp],     (uint32)CpuSoftIrqPriority[temp]);

	/* 开启中断 */
	IfxCpu_restoreInterrupts(interruptState);
}

/*************************************************************************
*  函数名称：CPSR_Trig(IfxSrc_Tos cpu, uint8 softIrq)
*  功能说明：触发软件中断
*  参数说明：cpu     ： IfxSrc_Tos_cpu0   IfxSrc_Tos_cpu1
*  参数说明：softIrq ： 每个CPU有4个软件中断, irq序号
*  函数返回：无
*  修改时间：2020年5月6日
*  备    注：
*************************************************************************/
void CPSR_Trig(IfxSrc_Tos cpu, SOFT_IRQ index)
{
	if(cpu == IfxSrc_Tos_cpu0)
	{
		INT_SRB0.U = 1 << (uint8)index;
	}
	else
	{
		INT_SRB1.U = 1 << (uint8)index;
	}
}










