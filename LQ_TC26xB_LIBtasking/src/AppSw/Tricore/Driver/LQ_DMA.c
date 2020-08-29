/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】ZYF/chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年5月10日
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


#include <IfxPort_reg.h>
#include <LQ_CAMERA.h>
#include <LQ_DMA.h>
#include <LQ_GPIO.h>

//安装中断服务函数，向量号和优先级
IFX_INTERRUPT(DMA_IRQHandler, DMA_VECTABNUM, DMA_PRIORITY);

//安装中断服务函数
void DMA_IRQHandler(void)
{
	/* 开启CPU中断  否则中断不可嵌套 */
//	IfxCpu_enableInterrupts();

	DMA_CameraIRQ(PIN_INT2_PRIORITY);
}


/**
 * 摄像头 DMA 配置结构体
 */
Dma_Camera_t g_DmaCameraLinkedList;

/** 摄像头 数据采集完成标志位 */
volatile unsigned char Camera_Flag = 0;




/*!
  * @brief    摄像头DMA传输初始化
  *
  * @param    srcStartAddr   ：源地址 IO地址
  * @param    dstStartAddr   ：目的地址 摄像头数据缓冲区
  * @param    channel        ：DMA传输通道 0-47      注意 DMA传输通道需要和PIX像素中断优先级保持一致
  *
  * @return   无
  *
  * @note     注意 DMA传输通道需要和PIX像素中断优先级保持一致    一幅图需要进行两次DMA传输，因此会触发两次DMA中断
  *
  * @see      DMA_CameraInitConfig((unsigned long)(&MODULE_P02.IN.U), (unsigned long)Image_Data, PIN_INT2_PRIORITY);
  *
  * @date     2020/4/8
  */
void DMA_CameraInitConfig(unsigned long srcStartAddr, unsigned long dstStartAddr, unsigned long channel)
{
	/* 关闭中断 */
	boolean        interruptState = IfxCpu_disableInterrupts();

	/* create module config */
	IfxDma_Dma_Config        dmaConfig;
	IfxDma_Dma_initModuleConfig(&dmaConfig, &MODULE_DMA);

	/* initialize module */
	IfxDma_Dma               dma;
	IfxDma_Dma_initModule(&dma, &dmaConfig);

	/* initial configuration for all channels */
	IfxDma_Dma_ChannelConfig cfg;
	IfxDma_Dma_initChannelConfig(&cfg, &dma);

//	cfg.busPriority   = IfxDma_ChannelBusPriority_high;                    //优先级最高
	cfg.requestMode   = IfxDma_ChannelRequestMode_oneTransferPerRequest;   //请求启动一次传输
	cfg.moveSize      = IfxDma_ChannelMoveSize_8bit;                       //一次move 8bit
	cfg.shadowControl = IfxDma_ChannelShadow_none;                         //单次模式
	cfg.operationMode = IfxDma_ChannelOperationMode_continuous;            //传输完成继续传输
	cfg.hardwareRequestEnabled = TRUE;                                     //使能硬件触发传输
	cfg.sourceAddress = IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), srcStartAddr);  //设置源地址
	cfg.sourceCircularBufferEnabled = TRUE;                                      //保持源地址不变
	cfg.sourceAddressCircularRange = IfxDma_ChannelIncrementCircular_none;
	cfg.channelId     = channel;                                                 //设置DMA通道
	cfg.channelInterruptEnabled = TRUE;                                          //使能通道中断
	cfg.channelInterruptPriority = DMA_PRIORITY;                                 //DMA中断优先级
	cfg.channelInterruptTypeOfService = DMA_VECTABNUM;


	/* 设置传输表述符 */
	cfg.destinationAddress =  IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), dstStartAddr);

	/* address to next transaction set */
	cfg.shadowAddress =       0;

	cfg.transferCount                    = 11280;

	/* transfer first transaction set into DMA channel */
	IfxDma_Dma_initChannel(&g_DmaCameraLinkedList.chn, &cfg);

	/* transfer into linked list storage */
	IfxDma_Dma_initLinkedListEntry((void *)&g_DmaCameraLinkedList.linkedList[0], &cfg);

	IfxCpu_Irq_installInterruptHandler((void*)DMA_IRQHandler, DMA_PRIORITY);

	/* clear service request flag */
	(IfxDma_Dma_getSrcPointer(&g_DmaCameraLinkedList.chn))->B.CLRR = 1;

	IfxDma_clearChannelInterrupt(&MODULE_DMA, channel);

	IfxDma_disableChannelTransaction(&MODULE_DMA, channel);

	/* 开启中断 */
	IfxCpu_restoreInterrupts(interruptState);
}





