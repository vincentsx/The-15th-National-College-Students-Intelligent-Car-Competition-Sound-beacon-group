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
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <Platform_Types.h>
#include "LQ_SOFTI2C.h"

/*************************************************************************
*  函数名称：IIC延时
*  功能说明：ADC初始化函数
*  参数说明：us      ：  延时时间
*  函数返回：无
*  修改时间：2020年3月10日
*  应用举例：iic_delay(1);  //简单的延时
*  内部调用 修改这里可以调整IIC速率
*************************************************************************/
void iic_delay()
{
	/* 200MHz 系统时钟下 模拟IIC速度为 400Khz */
	
	uint8  i = 0;
	for(i = 0; i < 30; i++) //修改这里可以调整IIC速率
	{
		__asm("NOP"); /* delay */
	}
}

/*************************************************************************
*  函数名称：void IIC_Init(void)
*  功能说明：模拟IIC初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  应用举例：IIC_Init();  //模拟IIC初始化 IIC管脚在LQ_SOFTI2C.h中定义
*************************************************************************/
void IIC_Init(void)
{			
    IIC_SCL_INIT;
    IIC_SDA_INIT;
    IIC_SCL_H; 
    IIC_SDA_H;
}

/*************************************************************************
*  函数名称：void IIC_Start(void)
*  功能说明：模拟IIC起始信号
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  应用举例：IIC_Start();
*************************************************************************/
void IIC_Start(void)
{
	SDA_OUT;   //sda线输出 
	IIC_SDA_H;	
	IIC_SCL_H;
	iic_delay();
	iic_delay();
	iic_delay();
	iic_delay();
	iic_delay();
 	IIC_SDA_L; //START:when CLK is high,DATA change form high to low 
	iic_delay();
	iic_delay();
	iic_delay();
	IIC_SCL_L; //钳住I2C总线，准备发送或接收数据 
}


/*************************************************************************
*  函数名称：void IIC_Stop(void)
*  功能说明：模拟IIC停止信号
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  应用举例：IIC_Stop();
*************************************************************************/
void IIC_Stop(void)
{
	SDA_OUT; //sda线输出
	IIC_SCL_L;
	IIC_SDA_L; //STOP:when CLK is high DATA change form low to high
    iic_delay();
	iic_delay();
	iic_delay();
	IIC_SCL_H; 
    iic_delay();
	iic_delay();
	iic_delay();
	IIC_SDA_H; //发送I2C总线结束信号
    iic_delay();							   	
}


/*************************************************************************
*  函数名称：uint8 IIC_WaitAck(void)
*  功能说明：模拟IIC等待应答信号
*  参数说明：无
*  函数返回：1，接收应答失败    0，接收应答成功
*  修改时间：2020年3月10日
*  应用举例：内部调用 有效应答：从机第9个 SCL=0 时 SDA 被从机拉低,并且 SCL = 1时 SDA依然为低
*************************************************************************/
uint8 IIC_WaitAck(void)
{
	uint8  ucErrTime=0;
	SDA_IN; //SDA设置为输入  （从机给一个低电平做为应答） 
	IIC_SDA_H;iic_delay();	   
	IIC_SCL_H;iic_delay();	 
	while(IIC_SDA_READ)
	{
		ucErrTime++;
		if(ucErrTime>100)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_L; //时钟输出0 	   
	return 0;  
} 

/*************************************************************************
*  函数名称：void IIC_Ack(void)
*  功能说明：模拟IIC产生ACK应答
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  应用举例：内部调用 主机接收完一个字节数据后，主机产生的ACK通知从机一个字节数据已正确接收
*************************************************************************/
void IIC_Ack(void)
{
	IIC_SCL_L;
	SDA_OUT;
	IIC_SDA_L;
	iic_delay();
	iic_delay();
	iic_delay();
	IIC_SCL_H;
	iic_delay();
	iic_delay();
	iic_delay();
	IIC_SCL_L;
}


/*************************************************************************
*  函数名称：void IIC_NAck(void)
*  功能说明：模拟IIC不产生ACK应答
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  应用举例：内部调用 主机接收完最后一个字节数据后，主机产生的NACK通知从机发送结束，释放SDA,以便主机产生停止信号
*************************************************************************/
void IIC_NAck(void)
{
	IIC_SCL_L;
	SDA_OUT;
	IIC_SDA_H;
	iic_delay();
	iic_delay();
	iic_delay();
	IIC_SCL_H;
	iic_delay();
	iic_delay();
	iic_delay();
	IIC_SCL_L;
}					 				     


/*************************************************************************
*  函数名称：void IIC_SendByte(uint8 data_t)
*  功能说明：模拟IIC发送一个字节
*  参数说明：data   :  发送的字节
*  函数返回：无
*  修改时间：2020年3月10日
*  应用举例：IIC_SendByte(0x12);
*************************************************************************/
void IIC_SendByte(uint8 data_t)
{                        
    uint8  t;
    SDA_OUT; 	    
    IIC_SCL_L; //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {   
//        IIC_SDA_READ = data_t&0x80;	
        if(data_t&0x80)
        {
            IIC_SDA_H;
        }
        else 
        {
            IIC_SDA_L;
        }
        
        IIC_SCL_H;;
		iic_delay();
        data_t<<=1;
        iic_delay();
		iic_delay();
        IIC_SCL_L;	 
		iic_delay();		
    }
    iic_delay();
} 	 


/*************************************************************************
*  函数名称：uint8 IIC_ReadByte(uint8 ack)
*  功能说明：模拟IIC读取一个字节
*  参数说明：ack=1 时，主机数据还没接收完 ack=0 时主机数据已全部接收完成
*  函数返回：接收到的字节
*  修改时间：2020年3月10日
*  应用举例：IC_ReadByte(0x12);
*************************************************************************/
uint8 IIC_ReadByte(uint8 ack)
{
	uint8  i,receive=0;
	SDA_IN; //SDA设置为输入模式 等待接收从机返回数据
    for(i=0;i<8;i++ )
	{
        IIC_SCL_L; 
		iic_delay();
		iic_delay();
		iic_delay();
        IIC_SCL_H;
        receive<<=1;
        if(IIC_SDA_READ)receive++; //从机发送的电平
        iic_delay();
    }					 
    if(ack)
        IIC_Ack(); //发送ACK 
    else
        IIC_NAck(); //发送nACK  
    return receive;
}

/*************************************************************************
*  函数名称：void ADC_init(void)
*  功能说明：模拟IIC读取指定设备 指定寄存器的一个值
*  参数说明：
  * @param    I2C_Addr  目标设备地址
  * @param    reg       目标寄存器
  * @param    buf       存放读出字节
*  函数返回：1失败 0成功
*  修改时间：2020年3月10日
*  应用举例：IIC_ReadByteFromSlave(0xD0, 0x75, &data);   //读 IIC地址为 0xD0器件（MPU6050）寄存器0x75
*************************************************************************/
uint8 IIC_ReadByteFromSlave(uint8 I2C_Addr,uint8 reg,uint8 *buf)
{
	IIC_Start();	
	IIC_SendByte(I2C_Addr);	 //发送从机地址
	if(IIC_WaitAck()) //如果从机未应答则数据发送失败
	{
		IIC_Stop();
		return 1;
	}
	IIC_SendByte(reg); //发送寄存器地址
	IIC_WaitAck();	  
	
	IIC_Start();
	IIC_SendByte(I2C_Addr+1); //进入接收模式			   
	IIC_WaitAck();
	*buf=IIC_ReadByte(0);	   
    IIC_Stop(); //产生一个停止条件
	return 0;
}
 

/*************************************************************************
*  函数名称：void ADC_init(void)
*  功能说明：模拟IIC写指定设备 指定寄存器的一个值
*  参数说明：
  * @param    I2C_Addr  目标设备地址
  * @param    reg       目标寄存器
  * @param    data      写入的数据
*  函数返回：1失败 0成功
*  修改时间：2020年3月10日
*  应用举例：IIC_ReadByteFromSlave(0xD0, 0X6B, 0X80);   //IIC地址为 0xD0器件（MPU6050）寄存器0x6B 写入0x80
*************************************************************************/
uint8 IIC_WriteByteToSlave(uint8 I2C_Addr,uint8 reg,uint8 data_t)
{
	IIC_Start();
	IIC_SendByte(I2C_Addr); //发送从机地址
	if(IIC_WaitAck())
	{
		IIC_Stop();
		return 1; //从机地址写入失败
	}
	IIC_SendByte(reg); //发送寄存器地址
    IIC_WaitAck();	  
	IIC_SendByte(data_t); 
	if(IIC_WaitAck())
	{
		IIC_Stop(); 
		return 1; //数据写入失败
	}
	IIC_Stop(); //产生一个停止条件
    
    //return 1; //status == 0;
	return 0;
}

/*************************************************************************
*  函数名称：uint8 IIC_ReadMultByteFromSlave(uint8 dev, uint8 reg, uint8 length, uint8 *data_t)
*  功能说明：模拟IIC读取指定设备 指定寄存器的n个值
*  参数说明：
  * @param    dev       目标设备地址
  * @param    reg       目标寄存器
  * @param    length    读取长度
  * @param    data      存放读出数据
*  函数返回：1失败 0成功
*  修改时间：2020年3月10日
*  应用举例：IIC_ReadByteFromSlave(0xD0, 0X3B, 14, &data);   //读 14个字节
*************************************************************************/
uint8 IIC_ReadMultByteFromSlave(uint8 dev, uint8 reg, uint8 length, uint8 *data_t)
{
    uint8  count = 0;
	uint8  temp;
	IIC_Start();
	IIC_SendByte(dev); //发送从机地址
	if(IIC_WaitAck())
	{
		IIC_Stop(); 
		return 1; //从机地址写入失败
	}
	IIC_SendByte(reg); //发送寄存器地址
    IIC_WaitAck();	  
	IIC_Start();
	IIC_SendByte(dev+1); //进入接收模式	
	IIC_WaitAck();
    for(count=0;count<length;count++)
	{
		if(count!=(length-1))
            temp = IIC_ReadByte(1); //带ACK的读数据
		else  
            temp = IIC_ReadByte(0); //最后一个字节NACK
        
		data_t[count] = temp;
	}
    IIC_Stop(); //产生一个停止条件
    //return count;
    return 0;
}

/*************************************************************************
*  函数名称：uint8 IIC_WriteMultByteToSlave(uint8 dev, uint8 reg, uint8 length, uint8* data_t)
*  功能说明：模拟IIC写指定设备 指定寄存器的n个值
*  参数说明：
  * @param    dev       目标设备地址
  * @param    reg       目标寄存器
  * @param    length    写入长度
  * @param    data      存放写入数据
*  函数返回： 1失败 0成功
*  修改时间：2020年3月10日
*  应用举例：IIC_WriteMultByteToSlave(0xD0, 0X6B, 1, 0X80);   //向寄存器0x6B写入0x80
*************************************************************************/
uint8 IIC_WriteMultByteToSlave(uint8 dev, uint8 reg, uint8 length, uint8* data_t)
{
    
 	uint8  count = 0;
	IIC_Start();
	IIC_SendByte(dev); //发送从机地址
	if(IIC_WaitAck())
	{
		IIC_Stop();
		return 1; //从机地址写入失败
	}
	IIC_SendByte(reg); //发送寄存器地址
    IIC_WaitAck();	  
	for(count=0;count<length;count++)
	{
		IIC_SendByte(data_t[count]); 
		if(IIC_WaitAck()) //每一个字节都要等从机应答
		{
			IIC_Stop();
			return 1; //数据写入失败
		}
	}
	IIC_Stop(); //产生一个停止条件
    
	return 0;
}



