/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】zyf/chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.0
【最后更新】2020年4月10日
【相关信息参考下列地址】
【网    站】http://www.lqist.cn
【淘宝店铺】http://longqiu.taobao.com
------------------------------------------------
【dev.env.】Hightec4.9.3/Tasking6.3及以上版本
【Target 】 TC264DA
【Crystal】 20.000Mhz
【SYS PLL】 200MHz
基于iLLD_1_0_1_11_0底层程序
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/
#ifndef _LQ_INCLUDE_H_
#define _LQ_INCLUDE_H_

/////////////c标准库文件//////////////////////////////////////////////////
#include "math.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"

/////////////illD底层库文件//////////////////////////////////////////////////
#include <Asclin\Asc\IfxAsclin_Asc.h>
#include "Asclin\Spi\IfxAsclin_Spi.h"
#include "Ccu6\TPwm\IfxCcu6_TPwm.h"
#include "Ccu6\Timer\IfxCcu6_Timer.h"
#include "Ccu6\Icu\IfxCcu6_Icu.h"
#include "Ccu6\PwmBc\IfxCcu6_PwmBc.h"
#include "Compilers.h"
#include "ConfigurationIsr.h"
#include <Configuration.h>
#include <Cif\Cam\IfxCif_Cam.h>
#include "Cif\Std\IfxCif.h"
#include "Cpu\Std\Ifx_Types.h"
#include "Cpu\Irq\IfxCpu_Irq.h"
#include <Cpu\Std\IfxCpu.h>
#include "Cpu\Std\Platform_Types.h"
#include "Cpu\Std\IfxCpu_Intrinsics.h"
#include <Dma\Dma\IfxDma_Dma.h>
#include "Flash\Std\IfxFlash.h"
#include <Fft\Fft\IfxFft_Fft.h>
#include "Gtm\Std\IfxGtm.h"
#include "Gtm\Std\IfxGtm_Tom.h"
#include "Gtm\Std\IfxGtm_Tim.h"
#include "Gtm\Atom\Pwm\IfxGtm_Atom_Pwm.h"
#include "Gtm\Tim\In\IfxGtm_Tim_In.h"
#include "Gtm\Tom\Pwm\IfxGtm_Tom_Pwm.h"
#include "Gtm\Tom\Timer\IfxGtm_Tom_Timer.h"
#include "Gtm\Tom\PwmHl\IfxGtm_Tom_PwmHl.h"
#include "Gtm\Trig\IfxGtm_Trig.h"
#include <Gpt12\IncrEnc\IfxGpt12_IncrEnc.h>
#include "Gpt12\Std\IfxGpt12.h"
//#include "Ifx_Assert.h"
#include <I2c\I2c\IfxI2c_I2c.h>
#include "IfxCpu_reg.h"
#include <Ifx_Cfg.h>
#include "IfxScu_reg.h"
#include "IfxPort_reg.h"
#include "Port\Std\IfxPort.h"
#include "Port\Io\IfxPort_Io.h"
#include "Qspi\SpiMaster\IfxQspi_SpiMaster.h"
#include "Qspi\SpiSlave\IfxQspi_SpiSlave.h"
#include "SysSe\Bsp\Bsp.h"
#include "Stm\Std\IfxStm.h"
#include "Scu\Std\IfxScuWdt.h"
#include "Src\Std\IfxSrc.h"
#include "Scu\Std\IfxScuEru.h"
#include "Tricore\Compilers\Compilers.h"
#include "Vadc\Adc\IfxVadc_Adc.h"
#include "_PinMap\IfxGtm_PinMap.h"
#include "_Reg\IfxVadc_reg.h"
#include "_Reg\IfxStm_reg.h"
#include "_Utilities\Ifx_Assert.h"
#include "I2c/I2c/IfxI2c_I2c.h"
/////////////基础驱动文件//////////////////////////////////////////////////
#include "LQ_ADC.h"
#include "LQ_DMA.h"
#include "LQ_EEPROM.h"
#include "LQ_FFT.h"
#include "LQ_GPIO.h"
#include "LQ_GPT12_ENC.h"
#include "LQ_GTM.h"
#include "LQ_SOFTI2C.h"
#include "LQ_SPI.h"
#include "LQ_STM.h"
#include "LQ_UART.h"
#include "LQ_CCU6.h"
#include "LQ_QSPI.h"
#include "LQ_EMEM.h"
#include "LQ_GPSR.h"
/////////////用户应用实例//////////////////////////////////////////////////
//#include "..\APP\LQ_CAM.h"
#include "..\APP\LQ_ADC_7Mic.h"
#include "..\APP\LQ_CAMERA.h"
#include "..\APP\LQ_I2C_9AX.h"
#include "..\APP\LQ_GPIO_ExInt.h"
#include "..\APP\LQ_ICM20602.h"
#include "..\APP\LQ_GPIO_KEY.h"
#include "..\APP\LQ_GPIO_LED.h"
#include "..\APP\LQ_GPT_mini512.h"
#include "..\APP\LQ_Atom_Motor.h"
#include "..\APP\LQ_I2C_MPU6050.h"
#include "..\APP\LQ_MT9V034.h"
#include "..\APP\LQ_OLED096.h"
#include "..\APP\LQ_Tom_Servo.h"
#include "..\APP\LQ_I2C_VL53.h"
#include "..\APP\LQ_TFT18.h"
#include "..\APP\LQ_RDA5807.h"
#include "..\APP\LQ_UART_Bluetooth.h"
#include "LQ_STM_TImer.h"
#include "LQ_Tim_InputCature.h"
#include "LQ_EEPROM_TEST.h"
#include "LQ_FFT_TEST.h"
////////////////////////////////////////////////////////////////////////////
typedef struct
{
    float32 sysFreq;  //Actual SPB frequency
    float32 cpuFreq;  //Actual CPU frequency
    float32 pllFreq;  //Actual PLL frequency
    float32 stmFreq;  //Actual STM frequency
} AppInfo;

// Application information
typedef struct
{
    AppInfo info;     //Info object
} App_Cpu0;

#endif
////////////////////////////////////////////////////////////////////////////
