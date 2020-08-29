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



#include <LQ_DMA.h>
#include <LQ_GPIO.h>
#include <LQ_GPIO_LED.h>

/* GPIO外部中断 */
IFX_INTERRUPT(PIN_INT0_IRQHandler, PIN_INT0_VECTABNUM, PIN_INT0_PRIORITY);
IFX_INTERRUPT(PIN_INT1_IRQHandler, PIN_INT1_VECTABNUM, PIN_INT1_PRIORITY);
IFX_INTERRUPT(PIN_INT2_IRQHandler, PIN_INT2_VECTABNUM, PIN_INT2_PRIORITY);
IFX_INTERRUPT(PIN_INT3_IRQHandler, PIN_INT3_VECTABNUM, PIN_INT3_PRIORITY);


/** 外部中断CPU标号 */
const uint8 PinIrqVectabNum[4]  = {PIN_INT0_VECTABNUM, PIN_INT1_VECTABNUM, PIN_INT2_VECTABNUM, PIN_INT3_VECTABNUM};

/** 外部中断优先级 */
const uint8 PinIrqPriority[4]   = {PIN_INT0_PRIORITY, PIN_INT1_PRIORITY, PIN_INT2_PRIORITY, PIN_INT3_PRIORITY};

/** 外部中断服务函数地址 */
const void *PinIrqFuncPointer[4] = {&PIN_INT0_IRQHandler, &PIN_INT1_IRQHandler, &PIN_INT2_IRQHandler, &PIN_INT3_IRQHandler};


/*************************************************************************
*  函数名称：PIN_INT0_IRQHandler中断服务函数
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：外部中断0组管脚 使用的中断服务函数
*************************************************************************/
void PIN_INT0_IRQHandler(void)
{

	/* 用户代码 */
	LED_Ctrl(LED1,RVS);        //电平翻转,LED闪烁
}

/*************************************************************************
*  函数名称：PIN_INT1_IRQHandler中断服务函数
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：外部中断1组管脚 使用的中断服务函数
*************************************************************************/
void PIN_INT1_IRQHandler(void)
{

	/* 用户代码 */
	LED_Ctrl(LED1,RVS);        //电平翻转,LED闪烁
}

/*************************************************************************
*  函数名称：PIN_INT2_IRQHandler中断服务函数
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：外部中断2组管脚 使用的中断服务函数
*************************************************************************/
void PIN_INT2_IRQHandler(void)
{

	// 用户代码
	LED_Ctrl(LED1,RVS);        //电平翻转,LED闪烁
}

/*************************************************************************
*  函数名称：PIN_INT3_IRQHandler中断服务函数
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：外部中断3组管脚 使用的中断服务函数
*************************************************************************/
void PIN_INT3_IRQHandler(void)
{

	// 用户代码
	DMA_CameraStart(PIN_INT2_PRIORITY);

}


/*************************************************************************
*  函数名称：void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, uint8 output)
*  功能说明：GPIO初始化函数
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
*            mode     ： GPIO 模式   lq_gpio.h中的宏定义中选择
*            output   ： 输出模式时，输出电平 1：高电平  0：低电平
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例： PIN_InitConfig(P00_0, PIN_MODE_OUTPUT, 1);  //P00_0初始化推挽输出 高电平
*************************************************************************/
void PIN_InitConfig(GPIO_Name_t pin, IfxPort_Mode mode, uint8 output)
{

	Ifx_P *port = PIN_GetModule(pin);
	uint8 pinIndex = PIN_GetIndex(pin);

	/* 配置 GPIO模式 */
	IfxPort_setPinMode(port, pinIndex,  mode);

	IfxPort_setPinPadDriver(port,  pinIndex,  IfxPort_PadDriver_cmosAutomotiveSpeed2);


	/* GPIO输出模式时 输出状态 */
	if(0 == output)
	{
		IfxPort_setPinState(port, pinIndex, IfxPort_State_low);
	}
	else
	{
		IfxPort_setPinState(port, pinIndex, IfxPort_State_high);
	}
}

/*************************************************************************
*  函数名称：uint8 PIN_Read(GPIO_Name_t pin)
*  功能说明：读取GPIO电平函数
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：uint8_t pinState = PIN_Read(P00_0);  //读取P00_0 的电平状态
*************************************************************************/
uint8 PIN_Read(GPIO_Name_t pin)
{
	Ifx_P *port = PIN_GetModule(pin);
	uint8 pinIndex = PIN_GetIndex(pin);
	return IfxPort_getPinState(port, pinIndex);
}

/*************************************************************************
*  函数名称：void PIN_Write(GPIO_Name_t pin, uint8 output)
*  功能说明：设置GPIO输出状态
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
*            output   ： 输出模式时，输出电平 1：高电平  0：低电平
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：PIN_Write(P00_0, 1);//P00_0置为高电平
*************************************************************************/
void PIN_Write(GPIO_Name_t pin, uint8 output)
{
	Ifx_P *port = PIN_GetModule(pin);
	uint8 pinIndex = PIN_GetIndex(pin);

	/* GPIO输出模式时 输出状态 */
	if(0 == output)
	{
		IfxPort_setPinState(port, pinIndex, IfxPort_State_low);
	}
	else
	{
		IfxPort_setPinState(port, pinIndex, IfxPort_State_high);
	}
}

/*************************************************************************
*  函数名称：void PIN_Dir(GPIO_Name_t pin, uint8 mode)
*  功能说明：设置GPIO 输入输出方向
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
*            mode     ： GPIO 模式   lq_gpio.h中的宏定义中选择
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：PIN_Dir(P00_0, 1);//P00_0设置为输出
*************************************************************************/
void PIN_Dir(GPIO_Name_t pin, uint8 mode)
{
	Ifx_P *port = PIN_GetModule(pin);
	uint8 pinIndex = PIN_GetIndex(pin);

    if(0 == mode)
    {
    	/* 配置 GPIO模式 */
    	IfxPort_setPinMode(port, pinIndex,  PIN_MODE_INPUT);
    }
    else
    {
    	/* 配置 GPIO模式 */
    	IfxPort_setPinMode(port, pinIndex,  PIN_MODE_OUTPUT);
    }
}

/*************************************************************************
*  函数名称：void PIN_Reverse(GPIO_Name_t pin)
*  功能说明：GPIO电平翻转函数，使用前请先初始化
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义*
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：PIN_Reverse(P00_0); //翻转P00_0管脚的电平状态
*************************************************************************/
void PIN_Reverse(GPIO_Name_t pin)
{
	Ifx_P *port = PIN_GetModule(pin);
	uint8 pinIndex = PIN_GetIndex(pin);

	IfxPort_togglePin(port, pinIndex);
}

/*************************************************************************
*  函数名称：void PIN_Exti(GPIO_Name_t pin, IfxPort_InputMode mode)
*  功能说明：GPIO外部中断初始化
*  参数说明：pin      ： 管脚标号    lq_gpio.h中定义
*            mode     ： GPIO 模式   lq_gpio.h中的宏定义中选择*
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：PIN_Exti(P20_0, PIN_IRQ_MODE_FALLING);//配置P20_0 下降沿触发中断
*  备    注：TC264只有15个固定的GPIO支持外部中断 这15个固定GPIO分为4组，每组只能选择其中一个作为外部中断管脚使用
*           0：P15_4  P33_7  P15_5                             1：P14_3  P15_8
*           2：P10_2  P02_1  P00_4  P20_0  P11_10              3：P10_3  P14_1  P02_0  P20_9  P15_1
*			外部中断服务函数在LQ_GPIO.c中  中断优先级配置在LQ_GPIO.h中 可以自行修改
*************************************************************************/
void PIN_Exti(GPIO_Name_t pin, IfxPort_InputMode mode)
{
	int i,j;
	Ifx_P *port = PIN_GetModule(pin);
	uint8 pinIndex = PIN_GetIndex(pin);

	/* 关闭中断 */
	boolean        interruptState = IfxCpu_disableInterrupts();

	IfxScu_Req_In * reqPin =  0;

	/* 检测管脚 是否是 固定的17个管脚 */
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if(IfxScu_Req_In_pinTable[0][i][j] == NULL_PTR)
			{

			}
			else if((uint32)port == (uint32)(IfxScu_Req_In_pinTable[0][i][j]->pin.port) && pinIndex == IfxScu_Req_In_pinTable[0][i][j]->pin.pinIndex)
			{
				reqPin = IfxScu_Req_In_pinTable[0][i][j];
			}
		}
	}

	if(reqPin == 0)
	{
#pragma warning 557         // 屏蔽警告
		while (1);          /* 请选择有外部中断功能的管脚 */
#pragma warning 557         // 打开警告
	}

	/* 配置上下拉电阻 */
	IfxScuEru_initReqPin(reqPin, mode);

	/* 根据输入引脚确定输入通道EICRm */
	IfxScuEru_InputChannel inputChannel = (IfxScuEru_InputChannel)reqPin->channelId;

	/* 配置中断模式 */
	IfxScuEru_disableFallingEdgeDetection(inputChannel);                                               /* EICRm.FENn */
	IfxScuEru_disableRisingEdgeDetection(inputChannel);                                                /* EICRm.RENn */
	if(mode == IfxPort_InputMode_pullUp || mode == IfxPort_InputMode_noPullDevice)
	{
		IfxScuEru_enableFallingEdgeDetection(inputChannel);               /* EICRm.FENn */
	}
	if(mode == IfxPort_InputMode_pullDown|| mode == IfxPort_InputMode_noPullDevice)
	{
		IfxScuEru_enableRisingEdgeDetection(inputChannel);                /* EICRm.RENn */
	}
	IfxScuEru_enableAutoClear(inputChannel);

	/* 信号的目的地 */
	IfxScuEru_InputNodePointer triggerSelect = (IfxScuEru_InputNodePointer)inputChannel;
	IfxScuEru_OutputChannel    outputChannel = (IfxScuEru_OutputChannel)inputChannel;

//	/* Connecting Matrix, Event Trigger Logic ETL block */
	IfxScuEru_enableTriggerPulse(inputChannel);                                                        /* EICRM.EIENN */
	IfxScuEru_connectTrigger(inputChannel, triggerSelect);                                             /* EICRm.INPn */

	/* output channels, OutputGating Unit OGU block */
	IfxScuEru_setFlagPatternDetection(outputChannel, inputChannel, FALSE);                             /* IGCRm.IPENn */
	IfxScuEru_disablePatternDetectionTrigger(outputChannel);                                           /* IGCRm.GEENn */
	IfxScuEru_setInterruptGatingPattern(outputChannel, IfxScuEru_InterruptGatingPattern_alwaysActive); /* IGCEm.IGPn */

	uint8 vectabNum = PinIrqVectabNum[(int)inputChannel % 4];
	uint8 Priority  = PinIrqPriority[(int)inputChannel % 4];
	/* service request configuration */
	{
		volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCU.ERU[(int)outputChannel % 4];
		IfxSrc_init(src, vectabNum, Priority);
		IfxSrc_enable(src);
	}

	/* 安装管理中断的中断服务 */
	IfxCpu_Irq_installInterruptHandler((void *)PinIrqFuncPointer[(int)inputChannel % 4], Priority);

	/* 开启中断 */
	IfxCpu_restoreInterrupts(interruptState);
}

/*************************************************************************
*  函数名称：void PIN_ExtiEnable(GPIO_Name_t pin,  boolean bool)
*  功能说明：GPIO外部中断使能
*  参数说明： pin      ： 管脚标号    lq_gpio.h中定义
*            bool     ： 是否开启中断
*  函数返回：无
*  修改时间：2020年3月10日
*  举    例：PIN_ExtiEnable(P20_0, FALSE);//关闭P20_0 外部中断
*  备    注：TC264只有15个固定的GPIO支持外部中断 这15个固定GPIO分为4组，每组只能选择其中一个作为外部中断管脚使用
*           0：P15_4  P33_7  P15_5                             1：P14_3  P15_8
*           2：P10_2  P02_1  P00_4  P20_0  P11_10              3：P10_3  P14_1  P02_0  P20_9  P15_1
*			PIN_Exti()函数初始化后，默认GPIO开启外部中断，如果想要关闭，则可以调用PIN_ExtiEnable()
*************************************************************************/
void PIN_ExtiEnable(GPIO_Name_t pin,  boolean bool)
{
	int i,j;
	Ifx_P *port = PIN_GetModule(pin);
	uint8 pinIndex = PIN_GetIndex(pin);

	/* 关闭中断 */
	boolean        interruptState = IfxCpu_disableInterrupts();

	IfxScu_Req_In * reqPin =  0;

	/* 检测管脚 是否是 固定的17个管脚 */
	for(i = 0; i < 8; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if(IfxScu_Req_In_pinTable[0][i][j] == NULL_PTR)
			{

			}
			else if((uint32)port == (uint32)(IfxScu_Req_In_pinTable[0][i][j]->pin.port) && pinIndex == IfxScu_Req_In_pinTable[0][i][j]->pin.pinIndex)
			{
				reqPin = IfxScu_Req_In_pinTable[0][i][j];
			}
		}
	}

	if(reqPin == 0)
	{
#pragma warning 557         // 屏蔽警告
		while (1);          /* 请选择有外部中断功能的管脚 */
#pragma warning 557         // 打开警告
	}

	/* 根据输入引脚确定输入通道EICRm */
	IfxScuEru_InputChannel inputChannel = (IfxScuEru_InputChannel)reqPin->channelId;

	volatile Ifx_SRC_SRCR *src = &MODULE_SRC.SCU.SCU.ERU[(int)inputChannel % 4];

	IfxSrc_clearOverrun(src);
	IfxSrc_clearRequest(src);

	if(bool)
	{
		IfxSrc_enable(src);
	}
	else
	{
		IfxSrc_disable(src);
	}


	/* 开启中断 */
	IfxCpu_restoreInterrupts(interruptState);
}

