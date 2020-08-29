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
通过窗口设置神眼分辨率为188*120，
________________________________________________________________
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 摄像头管脚  TC264DA管脚   说明              DMA方式采集所用管脚通过电阻跳线
	D0        P02_0       摄像头数据bit0
	D1        P02_1       摄像头数据bit1
	D2        P02_2       摄像头数据bit2
	D3        P02_3       摄像头数据bit3
	D4        P02_4       摄像头数据bit4
	D5        P02_5       摄像头数据bit5
	D6        P02_6       摄像头数据bit6
	D7        P02_7       摄像头数据bit7
	CLK       P00_7       像素时钟        外部中断第2组：P00_4
	VSNC      P00_8       场信号             外部中断第3组：P15_1
	HSNC      P00_9       行信号
    SCL       P11_2       兼容模拟SCCB SCL
    SDA       P11_3       兼容模拟SCCB SDA
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <IfxPort.h>
#include <LQ_GPIO.h>
#include <LQ_MT9V034.h>
#include <LQ_TFT18.h>



/*!
  * @brief    MT9V034寄存器初始化
  *
  * @param    fps  :帧率
  *
  * @return   无
  *
  * @note     一般使用50帧
  *
  * @see      MT9V034_Init(50);
  *
  * @date     2019/6/12 星期三
  */
void MT9V034_Init(unsigned char fps)
{
	unsigned short data = 0;

	//摄像头SCCB接口初始化
	SCCB_Init();

	if(SCCB_RegRead(MT9V034_I2C_ADDR>>1,MT9V034_CHIP_VERSION,&data) == 0)  //读取摄像头版本寄存器
	{
		if(data != MT9V034_CHIP_ID)                                        //芯片ID不正确，说明没有正确读取导数据，检查接线
		{
#ifdef USEOLED
			OLED_P6x8Str(0, 0, "LQ-V034 id error");
#else
			TFTSPI_P8X8Str(0,8,"LQ-V034 id error",u16RED,u16BLUE);
#endif
#pragma warning 557         // 屏蔽警告
			while (1);
#pragma warning default     // 打开警告
		}
	}
	else
	{
#ifdef USEOLED
		OLED_P6x8Str(0, 0, "LQ-V034 sccb error");
#else
		TFTSPI_P8X8Str(0,8,"LQ-V034 sccb error",u16RED,u16BLUE);
#endif

#pragma warning 557         // 屏蔽警告
		while (1);          //摄像头识别失败，停止运行
#pragma warning default     // 打开警告
	}
	/* 恢复默认配置 */
//	MT9V034_Default_Settings();

    /* 设置摄像头图像4*4分频输出PCLK 27/4 = 6.75M ,BIT4,5镜像设置:上下左右均镜像 */
	MT9V034_SetFrameResolution(MT9V034_IMAGEH, MT9V034_IMAGEW);

    /* 设置帧率 */
    MT9V034_SetFrameRate(fps);

    /* 曝光设置 */
    MT9V034_SetAutoExposure(CAMERA_AUTO_EXPOSURE);

	SCCB_RegWrite(MT9V034_I2C_ADDR, 0x2C, 0x0004);  //参考电压设置   1.4v
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ANALOG_CTRL, MT9V034_ANTI_ECLIPSE_ENABLE);  //反向腐蚀
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_ADC_RES_CTRL_REG, 0x0303);      //0x1C  here is the way to regulate darkness :)
    ////
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x13,0x2D2E);//We also recommended using R0x13 = 0x2D2E with this setting for better column FPN.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x20,CAMERA_CONTRAST);//0x01C7对比度差，发白；0x03C7对比度提高 Recommended by design to improve performance in HDR mode and when frame rate is low.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x24,0x0010);//Corrects pixel negative dark offset when global reset in R0x20[9] is enabled.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x2B,0x0003);//Improves column FPN.
    SCCB_RegWrite(MT9V034_I2C_ADDR,0x2F,0x0003);//Improves FPN at near-saturation.

    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_V2_CTRL_REG_A, 0x001A);        //0x32   0x001A
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_HDR_ENABLE_REG,0x0103);        //0x0F High Dynamic Range enable,bit is set (R0x0F[1]=1), the sensor uses black level correction values from one green plane, which are applied to all colors.



    /* 0xA5  图像亮度  60  1-64  */
    SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_AGC_AEC_DESIRED_BIN_REG, CAMERA_AUTO_EXPOSURE_BRIGHTNESS);

}

/*!
  * @brief    帧率设置
  *
  * @param    fps : 帧率
  *
  * @return
  *
  * @note     帧率其实是通过增加空白行实现的， 下面是实测的帧率与虚拟行之间的关系
  * @note     帧率	    200    150    100    70.9    50    20    10
  * @note     虚拟行    39     92     199    330     515   1450  3000
  *
  * @see      MT9V034_SetFrameRate(50);
  *
  * @date     2019/7/26 星期五
  */
void MT9V034_SetFrameRate(unsigned char fps)
{
    float vertical = 0;
    if(fps > 200)
    {
        vertical = 2;
    }
    else if(fps >= 150)
    {
        vertical = -1.06 * fps + 251;
    }
    else if(fps > 100)
    {
        vertical = -2.14 * fps + 413;
    }
    else if(fps >= 71)
    {
        vertical = -4.5017 * fps + 649.17;
    }
    else if(fps > 50)
    {
        vertical = -8.8517 * fps + 957.58;
    }
    else if(fps > 20)
    {
        vertical = -31.167 * fps + 2073.3;
    }
    else
    {
        vertical = -155 * fps + 4550;
    }
    /* 虚拟行 设置帧率时间 2–32288 */
    SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_VERTICAL_BLANKING, (unsigned short)vertical);
}

/*!
  * @brief    设置图像窗口大小
  *
  * @param    fps : 帧率
  *
  * @return   无
  *
  * @note     一般使用默认即可
  *
  * @see      MT9V034_SetFrameResolution(MT9V034_IMAGEH, MT9V034_IMAGEW);
  *
  * @date     2019/7/26 星期五
  */
static void MT9V034_SetFrameResolution(unsigned short height,unsigned short width)
{
    unsigned short data = 0;

    if((height*4) <= MAX_IMAGE_HEIGHT)   //判断行是否4分频
    {
        height *= 4;
        data |= MT9V034_READ_MODE_ROW_BIN_4;
    }
    else if((height*2)<=MAX_IMAGE_HEIGHT )  //判断行是否2分频
    {
        height *= 2;
        data |= MT9V034_READ_MODE_ROW_BIN_2;
    }

    if((width*4)<=MAX_IMAGE_WIDTH )   //判断列是否4分频
    {
        width *= 4;
        data |= MT9V034_READ_MODE_COL_BIN_4;
    }
    else if((width*2)<=MAX_IMAGE_WIDTH )   //判断列是否2分频
    {
        width *= 2;
        data |= MT9V034_READ_MODE_COL_BIN_2;
    }

    //         水平翻转                     垂直翻转
    data |= (MT9V034_READ_MODE_ROW_FLIP|MT9V034_READ_MODE_COLUMN_FLIP);  //需要翻转的可以打开注释
    MTV_IICWriteReg16(MT9V034_READ_MODE, data);       //写寄存器，配置行分频

    MTV_IICWriteReg16(MT9V034_WINDOW_WIDTH,  width);  //读取图像的列数  改变此处也可改变图像输出大小，不过会丢失视角
    MTV_IICWriteReg16(MT9V034_WINDOW_HEIGHT, height); //读取图像的行数  改变此处也可改变图像输出大小，不过会丢失视角

    MTV_IICWriteReg16(MT9V034_COLUMN_START, MT9V034_COLUMN_START_MIN);  //列开始
    MTV_IICWriteReg16(MT9V034_ROW_START, MT9V034_ROW_START_MIN);        //行开始

}

/*!
  * @brief    设置自动曝光时间
  *
  * @param    enable ： 1 使能自动曝光  自动曝光时间通过上面的宏定义修改
  *
  * @return   无
  *
  * @note     无
  *
  * @see      MT9V034_SetAutoExposure(CAMERA_AUTO_EXPOSURE);
  *
  * @date     2019/7/26 星期五
  */
void MT9V034_SetAutoExposure(char enable)
{
    unsigned short reg =0;
    SCCB_RegRead(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE,&reg);
    if(enable)
    {
        /* 开启自动曝光自动增益 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg|MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE);
        /* 最大曝光时间 修改这里可以修改比较暗时的图像整体亮度*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_EXPOSURE_REG, CAMERA_MAX_EXPOSURE_TIME);
        /* 最小曝光时间 修改这里可以修改遇到强光时的图像整体亮度*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MIN_EXPOSURE_REG, CAMERA_MIN_EXPOSURE_TIME);
        /* 最大增益 增大这里 图像偏暗的情况下保留更多的细节 但是可能产生噪点 0-60*/
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 20);
        /* 0xB0  用于AEC/AGC直方图像素数目,22560 最大44000  */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AGC_AEC_PIXEL_COUNT_REG, 22560);

    }
    else
    {
        /* 关闭自动曝光 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_AEC_AGC_ENABLE, reg&~(MT9V034_AEC_ENABLE|MT9V034_AGC_ENABLE));

        /* 0xAB  最大模拟增益     64 */
        SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 30);

        /* 0x0B 设置曝光时间 0–32765 */
        SCCB_RegWrite(MT9V034_I2C_ADDR,MT9V034_TOTAL_SHUTTER_WIDTH,CAMERA_EXPOSURE_TIME);

    }

}






/*********************************************************************
 *摄像头SCCB底层驱动
 *
 ***********************************************************************/


/*!
  * @brief    SCCB写16位寄存器
  *
  * @param    reg ： 寄存器
  * @param    val ： 写入的值
  *
  * @return   无
  *
  * @note     无
  *
  * @see      MTV_IICWriteReg16(0x20, 0x0003); //向MT9V034的0x20寄存器写入0x0003
  *
  * @date     2019/7/26 星期五
  */
void MTV_IICWriteReg16(unsigned char reg, unsigned short val)
{
	SCCB_RegWrite(MT9V034_I2C_ADDR, reg, val);
}


/*!
  * @brief    SCCB 初始化
  *
  * @param    无
  *
  * @return   无
  *
  * @note     SCCB IO在LQ_MT9V034.h中通过宏定义选择
  *
  * @see      SCCB_Init(); //初始化SCCB  IO
  *
  * @date     2019/7/26 星期五
  */
void SCCB_Init(void)
{
  PIN_InitConfig(MT9V034_SCL_PIN, PIN_MODE_OUTPUT, 1);
  PIN_InitConfig(MT9V034_SDA_PIN, PIN_MODE_OUTPUT, 1);
  IfxPort_setPinPadDriver(PIN_GetModule(MT9V034_SCL_PIN), PIN_GetIndex(MT9V034_SCL_PIN), IfxPort_PadDriver_cmosAutomotiveSpeed1);
  IfxPort_setPinPadDriver(PIN_GetModule(MT9V034_SDA_PIN), PIN_GetIndex(MT9V034_SDA_PIN), IfxPort_PadDriver_cmosAutomotiveSpeed1);
  MT9V034_SCL_High;
  MT9V034_SDA_High;
}



/*!
  * @brief    SCCB 延时
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      SCCB_Wait();
  *
  * @date     2019/7/26 星期五
  */
void SCCB_Wait(void)
{
	int i;
	for (i = 0; i < 500; ++i)
	{
		__asm("NOP"); /* delay */
	}
}

/*!
  * @brief    SCCB 开始时序
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      SCCB_Star();
  *
  * @date     2019/7/26 星期五
  */
void SCCB_Star(void)
{
	MT9V034_SCL_Out;
	MT9V034_SDA_Out;
	SCCB_Wait();
	MT9V034_SDA_High;
	MT9V034_SCL_High;
	SCCB_Wait();
	MT9V034_SDA_Low;
	SCCB_Wait();
	MT9V034_SCL_Low;
	SCCB_Wait();
}

/*!
  * @brief    SCCB 停止时序
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      SCCB_Star();
  *
  * @date     2019/7/26 星期五
  */
void SCCB_Stop(void)
{
	MT9V034_SCL_Out;
	MT9V034_SDA_Out;
	SCCB_Wait();
	MT9V034_SDA_Low;
	SCCB_Wait();
	MT9V034_SCL_High;
	SCCB_Wait();
	MT9V034_SDA_High;
	SCCB_Wait();
}

/*!
  * @brief    SCCB 发送一个字节
  *
  * @param    Data  ：发送字节
  *
  * @return   ack 应答信号
  *
  * @note     无
  *
  * @see      SCCB_SendByte(0x01);
  *
  * @date     2019/7/26 星期五
  */
unsigned char SCCB_SendByte(unsigned char Data)
{
  unsigned char i;
  unsigned char Ack;
  MT9V034_SDA_Out;
  for( i=0; i<8; i++)
  {
    if(Data & 0x80)
    {
    	MT9V034_SDA_High;
    }
    else
    {
    	MT9V034_SDA_Low;
    }
    Data <<= 1;
    SCCB_Wait();
    MT9V034_SCL_High;
    SCCB_Wait();
    MT9V034_SCL_Low;
    SCCB_Wait();
  }
  MT9V034_SDA_High;
  MT9V034_SDA_In;
  SCCB_Wait();

  MT9V034_SCL_High;
  SCCB_Wait();
  Ack = MT9V034_SDA_Data;
  MT9V034_SCL_Low;
  SCCB_Wait();
  return Ack;
}

/*!
  * @brief    SCCB 接收一个字节
  *
  * @param    无
  *
  * @return   接收到的字节
  *
  * @note     无
  *
  * @see      unsigned char data = SCCB_ReadByte();
  *
  * @date     2019/7/26 星期五
  */
unsigned char SCCB_ReadByte(void)
{
  unsigned char i;
  volatile unsigned char byte;
  MT9V034_SCL_Out;
  MT9V034_SDA_In; //使能输入
  for( i=0; i<8; i++)
  {
	MT9V034_SCL_Low;
	SCCB_Wait();
	MT9V034_SCL_High;
	SCCB_Wait();
	byte = (byte<<1)|(MT9V034_SDA_Data & 1);
  }
  MT9V034_SCL_Low;
  MT9V034_SDA_Out;
  SCCB_Wait();
  return byte;
}

/*!
  * @brief    SCCB 应答
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      SCCB_Ack();
  *
  * @date     2019/7/26 星期五
  */
static void SCCB_Ack(void)
{
	MT9V034_SCL_Out;
	MT9V034_SDA_Out;
	MT9V034_SCL_Low;
	MT9V034_SDA_Low;
	SCCB_Wait();
	MT9V034_SCL_High;
	SCCB_Wait();
	MT9V034_SCL_Low;
	SCCB_Wait();
}

/*!
  * @brief    SCCB 不应答
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      SCCB_NAck();
  *
  * @date     2019/7/26 星期五
  */
static void SCCB_NAck(void)
{
	MT9V034_SCL_Out;
	MT9V034_SDA_Out;
	MT9V034_SCL_Low;
	SCCB_Wait();
	MT9V034_SDA_High;
	SCCB_Wait();
	MT9V034_SCL_High;
	SCCB_Wait();
	MT9V034_SCL_Low;
	SCCB_Wait();
}

/*!
  * @brief    SCCB 写寄存器
  *
  * @param    Device  : 地址
  * @param    reg     : 寄存器
  * @param    Data    : 数据
  *
  * @return   ack
  *
  * @note     无
  *
  * @see      SCCB_RegWrite(MT9V034_I2C_ADDR, MT9V034_MAX_GAIN_REG, 30);
  *
  * @date     2019/7/26 星期五
  */
void SCCB_RegWrite(unsigned char Device,unsigned char reg,unsigned short Data)
{
  unsigned char i;
  unsigned char Ack;

  for( i=0; i<3; i++)
  {
    SCCB_Star();
    Ack = SCCB_SendByte(Device);
    if( Ack == 1 )
    {
      continue;
    }

    Ack = SCCB_SendByte(reg);
    if( Ack == 1 )
    {
      continue;
    }

    SCCB_SendByte((unsigned char)(Data>>8));
    Ack = SCCB_SendByte((unsigned char)Data);
    if( Ack == 1 )
    {
      continue;
    }

    SCCB_Stop();
    if( Ack == 0 ) break;
  }
}

/*!
  * @brief    SCCB 读寄存器
  *
  * @param    Device  : 地址
  * @param    reg     : 寄存器
  * @param    Data    : 读出数据
  *
  * @return   ack
  *
  * @note     无
  *
  * @see      SCCB_RegRead(MT9V034_I2C_ADDR>>1,MT9V034_CHIP_VERSION,&data)
  *
  * @date     2019/7/26 星期五
  */
unsigned char SCCB_RegRead(unsigned char Device,unsigned char reg,unsigned short *Data)
{
  unsigned char Ack = 0;
  Device = Device<<1;
  SCCB_Star();
  Ack += SCCB_SendByte(Device);

  Ack += SCCB_SendByte(reg);

  SCCB_Star();
  Ack += SCCB_SendByte(Device + 1);

  *Data = SCCB_ReadByte();
  SCCB_Ack();
  *Data = *Data<<8;

  *Data += SCCB_ReadByte();
  SCCB_NAck();

  SCCB_Stop();

  return  Ack;
}



