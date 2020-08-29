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

#include <LQ_I2C_9AX.h>
#include <LQ_OLED096.h>
#include <LQ_SOFTI2C.h>
#include <LQ_STM.h>
#include <stdio.h>

/**
  * @brief    九轴测试
  *
  * @param
  *
  * @return
  *
  * @note
  *
  * @example
  *
  * @date     2019/6/13 星期四
  */
void Test_9AX(void)
{
    char  txt[30];
	short aacx,aacy,aacz;	         //加速度传感器原始数据
	short gyrox,gyroy,gyroz;        //陀螺仪原始数据
    short magx, magy, magz;         //地磁计原始数据

    OLED_Init();                      //LCD初始化
    OLED_CLS();                       //LCD清屏

    IIC_Init();


    OLED_P8x16Str(15,0,"LQ 9AX Test");

    if(LQ9AX_Init())
    {

    	OLED_P8x16Str(0,2,"9AX Test Fail");
#pragma warning 557         // 屏蔽警告
		while (1);
#pragma warning default     // 打开警告
    }

    while(1)
    {
        FX_8700_GetACCRaw(&aacx,&aacy,&aacz);	  //得到加速度传感器数据
        FX_8700_GetMAGRaw(&magx, &magy, &magz);
        FX_21002_GetRaw(&gyrox, &gyroy, &gyroz);

        sprintf((char*)txt,"ax:%06d",aacx);
        OLED_P6x8Str(0,2,txt);
        sprintf((char*)txt,"ay:%06d",aacy);
        OLED_P6x8Str(0,3,txt);
        sprintf((char*)txt,"az:%06d",aacz);
        OLED_P6x8Str(0,4,txt);
        sprintf((char*)txt,"gx:%06d",gyrox);
        OLED_P6x8Str(0,5,txt);
        sprintf((char*)txt,"gy:%06d",gyroy);
        OLED_P6x8Str(0,6,txt);
        sprintf((char*)txt,"gz:%06d",gyroz);
        OLED_P6x8Str(0,7,txt);
        sprintf((char*)txt,"MX:%5d ",magx);
        OLED_P6x8Str(60,5,txt);
        sprintf((char*)txt,"MY:%5d ",magy);
        OLED_P6x8Str(60,6,txt);
        sprintf((char*)txt,"MZ:%5d ",magz);
        OLED_P6x8Str(60,7,txt);

        delayms(100);

    }

}


/**
  * @brief    不精确延时
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @example  delayms_9ax(100);
  *
  * @date     2019/4/22 星期一
*/
void delayms_9ax(signed short ms)
{
	volatile uint32_t i = 0;
	while(ms--)
	{
		for (i = 0; i < 30000; ++i)
		{
			__asm("NOP"); /* delay */
		}
	}
}



/*!
  * @brief    九轴寄存器初始化
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      LQ9AX_Init();
  *
  * @date     2019/6/13 星期四
  */
unsigned char LQ9AX_Init(void)
{
    FX_8700_Active(0);
    delayms_9ax(10);

    if(FX_8700_Check())
    {
//        PRINTF("\n FX_8700 Init Fail \n");
        return 1;
    }
    FX_8700_SetRate(800, 2);   // 800Hz  地磁和加速度计都使能
    FX_8700_SetRange(4);       //± 4g
    FX_8700_Active(1);


    FX_21002_Active(0);
    delayms_9ax(10);

    if(FX_21002_Check())
    {
//        PRINTF("\n FX_2100 Init Fail \n");
        return 2;
    }
    FX_21002_SetRate(800);     // 800Hz  角速度计
    FX_21002_SetRange(2000);   //± 2000dps
    FX_21002_SetLP(2);         //低通滤波
    FX_21002_Active(1);
    delayms_9ax(10);
    return 0;
}










/*!
  * @brief    设置FXOS8700 是否激活
  *
  * @param    enable :  1：激活  0：待机
  *
  * @return   0 : 成功  其他 ： 失败
  *
  * @note     无
  *
  * @see      FX_8700_Active(1); //使能
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_8700_Active(unsigned char enable)
{
    /* 开启高分辨率模式 */
    FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_CTRL_REG2, 0x02);

    unsigned char reg = FX_Read_Byte(FXOS8700_ADDR, FXOS8701CQ_CTRL_REG1);
    if(enable)
    {
        reg = reg | FXOS8701CQ_CTRL_REG1_ACTIVE;
    }
    else
    {
        reg = reg & ~FXOS8701CQ_CTRL_REG1_ACTIVE;
    }
    return FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_CTRL_REG1, reg);
}

/*!
  * @brief    获取加速度计原始数据
  *
  * @param    ax,ay,az:加速度计x,y,z轴的原始读数(带符号)
  *
  * @return   0 : 成功  其他 ： 失败
  *
  * @note     无
  *
  * @see      int16_t data[3];
  * @see      FX_8700_GetACCRaw(&data[0], &data[1], &data[2]);//获取加速度原始数据
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_8700_GetACCRaw(short *ax,short *ay,short *az)
{
    unsigned char buf[6],res;
    res = FX_Read_Len(FXOS8700_ADDR, FXOS8701CQ_OUT_X_MSB, 6, buf);
    if(res==0)
	{
		*ax = -((signed short)buf[0]<<8)|buf[1];
		*ay = -((signed short)buf[2]<<8)|buf[3];
		*az = ((signed short)buf[4]<<8)|buf[5];
	}
    return res;
}


/*!
  * @brief    获取地磁计计原始数据
  *
  * @param    mx,my,mz:地磁计x,y,z轴的原始读数(带符号)
  *
  * @return   0 : 成功  其他 ： 失败
  *
  * @note     无
  *
  * @see      int16_t data[3];
  * @see      FX_8700_GetMAGRaw(&data[0], &data[1], &data[2]);//获取地磁计原始数据
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_8700_GetMAGRaw(short *mx,short *my,short *mz)
{
    unsigned char buf[6],res;
    res = FX_Read_Len(FXOS8700_ADDR, FXOS8701CQ_M_OUT_X_MSB, 6, buf);
    if(res==0)
	{
		*mx = -((signed short)buf[0]<<8)|buf[1];
		*my = -((signed short)buf[2]<<8)|buf[3];
		*mz = ((signed short)buf[4]<<8)|buf[5];
	}
    return res;
}



/*!
  * @brief    设置FXOS8700 量程
  *
  * @param    dps :  设置量程 单位 度每秒
  *
  * @return   0 : 成功  其他 ： 失败
  *
  * @note     地磁计和加速度计 量程是一起设定的 ±2g;±0.244 mg/LSB   ±4g;±0.488 mg/LSB   ±8g ±0.976 mg/LSB
  *
  * @see      FX_8700_SetRange(4); //设置量程 ±4g
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_8700_SetRange(unsigned char mg)
{
    unsigned char reg = FX_Read_Byte(FXOS8700_ADDR, FXOS8701CQ_XYZ_DATA_CFG);
    reg = reg & ~XYZ_DATA_CFG_FS_MASK;
    if(mg <= 2)
    {
        reg |= XYZ_DATA_CFG_FS_2G;
    }
    else if(mg <= 4)
    {
        reg |= XYZ_DATA_CFG_FS_4G;
    }
    else
    {
        reg |= XYZ_DATA_CFG_FS_8G;
    }

    return FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_XYZ_DATA_CFG, reg);

}


/*!
  * @brief    设置FXOS8700 采样速率
  *
  * @param    fps  设置输出速率 单位 1秒刷新多少次
  * @param    mode 0: 仅使用加速度计  1：仅使用地磁计  2：加速度计和地磁计都用（会导致输出速率减半）
  *
  * @return   0 : 成功  其他 ： 失败
  *
  * @note     无
  *
  * @see      FX_8700_SetRate(800, 2);   // 800Hz  地磁和加速度计都使能
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_8700_SetRate(signed short fps, unsigned char mode)
{
    if(mode == 0)
    {
        FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_M_CTRL_REG1, 0x00);
    }
    else if(mode == 1)
    {
        FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_M_CTRL_REG1, 0x01);
    }
    else
    {
        FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_M_CTRL_REG1, 0x40|0x1C|0x03);
    }
    unsigned char reg = FX_Read_Byte(FXOS8700_ADDR, FXOS8701CQ_CTRL_REG1);
    reg &= ~CTRL_REG_DR_MASK;

    if(fps <= 6)
    {
        reg |= CTRL_REG1_DR(6);
    }
    else if(fps <= 12)
    {
        reg |= CTRL_REG1_DR(5);
    }
    else if(fps <= 50)
    {
        reg |= CTRL_REG1_DR(4);
    }
    else if(fps <= 100)
    {
        reg |= CTRL_REG1_DR(3);
    }
    else if(fps <= 200)
    {
        reg |= CTRL_REG1_DR(2);
    }
    else if(fps <= 400)
    {
        reg |= CTRL_REG1_DR(1);
    }
    else
    {
        reg |= CTRL_REG1_DR(0);
    }
    reg &= ~0x02;
    return FX_Write_Byte(FXOS8700_ADDR, FXOS8701CQ_CTRL_REG1, reg);
}


/*!
  * @brief    FXOS8700 自检
  *
  * @param    无
  *
  * @return   0 : 正常   其他：错误
  *
  * @note     无
  *
  * @see      FX_8700_Check(); //获取ID 检查是否连接正常
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_8700_Check(void)
{
    unsigned char ID = 0;

    ID = FX_Read_Byte(FXOS8700_ADDR, FXOS8701CQ_WHOAMI);
    if(FXOS8700CQ_WHOAMI_VAL == ID || FXOS8701CQ_WHOAMI_VAL == ID)
    {
//        PRINTF("\n FXOS8700 is OK\n");
        return 0;
    }
    else
    {
//        PRINTF("\n FXOS8700 is Failed\n\rCheck ID = %x \n" , ID);
    }
    return 1;
}


/*--------------------------------------------------------------------------------------
----------------------------------FXAS21002---------------------------------------------
----------------------------------------------------------------------------------------*/




/*!
  * @brief    获取角速度计原始数据
  *
  * @param    gyrox,gyroy,gyroz:陀螺仪x,y,z轴的原始读数(带符号)
  *
  * @return   0 : 正常   其他：错误
  *
  * @note     无
  *
  * @see      int16_t data[3];
  * @see      FX_21002_GetRaw(&data[0], &data[1], &data[2]);//获取陀螺仪原始数据
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_21002_GetRaw(short * gyrox, short *gyroy, short *gyroz)
{
    unsigned char buf[6],res;
    res = FX_Read_Len(FXAS21002C_ADDR, FXAS21002C_OUT_X_MSB, 6, buf);
    if(res==0)
	{
		*gyrox = ((signed short)buf[0]<<8)|buf[1];
		*gyroy = ((signed short)buf[2]<<8)|buf[3];
		*gyroz = ((signed short)buf[4]<<8)|buf[5];
	}
    return res;
}


/*!
  * @brief    设置FXAS21002 是否激活
  *
  * @param    enable :  1：激活  0：待机
  *
  * @return   0 : 正常   其他：错误
  *
  * @note     无
  *
  * @see      FX_21002_Active(1); //使能FX_21002
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_21002_Active(unsigned char enable)
{
    unsigned char reg = FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG1);
    if(enable)
    {
        reg = reg | CTRL_REG1_ACTIVE;
    }
    else
    {
        reg = reg & ~CTRL_REG1_READY;
    }
    return FX_Write_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG1, reg);
}



/*!
  * @brief    设置FXAS21002 高通 滤波
  *
  * @param    BW :  设置截止频率（具体频率看表）
  *
  * @return   0 : 正常   其他：错误
  *
  * @note     不同采样频率下  高通滤波可以设置的值入下表
  *
  *    mode        采样速率    800 Hz 400 Hz 200 Hz  100 Hz 50 Hz  25 Hz 12.5 Hz
  *     0          截止频率      15    7.5    3.75   1.875  0.937 0.468  0.234
  *     1          截止频率     7.7   3.85   1.925   0.963  0.481 0.241  0.120
  *     2          截止频率     3.9   1.95   0.975   0.488  0.244 0.122  0.061
  *     3          截止频率    1.98   0.99   0.495   0.248  0.124 0.062  0.031
  *
  * @see      FX_21002_SetHP(0); //设置高通滤波
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_21002_SetHP(unsigned char mode)
{
    unsigned char reg = FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG0);
    reg = reg | CTRL_REG0_HPF_EN;
    reg |= (mode << CTRL_REG0_SEL_SHIFTS);
    return FX_Write_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG0, reg);
}



/*!
  * @brief    设置FXAS21002 低通 滤波
  *
  * @param    BW :  设置截止频率（具体频率看表）
  *
  * @return   0 : 正常   其他：错误
  *
  * @note     不同采样频率下  低通滤波可以设置的值入下表
  *  mode     采样速率  800 Hz  400 Hz  200 Hz  100 Hz  50 Hz  25 Hz  12.5 Hz
  *  0        截止频率  256     128      64      32     16      8        4
  *  1        截止频率  128      64      32      16      8      4       —
  *  2        截止频率   64      32      16       8      4     —       —
  *
  * @see      FX_21002_SetLP(0) //如果采样速率 800Hz 则低通滤波截止频率 256
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_21002_SetLP(unsigned char mode)
{
    unsigned char reg = FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG0);
    reg = reg & ~CTRL_REG0_BW_MASK;
    reg |= (mode << CTRL_REG0_BW_SHIFTS);
    return FX_Write_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG0, reg);
}



/*!
  * @brief    设置FXAS21002 量程
  *
  * @param    dps :  设置量程 单位 度每秒
  *
  * @return   0 : 正常   其他：错误
  *
  * @note     ±250dps;1,±500dps;2,±1000dps;3,±2000dps
  *
  * @see      FX_21002_SetRange(2000); //设置量程 ±2000dps
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_21002_SetRange(unsigned short dps)
{
    unsigned char reg = FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG0);
    reg = reg & ~CTRL_REG0_FS_MASK;
    if(dps <= 250)
    {
        reg |= CTRL_REG0_FS_250_DPS;
    }
    else if(dps <= 500)
    {
        reg |= CTRL_REG0_FS_500_DPS;
    }
    else if(dps <= 1000)
    {
        reg |= CTRL_REG0_FS_1000_DPS;
    }
    else
    {
        reg |= CTRL_REG0_FS_2000_DPS;
    }

    return FX_Write_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG0, reg);

}


/*!
  * @brief    设置FXAS21002 采样速率
  *
  * @param    fps 设置输出速率 单位 1秒刷新多少次
  *
  * @return   0 : 正常   其他：错误
  *
  * @note     无
  *
  * @see      FX_21002_SetRate(800); //设置采样率800Hz
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_21002_SetRate(unsigned short fps)
{
    unsigned char reg = FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG1);
    reg &= ~CTRL_REG_DR_MASK;

    if(fps <= 13)
    {
        reg |= CTRL_REG_DR_12_5;
    }
    else if(fps <= 25)
    {
        reg |= CTRL_REG_DR_25HZ;
    }
    else if(fps <= 50)
    {
        reg |= CTRL_REG_DR_50HZ;
    }
    else if(fps <= 100)
    {
        reg |= CTRL_REG_DR_100HZ;
    }
    else if(fps <= 200)
    {
        reg |= CTRL_REG_DR_200HZ;
    }
    else if(fps <= 400)
    {
        reg |= CTRL_REG_DR_400HZ;
    }
    else
    {
        reg |= CTRL_REG_DR_800HZ;
    }
    return FX_Write_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG1, reg);
}



/*!
  * @brief    FXAS21002 自检
  *
  * @param    无
  *
  * @return   0 : 正常   其他：错误
  *
  * @note     无
  *
  * @see      FX_21002_Check(); //自检
  *
  * @date     2019/6/13 星期四
  */
unsigned char FX_21002_Check(void)
{
    unsigned char ID = 0;

    ID = FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_WHO_AM_I);
    if(FXAS21002C_ID == ID)
    {
//        PRINTF("\n FXAS21002 is OK\n");
        return 0;
    }
    else
    {
//        PRINTF("\n FXAS21002 is Failed\n\rCheck ID = %x \n" , ID);
    }
    return 1;
}








/*--------------------------------------------------------------------------------------
----------------------------------底层驱动行数------------------------------------------
----------------------------------------------------------------------------------------*/





/*!
  * @brief    IIC 连续读
  *
  * @param    addr:器件地址
  * @param    reg :要读取的寄存器地址
  * @param    len :要读取的长度
  * @param    buf :读取到的数据存储区
  *
  * @return   0 ：读取成功
  *
  * @note     移植时如果使用模拟IIC 需要修改这里
  *
  * @see      unsigned char buf[6],res;
  * @see      res = FX_Read_Len(FXAS21002C_ADDR, FXAS21002C_OUT_X_MSB, 6, buf);//读取陀螺仪数据
  *
  * @date     2019/6/12 星期三
  */
unsigned char FX_Read_Len(unsigned char addr,unsigned char reg,unsigned char len,unsigned char *buf)
{
    return IIC_ReadMultByteFromSlave(addr<<1, reg, len, buf);
}



/*!
  * @brief    IIC 写一个寄存器
  *
  * @param    addr  :器件地址
  * @param    reg   :要写的寄存器地址
  * @param    value :要写入的值
  *
  * @return   0 ：读取成功
  *
  * @note     移植时如果使用模拟IIC 需要修改这里
  *
  * @see      FX_Write_Byte(FXAS21002C_ADDR, FXAS21002C_CTRL_REG1, 0x00); //写一个寄存器
  *
  * @date     2019/6/12 星期三
  */
unsigned char FX_Write_Byte(unsigned char addr,unsigned char reg,unsigned char value)
{
    return IIC_WriteByteToSlave(addr<<1, reg, value);
}


/*!
  * @brief    IIC 读一个寄存器
  *
  * @param    addr  :器件地址
  * @param    reg   :要读的寄存器地址
  *
  * @return   0 ：读取成功
  *
  * @note     移植时如果使用模拟IIC 需要修改这里
  *
  * @see      FX_Read_Byte(FXAS21002C_ADDR, FXAS21002C_WHO_AM_I); //读一个寄存器
  *
  * @date     2019/6/12 星期三
  */
unsigned char FX_Read_Byte(unsigned char addr,unsigned char reg)
{
    unsigned char value[1];
    FX_Read_Len(addr, reg, 1, value);
    return value[0];
}
