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
注意  硬件FFT需要TC264 DA的芯片才有此功能
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/


#include <LQ_FFT.h>
#include <LQ_STM.h>



/**
 * FFT 句柄
 */
IfxFft_Fft g_Fft;
IfxFft_Fft_Job g_FftJob;
IfxFft_Fft_JobConfig fftJobConfig;
/***********************************************************************************************/
/********************************FFT中断  服务函数******************************************/
/***********************************************************************************************/

/*************************************************************************
*  函数名称：void FFT_INTRA_IRQHandler(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：FFT中断服务函数
*************************************************************************/
IFX_INTERRUPT(FFT_INTRA_IRQHandler, FFT_VECTABNUM, FFT_INTRA_PRIORITY)
{
	/* 开启CPU中断  否则中断不可嵌套 */
//	IfxCpu_enableInterrupts();

    IfxFft_Fft_isrIntra(&g_Fft);
}

/*************************************************************************
*  函数名称：void FFT_INTRA_IRQHandler(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：FFT中断服务函数
*************************************************************************/
IFX_INTERRUPT(FFT_INPUT_IRQHandler, FFT_VECTABNUM, FFT_INPUT_PRIORITY)
{
	/* 开启CPU中断  否则中断不可嵌套 */
//	IfxCpu_enableInterrupts();

    IfxFft_Fft_isrInput(&g_Fft);
}

/*************************************************************************
*  函数名称：void FFT_INTRA_IRQHandler(void)
*  功能说明：
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月30日
*  备    注：FFT中断服务函数
*************************************************************************/
IFX_INTERRUPT(FFT_OUTPUT_IRQHandler, FFT_VECTABNUM, FFT_OUTPUT_PRIORITY)
{
	/* 开启CPU中断  否则中断不可嵌套 */
//	IfxCpu_enableInterrupts();

    IfxFft_Fft_isrOutput(&g_Fft);

}






/*************************************************************************
*  函数名称：void FFT_InitConfig(uint32 InDataAddr, uint32 OutDataAddr, IfxFft_Length len)
*  功能说明：FFT初始化函数
*  参数说明：InDataAddr  : FFT输入数据地址
*  参数说明：OutDataAddr : FFT输出数据地址
*  参数说明：len         : FFT长度  IfxFft_Length_256  、   IfxFft_Length_512  、  IfxFft_Length_1024
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：       注意  硬件FFT需要TC264 DA的芯片才有此功能  注意FFT结果和实际结果相比 会放大一定倍数
*
*************************************************************************/
void FFT_InitConfig(uint32 InDataAddr, uint32 OutDataAddr, IfxFft_Length len)
{


	//新建FFT模块配置
	IfxFft_Fft_Config fftConfig;

	//关闭CPU总中断
	IfxCpu_disableInterrupts();



	IfxFft_Fft_initModuleConfig(&fftConfig, &MODULE_FFT, &MODULE_LMU);

	//中断配置
	fftConfig.inputPriority = FFT_INPUT_PRIORITY;
	fftConfig.intraPriority = FFT_INTRA_PRIORITY;//“内部传输”中断优先级
	fftConfig.outputPriority = FFT_OUTPUT_PRIORITY;
	fftConfig.typeOfService = FFT_VECTABNUM;     //调用CPU的服务类型

	//使用DMA通道进行input/output变换
	fftConfig.inputDmaChannelId = IfxDma_ChannelId_4;  //输入DMA通道，当(IFXFFT_FFT_DMA_SUPPORT != 0)时，使用中
	fftConfig.outputDmaChannelId = IfxDma_ChannelId_5; //输出DMA通道，当(IFXFFT_FFT_DMA_SUPPORT != 0)时，使用中
	fftConfig.discardDmaChannelId = IfxDma_ChannelId_6;//丢弃未使用样本的DMA通道，当(IFXFFT_FFT_DMA_SUPPORT != 0)时，使用中

	//安装interrupt handlers ISR_FFT_INTRA
	IfxCpu_Irq_installInterruptHandler(&FFT_INPUT_IRQHandler, FFT_INPUT_PRIORITY);
	IfxCpu_Irq_installInterruptHandler(&FFT_INTRA_IRQHandler, FFT_INTRA_PRIORITY);
	IfxCpu_Irq_installInterruptHandler(&FFT_OUTPUT_IRQHandler, FFT_OUTPUT_PRIORITY);
	IfxCpu_enableInterrupts();//总中断使能



	//FFT初始化
	IfxFft_Fft_initModule(&g_Fft, &fftConfig);


	//作业配置
	IfxFft_Fft_initJobConfig(&fftJobConfig, &g_Fft);

	fftJobConfig.operation = IfxFft_Operation_fft;             //FFT变换   或者IFFT变换

	//自定义作业
	fftJobConfig.fftLength = len;                              //FFT变换的长度

	fftJobConfig.inputFormat = IfxFft_Input_realSInt16;        //输入格式：16位数
	fftJobConfig.inputPtr = (void *)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (uint32)InDataAddr);//输入数据指针
	fftJobConfig.inputLength = (uint16)(pow(2, len)+0.90f);     //输入数据长度

	fftJobConfig.outputFormat = IfxFft_Output_complexSInt32;   //输出格式：复数32位
	fftJobConfig.outputPtr = (void *)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (uint32)OutDataAddr);//输出数据指针
	fftJobConfig.outputLength = (uint16)(pow(2, len)+0.90f);    //输出数据长度

	//初始化作业任务
	IfxFft_Fft_initJob(&g_FftJob, &fftJobConfig);
}

/*************************************************************************
*  函数名称：void IFFT_InitConfig(uint32 InDataAddr, uint32 OutDataAddr, IfxFft_Length len)
*  功能说明：IFFT初始化函数
*  参数说明：InDataAddr  : IFFT输入数据地址
*  参数说明：OutDataAddr : IFFT输出数据地址
*  参数说明：len         : IFFT长度  IfxFft_Length_256  、   IfxFft_Length_512  、  IfxFft_Length_1024
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：       注意  硬件FFT需要TC264 DA的芯片才有此功能
*
*************************************************************************/
void IFFT_InitConfig(uint32 InDataAddr, uint32 OutDataAddr, IfxFft_Length len)
{
	//新建FFT模块配置
	IfxFft_Fft_Config fftConfig;

	//关闭CPU总中断
	IfxCpu_disableInterrupts();



	IfxFft_Fft_initModuleConfig(&fftConfig, &MODULE_FFT, &MODULE_LMU);

	//中断配置
	fftConfig.inputPriority = FFT_INPUT_PRIORITY;
	fftConfig.intraPriority = FFT_INTRA_PRIORITY;//“内部传输”中断优先级
	fftConfig.outputPriority = FFT_OUTPUT_PRIORITY;
	fftConfig.typeOfService = FFT_VECTABNUM;     //调用CPU的服务类型

	//使用DMA通道进行input/output变换
	fftConfig.inputDmaChannelId = IfxDma_ChannelId_4;  //输入DMA通道，当(IFXFFT_FFT_DMA_SUPPORT != 0)时，使用中
	fftConfig.outputDmaChannelId = IfxDma_ChannelId_5; //输出DMA通道，当(IFXFFT_FFT_DMA_SUPPORT != 0)时，使用中
	fftConfig.discardDmaChannelId = IfxDma_ChannelId_6;//丢弃未使用样本的DMA通道，当(IFXFFT_FFT_DMA_SUPPORT != 0)时，使用中

	//安装interrupt handlers ISR_FFT_INTRA
	IfxCpu_Irq_installInterruptHandler(&FFT_INPUT_IRQHandler, FFT_INPUT_PRIORITY);
	IfxCpu_Irq_installInterruptHandler(&FFT_INTRA_IRQHandler, FFT_INTRA_PRIORITY);
	IfxCpu_Irq_installInterruptHandler(&FFT_OUTPUT_IRQHandler, FFT_OUTPUT_PRIORITY);
	IfxCpu_enableInterrupts();//总中断使能

	//FFT初始化
	IfxFft_Fft_initModule(&g_Fft, &fftConfig);



	//作业配置
	IfxFft_Fft_initJobConfig(&fftJobConfig, &g_Fft);

	fftJobConfig.operation = IfxFft_Operation_ifft;            //IFFT变换

	//自定义作业
	fftJobConfig.fftLength = len;                              //FFT变换的长度

	fftJobConfig.inputFormat = IfxFft_Input_complexSInt32;     //输入格式
	fftJobConfig.inputPtr = (void *)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (uint32)InDataAddr);//输入数据指针
	fftJobConfig.inputLength = (uint16)(pow(2, len)+0.90f);    //输入数据长度

	fftJobConfig.outputFormat = IfxFft_Output_complexSInt32;    //输出格式
	fftJobConfig.outputPtr = (void *)IFXCPU_GLB_ADDR_DSPR(IfxCpu_getCoreId(), (uint32)OutDataAddr);//输出数据指针
	fftJobConfig.outputLength = (uint16)(pow(2, len)+0.90f);    //输出数据长度

	//初始化作业任务
	IfxFft_Fft_initJob(&g_FftJob, &fftJobConfig);
}

/*************************************************************************
*  函数名称：void FFT_Start(void)
*  功能说明：FFT变换函数
*  参数说明：
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：     注意  硬件FFT需要TC264 DA的芯片才有此功能
*************************************************************************/
void FFT_Start(void)
{
	//开始转换，直到转换结束返回TRUE.
	while (IfxFft_Fft_startJob(&g_FftJob) == FALSE);

	/* 硬件FFT计算完后会通过DMA将结果传输到输出数组中   加一段延时   等待DMA传输结束  */
	for (volatile int i = 0; i < 1000; ++i)
	{
		__asm("NOP"); /* delay */
	}
}



