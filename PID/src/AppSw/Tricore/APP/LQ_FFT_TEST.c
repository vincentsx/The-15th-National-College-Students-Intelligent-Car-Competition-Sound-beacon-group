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

QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <Ifx_FftF32.h>
#include <LQ_FFT.h>
#include <LQ_FFT_TEST.h>
#include <LQ_GPIO_LED.h>
#include <stdio.h>

/* matlab 代码  可以对比fft的结果
Fs = 1024;            % Sampling frequency
T = 1/Fs;             % Sampling period
L = 1024;             % Length of signal
t = (0:L-1)*T;        % Time vector
S = 70*sin(2*pi*50*t) + 100*sin(2*pi*120*t);
f = Fs*(0:(L/2))/L;
Y = fft(S);
P2 = abs(Y/L);
P1 = P2(1:L/2+1);
P1(2:end-1) = 2*P1(2:end-1);
hold off
plot(f,P1)
title('Single-Sided Amplitude Spectrum of S(t)')
xlabel('f (Hz)')
ylabel('|P1(f)|')
hold on
plot(1000*t(1:50),S(1:50))
title('Signal Corrupted with Zero-Mean Random Noise')
xlabel('t (milliseconds)')
ylabel('X(t)')
 */

/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////以下部分为功能测试/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////



//IFFT变换结果，必须256位对齐
__attribute__ ((aligned(256)))  csint32 ifftOut[1024];

//源波形，必须256位对齐
__attribute__ ((aligned(256)))  int16_t fftIn[1024];

//FFT变换结果，必须256位对齐
__attribute__ ((aligned(256)))  csint32 fftOut[1024];

/*************************************************************************
*  函数名称：void Test_FFT(void)
*  功能说明：测试FFT变换
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月20日
*  备    注：测试FFT变换和IFFT变换    注意  硬件FFT需要TC264 DA的芯片才有此功能
*************************************************************************/
void Test_FFT(void)
{
	uint16_t Fs = 1024;    //信号频率

	float    T  = 1.0f/Fs; //信号周期

	uint16_t L  = 1024;    //长度

	/* 做一个50Hz和120Hz的正玄叠加信号  */
	int i = 0;
	for(i = 0; i < L; i++)
	{
		fftIn[i] = 70*sin(2*IFX_PI*50*T*i) + 100*sin(2*IFX_PI*120*T*i);
	}


	while(1)
	{
		/* 初始化为 FFT  注意 这里FFT输出结果是实际结果的64倍  */
		FFT_InitConfig((uint32)fftIn, (uint32)fftOut, IfxFft_Length_1024);

		FFT_Start();

		/* 初始化为 IFFT  */
		IFFT_InitConfig((uint32)fftOut, (uint32)ifftOut, IfxFft_Length_1024);

		FFT_Start();

		LED_Ctrl(LED0,RVS);               //电平翻转,LED闪烁

	}
}


//源波形，256位对齐
__attribute__ ((aligned(256))) cfloat32 sfftIn[1024];

//IFFT变换结果，256位对齐
__attribute__ ((aligned(256))) cfloat32 sifftOut[1024];

//FFT变换结果，256位对齐
__attribute__ ((aligned(256))) cfloat32 sfftOut[1024];

/*************************************************************************
*  函数名称：void Test_SoftFft(void)
*  功能说明：测试软件FFT变换
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月20日
*  备    注：测试FFT变换和IFFT变换    注意  所有TC264 芯片都可以
*************************************************************************/
void Test_SoftFft(void)
{
	uint16_t Fs = 1024;    //信号频率

	float    T  = 1.0f/Fs; //信号周期

	uint16_t L  = 1024;    //长度

	/* 做一个50Hz和120Hz的正玄叠加信号  */
	int i = 0;
	for(i = 0; i < L; i++)
	{
		sfftIn[i].real = 70*sin(2*IFX_PI*50*T*i) + 100*sin(2*IFX_PI*120*T*i);
		sfftIn[i].imag = 0;
	}

	while(1)
	{
		//FFT
		Ifx_FftF32_radix2(sfftOut, sfftIn, 1024);

		//IFFT  注意 这里IFFT输出结果是实际结果的1024倍
		Ifx_FftF32_radix2I(sifftOut, sfftOut, 1024);

		LED_Ctrl(LED0,RVS);               //电平翻转,LED闪烁

	}
}
