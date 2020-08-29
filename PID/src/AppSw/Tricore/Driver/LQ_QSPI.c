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


#include <LQ_QSPI.h>



/** QSPI中断CPU标号 */
const uint8 QSpiIrqVectabNum[4] = {QSPI0_VECTABNUM, QSPI1_VECTABNUM, QSPI2_VECTABNUM, QSPI3_VECTABNUM};

/** QSPI中断优先级 */
const uint8 QSpiIrqPriority[12] = {QSPI0_RX_PRIORITY, QSPI0_TX_PRIORITY, QSPI0_ER_PRIORITY, QSPI1_RX_PRIORITY, QSPI1_TX_PRIORITY, QSPI1_ER_PRIORITY,
		                           QSPI2_RX_PRIORITY, QSPI2_TX_PRIORITY, QSPI2_ER_PRIORITY, QSPI3_RX_PRIORITY, QSPI3_TX_PRIORITY, QSPI3_ER_PRIORITY};

/** QSPI 配置结构体 */
IfxQspi_SpiMaster         g_QSPIConfig[4];
IfxQspi_SpiMaster_Channel g_QspiMasterChannel[4];

/**
  * @brief    QSPI0_RX_IRQHandler中断服务函数
  * @note     QSPI0 接收中断服务函数
  */
void QSPI0_RX_IRQHandler(void)
{
	IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[0]);

	/* 用户代码 */
}

void QSPI0_TX_IRQHandler(void)
{
	IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[0]);
	/* 用户代码 */
}

void QSPI0_ER_IRQHandler(void)
{
	IfxQspi_SpiMaster_isrError(&g_QSPIConfig[0]);
	/* 用户代码 */
}

void QSPI1_RX_IRQHandler(void)
{
	IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[1]);
	/* 用户代码 */
}

void QSPI1_TX_IRQHandler(void)
{
	IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[1]);
	/* 用户代码 */
}

void QSPI1_ER_IRQHandler(void)
{
	IfxQspi_SpiMaster_isrError(&g_QSPIConfig[1]);
	/* 用户代码 */
}

void QSPI2_RX_IRQHandler(void)
{
	IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[2]);
	/* 用户代码 */
}

void QSPI2_TX_IRQHandler(void)
{
	IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[2]);
	/* 用户代码 */
}

void QSPI2_ER_IRQHandler(void)
{
	IfxQspi_SpiMaster_isrError(&g_QSPIConfig[2]);
	/* 用户代码 */
}

void QSPI3_RX_IRQHandler(void)
{
	IfxQspi_SpiMaster_isrReceive(&g_QSPIConfig[3]);
	/* 用户代码 */
}

void QSPI3_TX_IRQHandler(void)
{
	IfxQspi_SpiMaster_isrTransmit(&g_QSPIConfig[3]);
	/* 用户代码 */
}

void QSPI3_ER_IRQHandler(void)
{
	IfxQspi_SpiMaster_isrError(&g_QSPIConfig[3]);
	/* 用户代码 */
}

/* QSPI中断 */
IFX_INTERRUPT(QSPI0_RX_IRQHandler, QSPI0_VECTABNUM, QSPI0_RX_PRIORITY);
IFX_INTERRUPT(QSPI1_RX_IRQHandler, QSPI1_VECTABNUM, QSPI1_RX_PRIORITY);
IFX_INTERRUPT(QSPI2_RX_IRQHandler, QSPI2_VECTABNUM, QSPI2_RX_PRIORITY);
IFX_INTERRUPT(QSPI3_RX_IRQHandler, QSPI3_VECTABNUM, QSPI3_RX_PRIORITY);
IFX_INTERRUPT(QSPI0_TX_IRQHandler, QSPI0_VECTABNUM, QSPI0_TX_PRIORITY);
IFX_INTERRUPT(QSPI1_TX_IRQHandler, QSPI1_VECTABNUM, QSPI1_TX_PRIORITY);
IFX_INTERRUPT(QSPI2_TX_IRQHandler, QSPI2_VECTABNUM, QSPI2_TX_PRIORITY);
IFX_INTERRUPT(QSPI3_TX_IRQHandler, QSPI3_VECTABNUM, QSPI3_TX_PRIORITY);
IFX_INTERRUPT(QSPI0_ER_IRQHandler, QSPI0_VECTABNUM, QSPI0_ER_PRIORITY);
IFX_INTERRUPT(QSPI1_ER_IRQHandler, QSPI1_VECTABNUM, QSPI1_ER_PRIORITY);
IFX_INTERRUPT(QSPI2_ER_IRQHandler, QSPI2_VECTABNUM, QSPI2_ER_PRIORITY);
IFX_INTERRUPT(QSPI3_ER_IRQHandler, QSPI3_VECTABNUM, QSPI3_ER_PRIORITY);

/** QSPI中断服务函数地址 */
const void *QSPIIrqFuncPointer[12] = {&QSPI0_RX_IRQHandler, &QSPI0_TX_IRQHandler, &QSPI0_ER_IRQHandler,
									&QSPI1_RX_IRQHandler, &QSPI1_TX_IRQHandler, &QSPI1_ER_IRQHandler,
									&QSPI2_RX_IRQHandler, &QSPI2_TX_IRQHandler, &QSPI2_ER_IRQHandler,
									&QSPI3_RX_IRQHandler, &QSPI3_TX_IRQHandler, &QSPI3_ER_IRQHandler,};
/*************************************************************************
*  函数名称：QSPI_InitConfig(QSPI_CLK_t clkPin, QSPI_MISO_t misoPin, QSPI_MOSI_t mosiPin, QSPI_CS_t cQSPIn, uint32_t baudrate, uint8_t mode)
*  功能说明：QSPI 主机模式初始化
*  参数说明：
  * @param    clkPin      ： QSPI  clk管脚
  * @param    misoPin     :  QSPI  MISO管脚
  * @param    misoPin     :  QSPI  MOSI管脚
  * @param    misoPin     :  QSPI  CS管脚
  * @param    baudrate    ： QSPI 波特率
  * @param      mode            0: CLK空闲时刻为低电平 在第一个CLK跳边沿 捕获数据
  * @param      mode            1: CLK空闲时刻为低电平 在第二个CLK跳边沿 捕获数据
  * @param      mode            2: CLK空闲时刻为高电平 在第一个CLK跳边沿 捕获数据
  * @param      mode            3: CLK空闲时刻为高电平 在第二个CLK跳边沿 捕获数据
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：QSPI_InitConfig(QSPI2_CLK_P15_8, QSPI2_MISO_P15_7, QSPI2_MOSI_P15_5, QSPI2_CS_P15_2, 5000000, 3);
*
*************************************************************************/
void QSPI_InitConfig(QSPI_CLK_t clkPin, QSPI_MISO_t misoPin, QSPI_MOSI_t mosiPin, QSPI_CS_t cQSPIn, uint32_t baudrate, uint8_t mode)
{
	uint8_t QSPINum   = QSPI_GetModule(clkPin);

	IfxQspi_Sclk_Out   * QSPI_Sclk = IfxQspi_Sclk_Out_pinTable[QSPINum][QSPI_GetIndex(clkPin)];
	IfxQspi_Mrst_In    * QSPI_MISO = IfxQspi_Mrst_In_pinTable[QSPINum][QSPI_GetIndex(misoPin)];
	IfxQspi_Mtsr_Out   * QSPI_MOSI = IfxQspi_Mtsr_Out_pinTable[QSPINum][QSPI_GetIndex(mosiPin)];
	IfxQspi_Slso_Out   * QSPI_CS   = IfxQspi_Slso_Out_pinTable[QSPINum][QSPI_GetIndex(cQSPIn)][0];

	IfxQspi_SpiMaster_Config        spiMasterConfig;
	IfxQspi_SpiMaster_ChannelConfig spiMasterChannelConfig;


	{
		/* create module config */
		IfxQspi_SpiMaster_initModuleConfig(&spiMasterConfig, IfxQspi_getAddress((IfxQspi_Index)QSPINum));

		/* set the maximum baudrate */
		spiMasterConfig.base.maximumBaudrate = 50000000;

		/* ISR priorities and interrupt target */
		spiMasterConfig.base.rxPriority  = QSpiIrqPriority[QSPINum * 3];
		spiMasterConfig.base.txPriority  = QSpiIrqPriority[QSPINum * 3 + 1];
		spiMasterConfig.base.erPriority  = QSpiIrqPriority[QSPINum * 3 + 2];
		spiMasterConfig.base.isrProvider = (IfxSrc_Tos)QSpiIrqVectabNum[QSPINum];

		/* pin configuration */
		const IfxQspi_SpiMaster_Pins pins = {QSPI_Sclk,                               /* SCLK */
											 IfxPort_OutputMode_pushPull,
											 QSPI_MOSI,  IfxPort_OutputMode_pushPull, /* MTSR */
											 QSPI_MISO,  IfxPort_InputMode_pullDown,  /* MRST */
											 IfxPort_PadDriver_cmosAutomotiveSpeed3                   /* pad driver mode */
		};
		spiMasterConfig.pins = &pins;

		/* initialize module */
		IfxQspi_SpiMaster_initModule(&g_QSPIConfig[QSPINum], &spiMasterConfig);
	}

	{
		/* create channel config */
		IfxQspi_SpiMaster_initChannelConfig(&spiMasterChannelConfig,
			&g_QSPIConfig[QSPINum]);




		const IfxQspi_SpiMaster_Output slsOutput = {QSPI_CS,
													IfxPort_OutputMode_pushPull,
													IfxPort_PadDriver_cmosAutomotiveSpeed1};

		spiMasterChannelConfig.sls.output.pin    = slsOutput.pin;
		spiMasterChannelConfig.sls.output.mode   = slsOutput.mode;
		spiMasterChannelConfig.sls.output.driver = slsOutput.driver;
		spiMasterChannelConfig.base.mode.csInactiveDelay = 1;
		spiMasterChannelConfig.base.mode.csLeadDelay     = 10;                    /* CS前置延时 */
		spiMasterChannelConfig.base.mode.csTrailDelay    = 1;
		spiMasterChannelConfig.base.mode.dataHeading = SpiIf_DataHeading_msbFirst;  /* MSB在前      */
		spiMasterChannelConfig.base.mode.dataWidth = 8;                             /* 8bit     */
		spiMasterChannelConfig.base.baudrate = (float)baudrate;                     /* 设置速度    */
		spiMasterChannelConfig.base.mode.csActiveLevel = Ifx_ActiveState_low;       /* CS为低有效*/

	    if(mode / 2)
	    {
	    	spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleHigh;  //时钟空闲时为高电平
	    }
	    else
	    {
	    	spiMasterChannelConfig.base.mode.clockPolarity = SpiIf_ClockPolarity_idleLow;   //时钟空闲时为低电平
	    }

	    if(mode % 2)
	    {
	    	spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnTrailingEdge;  //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	    }
	    else
	    {
	    	spiMasterChannelConfig.base.mode.shiftClock = SpiIf_ShiftClock_shiftTransmitDataOnLeadingEdge;  //串行同步时钟的第一个跳变沿（上升或下降）数据被采样
	    }
	    //接收，发送和错误中断配置
		IfxCpu_Irq_installInterruptHandler((void*)QSPIIrqFuncPointer[QSPINum * 3],     (uint32)QSpiIrqPriority[QSPINum * 3]);
		IfxCpu_Irq_installInterruptHandler((void*)QSPIIrqFuncPointer[QSPINum * 3 + 1], (uint32)QSpiIrqPriority[QSPINum * 3 + 1]);
		IfxCpu_Irq_installInterruptHandler((void*)QSPIIrqFuncPointer[QSPINum * 3 + 2], (uint32)QSpiIrqPriority[QSPINum * 3 + 2]);


		IfxQspi_SpiMaster_initChannel(&g_QspiMasterChannel[QSPINum],
			&spiMasterChannelConfig);
	}

}

/*************************************************************************
*  函数名称：uint8 QSPI_ReadWriteNByte(QSPI_t QSPI, uint8_t *txData, uint8_t *rxData, uint32_t len)
*  功能说明：QSPI读写N个字节
*  参数说明：
  * @param    QSPI     ：  QSPI0 - QSPI3
  * @param    txData  ：  写入的buff
  * @param    rxData  ：  读出的buff
  * @param    len     ：  长度
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*  			QSPI_ReadWriteNByte(QSPI3, buff, buff, 2); //使用QSPI3 读写两个字节
*************************************************************************/
uint8 QSPI_ReadWriteNByte(QSPI_t QSPI, uint8_t *txData, uint8_t *rxData, uint16 len)
{
	/* 开始传输 */
	IfxQspi_SpiMaster_exchange(&g_QspiMasterChannel[QSPI], txData, rxData, len);

	/* 等待传输结束  */
	while (IfxQspi_SpiMaster_getStatus(&g_QspiMasterChannel[QSPI]) == SpiIf_Status_busy) ;

	return 0;
}



/////////////////////////////////////////////////////////////////////////////////////
