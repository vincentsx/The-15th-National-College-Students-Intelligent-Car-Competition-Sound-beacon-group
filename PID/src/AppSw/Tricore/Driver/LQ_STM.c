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
STM作为系统级的全局定时器，可以为系统提供基础时钟，内有两个比较寄存器，
可产生两个独立的中断，以满足需求。
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/


#include <LQ_GPIO_LED.h>
#include <LQ_STM.h>


/*************************************************************************
*  函数名称：
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：STM中断
*************************************************************************/
IFX_INTERRUPT(STM0_CH0_IRQHandler, STM0_VECTABNUM, STM0_CH0_PRIORITY);
IFX_INTERRUPT(STM0_CH1_IRQHandler, STM0_VECTABNUM, STM0_CH1_PRIORITY);
IFX_INTERRUPT(STM1_CH0_IRQHandler, STM1_VECTABNUM, STM1_CH0_PRIORITY);
IFX_INTERRUPT(STM1_CH1_IRQHandler, STM1_VECTABNUM, STM1_CH1_PRIORITY);

/** 外部中断CPU标号 */
const uint8 StmIrqVectabNum[2]  = {STM0_VECTABNUM, STM1_VECTABNUM};

/** 外部中断优先级 */
const uint8 StmIrqPriority[4]   = {STM0_CH0_PRIORITY, STM0_CH1_PRIORITY, STM1_CH0_PRIORITY, STM1_CH1_PRIORITY};

/** 外部中断服务函数地址 */
const void *StmIrqFuncPointer[4] = {&STM0_CH0_IRQHandler, &STM0_CH1_IRQHandler, &STM1_CH0_IRQHandler, &STM1_CH1_IRQHandler};


//	STM初始化配置结构体
IfxStm_CompareConfig g_StmCompareConfig[4];

/***********************************************************************************************/
/********************************STM外部中断  服务函数******************************************/
/***********************************************************************************************/

/*************************************************************************
*  函数名称：void STM0_CH0_IRQHandler(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：STM0_CH0使用的中断服务函数
*************************************************************************/
void STM0_CH0_IRQHandler(void)
{
	/* 开启CPU中断  否则中断不可嵌套 */
	IfxCpu_enableInterrupts();

	//清除中断标志
	IfxStm_clearCompareFlag(&MODULE_STM0, g_StmCompareConfig[0].comparator);

	//开启新的中断配置，开始下次中断
	IfxStm_increaseCompare(&MODULE_STM0, g_StmCompareConfig[0].comparator, g_StmCompareConfig[0].ticks);

	/* 用户代码 */
	LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
}

/*************************************************************************
*  函数名称：void STM0_CH1_IRQHandler(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：STM0_CH1使用的中断服务函数
*************************************************************************/
void STM0_CH1_IRQHandler(void)
{
	/* 开启CPU中断  否则中断不可嵌套 */
	IfxCpu_enableInterrupts();

	//清除中断标志
	IfxStm_clearCompareFlag(&MODULE_STM0, g_StmCompareConfig[1].comparator);

	//开启新的中断配置，开始下次中断
	IfxStm_increaseCompare(&MODULE_STM0, g_StmCompareConfig[1].comparator, g_StmCompareConfig[1].ticks);

	/* 用户代码 */
	LED_Ctrl(LED1,RVS);        //电平翻转,LED闪烁

}

/*************************************************************************
*  函数名称：void STM1_CH0_IRQHandler(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：STM1_CH0使用的中断服务函数
*************************************************************************/
void STM1_CH0_IRQHandler(void)
{
	/* 开启CPU中断  否则中断不可嵌套 */
	IfxCpu_enableInterrupts();

	//清除中断标志
	IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[2].comparator);

	//开启新的中断配置，开始下次中断
	IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[2].comparator, g_StmCompareConfig[2].ticks);

	/* 用户代码 */
	LED_Ctrl(LED1,RVS);        //电平翻转,LED闪烁

}

/*************************************************************************
*  函数名称：void STM1_CH1_IRQHandler(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：STM1_CH1使用的中断服务函数
*************************************************************************/
void STM1_CH1_IRQHandler(void)
{
	/* 开启CPU中断  否则中断不可嵌套 */
	IfxCpu_enableInterrupts();

	//清除中断标志
	IfxStm_clearCompareFlag(&MODULE_STM1, g_StmCompareConfig[3].comparator);

	//开启新的中断配置，开始下次中断
	IfxStm_increaseCompare(&MODULE_STM1, g_StmCompareConfig[3].comparator, g_StmCompareConfig[3].ticks);

	/* 用户代码 */

}


 /*************************************************************************
 *  函数名称：void STM_Init(STM_t stm, STM_Channel_t channel, uint32 us)
 *  功能说明：STM初始化
 *  参数说明：
 * @param    stm      ： STM标号     STM0 、 STM1
 * @param    channel  ： STM通道     STM_Channel_0  STM_Channel_1
 * @param    us       ： STM定时中断 时间
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  举    例：STM_InitConfig(STM0, STM_Channel_0, 50000);  //STM0 通道0初始化 50000us 进入一次中断
 *  备    注：外部中断服务函数在LQ_STM.c中  中断优先级配置在LQ_STM.h中 可以自行修改
 *************************************************************************/
 void STM_InitConfig(STM_t stm, STM_Channel_t channel, uint32 us)
 {
 	char      interruptState;
 	interruptState = disableInterrupts();

 	Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)stm);
 	uint8_t Index = (uint8_t)IfxStm_getIndex(STM) * 2 + (uint8_t)channel;

 	IfxStm_CompareConfig *stmCompareConfig = &g_StmCompareConfig[Index];

 	//调用结构体实现配置
 	IfxStm_initCompareConfig(stmCompareConfig);

     //挂起
 	IfxStm_enableOcdsSuspend(STM);

 	//获取STM模块的微秒计时 tick 数值
 	sint32 ticks = IfxStm_getTicksFromMicroseconds((Ifx_STM *)STM, us);

 	//修改ticks数值，使能触发输出
 	stmCompareConfig->ticks = ticks;
 	stmCompareConfig->comparator           = (IfxStm_Comparator)channel;
 	stmCompareConfig->comparatorInterrupt  = (IfxStm_ComparatorInterrupt)channel; //中断选择
 	stmCompareConfig->triggerPriority      = StmIrqPriority[Index];               //中断优先级设置
 	stmCompareConfig->typeOfService        = StmIrqVectabNum[stm];                //服务内核

 	//比较功能初始化
 	IfxStm_initCompare(STM, stmCompareConfig);
 	IfxCpu_Irq_installInterruptHandler((void*)StmIrqFuncPointer[Index], StmIrqPriority[Index]);//配置中断函数和中断号

 	restoreInterrupts(interruptState);
 }


 /*************************************************************************
 *  函数名称：void STM_DelayUs(STM_t stm, uint32 us)
 *  功能说明：STM延时
 *  参数说明：
 * @param    stm      ： STM标号     STM0 、 STM1
 * @param    us       ： STM延时     时间 us
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  举    例：STM_DelayUs(STM0, 5000);  //延时5000us
 *  备    注：使用延时函数时，请确保对应STM模块调用过STM_Init函数
 *************************************************************************/
 void STM_DelayUs(STM_t stm, uint32 us)
 {
 	Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)stm);
 	uint32 tick = IfxStm_getTicksFromMicroseconds(STM, us);

 	IfxStm_waitTicks(STM, tick);

 }

 /*************************************************************************
 *  函数名称：void delayms(unsigned short stmms)
 *  功能说明：延时函数
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  备    注：delayms(1);延时1 ms
 *************************************************************************/
 void delayms(unsigned short stmms)	//
 {
#if 0
	while(stmms--)
	{
	     STM_DelayUs(STM0, 1000);
	}
#else   //不精确延时
	volatile uint32_t i = 0;
	while(stmms--)
	{
		for (i = 0; i < 16580; ++i)
		{
			__asm("NOP"); /* delay */
		}
	}
#endif
 }

 /*************************************************************************
 *  函数名称：void delayus(unsigned short us)
 *  功能说明：延时函数
 *  参数说明：无
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  备    注：delayus(1);延时1 us
 *************************************************************************/
 void delayus(unsigned short us)
 {
#if 0

	 STM_DelayUs(STM0, us);

#else   //不精确延时
	volatile uint32_t i = 0;
	while(us--)
	{
		for (i = 0; i < 16; ++i)
		{
			__asm("NOP"); /* delay */
		}
	}
#endif

 }
 /*************************************************************************
 *  函数名称：uint32  STM_GetNowUs(STM_t stm)
 *  功能说明：STM获取当前时间
 *  参数说明：
 * @param    stm      ： STM标号     STM0 、 STM1
 * @param    us       ： STM延时     时间 us
 *  函数返回：无
 *  修改时间：2020年3月10日
 *  举    例：int32_t NowTime =  STM_GetNowUs(STM0);  //获取STM0 当前时间
 *  备    注：使用延时函数时，请确保对应STM模块调用过STM_Init函数
 *************************************************************************/
 uint32  STM_GetNowUs(STM_t stm)
 {
 	Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)stm);
 	sint32 freq = (sint32)IfxStm_getFrequency(STM)/1000000;
 	Ifx_TickTime stmNow;
 	char      interruptState;

 	interruptState = disableInterrupts();
 	stmNow         = (Ifx_TickTime)IfxStm_get(STM) & TIME_INFINITE;
 	restoreInterrupts(interruptState);

 	return (uint32_t)(stmNow/freq);
 }


 /*************************************************************************
 *  函数名称：STM_DisableInterrupt(STM_t stm, STM_Channel_t channel)
 *  功能说明：STM停止中断
 *  参数说明：
 * @param    stm      ： STM标号     STM0 、 STM1
 * @param    channel  ： STM通道     STM_Channel_0  STM_Channel_1
 *  函数返回：无
 *  修改时间：2020年5月7日
 *  举    例：
 *  备    注：
 *************************************************************************/
void STM_DisableInterrupt(STM_t stm, STM_Channel_t channel)
{
	Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)stm);
	IfxStm_clearCompareFlag(STM, (IfxStm_Comparator)channel);
	IfxStm_disableComparatorInterrupt(STM, (IfxStm_Comparator)channel);
}


/*************************************************************************
*  函数名称：STM_EnableInterrupt(STM_t stm, STM_Channel_t channel)
*  功能说明：STM使能中断
*  参数说明：
* @param    stm      ： STM标号     STM0 、 STM1
* @param    channel  ： STM通道     STM_Channel_0  STM_Channel_1
*  函数返回：无
*  修改时间：2020年5月7日
*  举    例：
*  备    注：
*************************************************************************/
void STM_EnableInterrupt(STM_t stm, STM_Channel_t channel)
{
	Ifx_STM * STM = IfxStm_getAddress((IfxStm_Index)stm);
	IfxStm_clearCompareFlag(STM, (IfxStm_Comparator)channel);
	IfxStm_enableComparatorInterrupt(STM, (IfxStm_Comparator)channel);
}




