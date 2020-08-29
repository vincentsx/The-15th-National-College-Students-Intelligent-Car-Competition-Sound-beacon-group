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

#include <LQ_I2C_VL53.h>
#include <LQ_OLED096.h>
#include <LQ_SOFTI2C.h>
#include <LQ_STM.h>
#include <Platform_Types.h>
#include <stdio.h>

/* 选择屏幕 */
#define OLED

/**
  * @brief    测试VL53
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @example
  *
  * @date     2019/4/17 星期三
  */
void Test_Vl53(void)
{

#ifdef OLED
	OLED_Init();
	OLED_CLS();
	OLED_P8x16Str(5,0,"LQ VL53 Test");
#else
	TFTSPI_Init(1);                //TFT1.8初始化  0：横屏显示  1：竖屏显示
    TFTSPI_CLS(u16BLUE);           //清屏
	TFTSPI_P8X16Str(0,0,"LQ VL53 Test",u16RED,u16BLUE);
#endif
	char txt[16];

    IIC_Init();

    unsigned char VL53_STAR = 0x02;    //0x02 连续测量模式    0x01 单次测量模式
    unsigned char dis_buff[2];
    uint16 dis, last_dis = 0;
	/* 开始测距 */
    VL53_Write_Byte(VL53ADDR, VL53L0X_REG_SYSRANGE_START, VL53_STAR);
    while(1)
    {
        /* 获取测量数据 */
		VL53_Read_nByte(VL53ADDR, VL53_REG_DIS, 2, dis_buff);

        /* 转换数据 */
        dis = (dis_buff[0]<<8) | (dis_buff[1]);

        /* 超过2M没有障碍 */
        if(dis > 8000)
        {
            dis = 0;
        }

        if(dis == 20)
        {
            dis = last_dis;
        }
        last_dis = dis;
		sprintf(txt, "DIS %5d mm",dis);
#ifdef OLED
		OLED_P8x16Str(0,5,txt);
#else
		TFTSPI_P8X16Str(2,5,txt,u16RED,u16BLUE);
#endif
		delayms(50);


    }

}





/**
  * @brief    VL53 写1个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    data：   写入数据
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2019/4/29 星期一
  */
void VL53_Write_Byte(unsigned char dev, unsigned char reg, unsigned char data)
{
    IIC_WriteByteToSlave(dev<<1, reg, data);
}

/**
  * @brief    VL53 读n个寄存器
  *
  * @param    dev：    地址
  * @param    reg：    寄存器
  * @param    length;  长度
  * @param    data：   指向存放读数据
  *
  * @return
  *
  * @note
  *
  * @see
  *
  * @date     2019/4/29 星期一
  */
void VL53_Read_nByte(unsigned char dev, unsigned char reg, unsigned char length, unsigned char* data)
{
    IIC_ReadMultByteFromSlave(dev<<1, reg, length, data);
}

