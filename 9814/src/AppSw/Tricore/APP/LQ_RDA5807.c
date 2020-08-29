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

#include <IfxPort.h>
#include <LQ_GPIO.h>
#include <LQ_GPIO_KEY.h>
#include <LQ_GPIO_LED.h>
#include <LQ_STM.h>
#include <LQ_TFT18.h>
#include <stdio.h>
#include <LQ_RDA5807.h>

unsigned short RXFreq = 9450;

/*************************************************************************
*  函数名称：void RDA5807_Reset()
*  功能说明：RDA5807 软件复位
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：      RDA5807_Reset();
*************************************************************************/
void RDA5807_Reset(void)
{
	RDA5807_WriteReg(RDA_R02,0x0002|RDA5807_ReadReg(RDA_R03)); //软件复位
	delayms(55);
}


/*************************************************************************
*  函数名称：void RDA5807_SetFreq(unsigned short freq)
*  功能说明：RDA5807 设置收听频率
*  参数说明：freq   ： 收听频率  范围 6500~10800
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：RDA5807_SetFreq(9500);   //设置收听频率  95.0Mhz
*************************************************************************/
void RDA5807_SetFreq(unsigned short freq)
{
  unsigned short temp;
  unsigned char  spc = 0,band = 0;
  unsigned short fbtm, chan;

  temp = RDA5807_ReadReg(RDA_R03);
  temp &= 0x001F;
  band = (temp>>2) & 0x03;
  spc = temp & 0x03;
  if(spc == 0)
  {
	  spc=10;
  }
  else if(spc == 1)
  {
	  spc=20;
  }
  else
  {
	  spc=5;
  }

  if(band == 0)
  {
	  fbtm=8700;
  }
  else if(band==1||band==2)
  {
	  fbtm=7600;
  }
  else
  {
    fbtm = RDA5807_ReadReg(0x53);
    fbtm *= 10;
  }

  if(freq<fbtm)
  {
	  return;
  }
  chan = (freq - fbtm)/spc;
  chan &= 0x3FF;
  temp |= chan<<6;
  temp |= 1<<4;
  RDA5807_WriteReg(RDA_R03, temp) ;
  delayms(55);
}

/*************************************************************************
*  函数名称：void RDA5807_Enable(unsigned char flag)
*  功能说明：RDA5807 使能
*  参数说明：flag   ： 1 使能芯片 0 禁用芯片
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：RDA5807_Enable(1);//1 使能芯片
*************************************************************************/
void RDA5807_Enable(unsigned char flag)
{
  unsigned short temp;
  temp=RDA5807_ReadReg(RDA_R02);
  if(flag ==1 )
    temp |=1;
  if(flag == 0)
    temp &= ~0x1;
  RDA5807_WriteReg(RDA_R02,temp);
  delayms(200);
}
/*************************************************************************
*  函数名称：unsigned char RDA5807_GetRssi(void)
*  功能说明：读取RSSI
*  参数说明：无
*  函数返回：信号强度获取0~127
*  修改时间：2020年4月10日
*  备    注：RDA5807_Enable(1);   //使能RDA5807
*************************************************************************/
unsigned char RDA5807_GetRssi(void)	//信号强度获取		0~127
{
  unsigned short temp=0;
  temp=RDA5807_ReadReg(0x0B);
  temp=(temp>>9)&0x7f;
  return  ((unsigned char)temp);
}
/*************************************************************************
*  函数名称：void RDA5807_SetVol(unsigned char vol)
*  功能说明：RDA5807 使能
*  参数说明：vol  音量设置	 0~15
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：RDA5807_SetVol(12);	                   //音量设置0~15
*************************************************************************/
void RDA5807_SetVol(unsigned char vol)	//音量设置	 0~15
{
  unsigned short temp=0;
  temp=RDA5807_ReadReg(RDA_R05);
  temp&=0xfff0;
  RDA5807_WriteReg(RDA_R05,vol|temp) ;
  delayms(55);
}
/*************************************************************************
*  函数名称：void RDA5807_SetMute(unsigned char mute)	//静音设置 1为静音	0为不静音
*  功能说明：RDA5807 静音设置
*  参数说明：mute)	静音设置 1为静音	0为不静音
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：RDA5807_SetMute(1);	                   //1为静音
*************************************************************************/
void RDA5807_SetMute(unsigned char mute)	//静音设置 1为静音	0为不静音
{
  unsigned short temp=0;

  temp=RDA5807_ReadReg(0x02);
  if(!mute)
  {
	  RDA5807_Init();
	  RDA5807_SetFreq(RXFreq);
  }
  else
  {
	  temp&=0xBFFF;
	  RDA5807_WriteReg(RDA_R02,temp) ;
	  RDA5807_Reset(); //软件复位
  }
}

/*************************************************************************
*  函数名称：void RDA5807_SetSeekth(unsigned short rssi)
*  功能说明：RDA5807 //自动搜台信号阈值强度
*  参数说明：rssi 0~15 默认为8  数值越低搜到的台越多
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：RDA5807_SetSeekth(5);	//自动搜台信号阈值强度0~15 默认为5  数值越低搜到的台越多
*************************************************************************/
void RDA5807_SetSeekth(unsigned short rssi)	   //自动搜台信号阈值强度		0~15 默认为8  数值越低搜到的台越多
{
  unsigned short temp;
  rssi = rssi & 0xf;
  temp=RDA5807_ReadReg(0x05);
  temp&=~(0xf<<8);
  temp|= rssi<<8;
  RDA5807_WriteReg(RDA_R05,temp) ;
  delayms(55);
}

/*************************************************************************
*  函数名称：void RDA5807_Init()
*  功能说明：RDA5807 初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：
*************************************************************************/
void RDA5807_Init(void)
{
  FMI2C_Init();
  RDA5807_Enable(1);//上电使能
  RDA5807_Reset();  //软件复位
  RDA5807_WriteReg(RDA_R02,0xd281); //晶振选择32.768Khz 向上搜索重低音立体声
  delayms(50);
  RDA5807_WriteReg(RDA_R03,0x0500); //频率设为87.5MHz ，步进100Khz 频带为87M~108M
  delayms(50);
  RDA5807_WriteReg(RDA_R04,0x0040); //基本配置
  delayms(20);
  RDA5807_WriteReg(RDA_R05,0x8886); //音量设置为vol=6 搜索阀值为8
  delayms(20);
  RDA5807_SetVol(12);	                   //音量设置0~15
  RDA5807_SetSeekth(5);	               //自动搜台信号阈值强度0~15 默认为5  数值越低搜到的台越多
}

/*************************************************************************
*  函数名称：void Test_RDA5807()
*  功能说明：RDA5807 测试
*  参数说明：无
*  函数返回：无
*  修改时间：2020年4月10日
*  备    注：
*************************************************************************/
void Test_RDA5807(void)
{
  char txt[16];
  unsigned char  Key_num=0,Vol=10,RSSI=0,mute=1;


  RDA5807_Init();	      //RDA5807初始化
  GPIO_KEY_Init();

  TFTSPI_Init(1);         //LCD初始化  0:横屏  1：竖屏
  TFTSPI_CLS(u16BLACK);   //黑色屏幕

  //显示芯片ID 0x5804
  RXFreq = RDA5807_ReadReg(RDA_R00);
  sprintf(txt,"Chip:0x%04X",RXFreq);
  TFTSPI_P8X16Str(1,2,txt,u16WHITE,u16BLACK);		//字符串显示

  RSSI=RDA5807_GetRssi();//显示信号强度0~127
  sprintf(txt,"RSSI:%02d  ",RSSI);
  TFTSPI_P8X16Str(1,3,txt,u16WHITE,u16BLACK);		//字符串显示

  //设置收听频率 90.50
  RXFreq=9050;
  RDA5807_Reset();  //软件复位
  RDA5807_SetFreq(RXFreq);
  sprintf(txt,"Freq:%d.%02dMHz ",RXFreq/100,RXFreq%100);
  TFTSPI_P8X16Str(1,4,txt,u16WHITE,u16BLACK);		//字符串显示

  //显示音量
  sprintf(txt,"Volm:%02d  ",Vol);
  TFTSPI_P8X16Str(1,5,txt,u16WHITE,u16BLACK);		//字符串显示

  TFTSPI_P8X16Str(3,1,"FM Radio",u16WHITE,u16BLACK);//字符串显示

  while (1)
  {
	RSSI = RDA5807_GetRssi();//显示信号强度0~127
	sprintf(txt,"RSSI:%02d  ",RSSI);
	TFTSPI_P8X16Str(1,3,txt,u16WHITE,u16BLACK);		//字符串显示
    Key_num = KEY_Read_All();;
    switch(Key_num)
    {
        case 3: 			     //KEY0 KEY1 同时按下声音减
          if(Vol == 0)
            Vol = 0;
          else
            Vol -= 1;
          RDA5807_Init();
          RDA5807_SetVol(Vol);
          RDA5807_SetFreq(RXFreq);
          sprintf(txt,"Volm:%02d  ",Vol);
          TFTSPI_P8X16Str(1,5,txt,u16WHITE,u16BLACK);		//字符串显示
          break;
        case 6:
          if(Vol == 15) Vol = 15; 	 //KEY2 KEY1 同时按下声音加
          else Vol += 1;
          RDA5807_Init();
          RDA5807_SetVol(Vol);
          RDA5807_SetFreq(RXFreq);
          sprintf(txt,"Volm:%02d  ",Vol);
          TFTSPI_P8X16Str(1,5,txt,u16WHITE,u16BLACK);		//字符串显示
          break;

		case 1:        //KEY0 按下  频率减
		  if(RXFreq == 8700) RXFreq = 10800;
		  else RXFreq -= 10;
		  RDA5807_Reset();
		  RDA5807_SetFreq(RXFreq);
		  sprintf(txt,"Freq:%d.%02dMHz ",RXFreq/100, RXFreq%100);
		  TFTSPI_P8X16Str(1,4,txt,u16WHITE,u16BLACK);		//字符串显示
		  break;

		case 2:	                 //KEY1 按下  频率95.0
		  RXFreq = 9500;
		  RDA5807_Reset();
		  RDA5807_SetFreq(RXFreq);
		  sprintf(txt,"Freq:%d.%02dMHz ",RXFreq/100, RXFreq%100);
		  TFTSPI_P8X16Str(1,4,txt,u16WHITE,u16BLACK);		//字符串显示
		  break;

		case 4:        //KEY2 按下  频率加
		  if(RXFreq == 10800) RXFreq = 8700;
		  else RXFreq += 10;
		  RDA5807_Reset();
		  RDA5807_SetFreq(RXFreq);
		  sprintf(txt,"Freq:%d.%02dMHz ",RXFreq/100, RXFreq%100);
		  TFTSPI_P8X16Str(1,4,txt,u16WHITE,u16BLACK);		//字符串显示
		  break;

		case 7:        //KEY012 同时按下
		      if(mute==1)			//静音
		      {
		        RDA5807_SetMute(1);
		        TFTSPI_P8X16Str(1,6,"Mute:Yes!",u16WHITE,u16BLACK);		//字符串显示
		        mute=0;
		      }
		      else
		      {
		        RDA5807_SetMute(0);
		        TFTSPI_P8X16Str(1,6,"Mute:No! ",u16WHITE,u16BLACK);		//字符串显示
		        mute=1;
		      }
		      break;

		default:
		  break;
    }
    LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
    delayms(100);              //延时等待
  }
}





/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////RDA5807M 底层IIC驱动/////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////



void FMI2C_delay()        //用于FMI2C延时
{
  unsigned short  i = 0;
  for(i = 0; i <2000; i++) //修改这里可以调整FMI2C速率
  {
      __asm("NOP"); /* delay */
  }
}
/*************************************************************************
*  函数名称：void FMI2C_Init(void)
*  功能说明：模拟FMI2C初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  应用举例：FMI2C_Init();  //模拟FMI2C初始化 FMI2C管脚在LQ_SOFTI2C.h中定义
*************************************************************************/
void FMI2C_Init(void)
{
  FMI2C_SCL_INIT;
  FMI2C_SDA_INIT;
  FMI2C_SCL_H;
  FMI2C_SDA_H;
}

void FMI2Cstart() 		//FMI2C启动信号
{
  FMI2C_SDA_H;
  FMI2C_SCL_H;
  FMI2C_delay();
  FMI2C_delay();
  FMI2C_SDA_L;
  FMI2C_delay();
  FMI2C_SCL_L;
}
void FMI2Cstop()			//FMI2C停止信号
{
  FMI2C_SCL_L;
  FMI2C_SDA_L;
  FMI2C_delay();
  FMI2C_delay();
  FMI2C_SCL_H;
  FMI2C_delay();
  FMI2C_SDA_H;
  FMI2C_delay();
}

unsigned char FMI2C_Wait_Ack(void)	//FMI2C发送字节后 等待从机发送响应信
{
  unsigned short ucErrTime=0;

  FMSDA_IN;             //SDA设置为输入模式 等待接收从机返回数据
  FMI2C_SDA_H;
  FMI2C_delay();
  FMI2C_SCL_H;
  FMI2C_delay();
  while(FMI2C_SDA_READ)
  {
    ucErrTime++;
    if(ucErrTime>10000)
    {
      FMSDA_OUT;
      FMI2Cstop();
      return 1;
    }
  }
  FMI2C_SCL_L;
  FMSDA_OUT;
  return 0;
}
void FMI2C_Ack(void)	  //发送应答信号
{
  FMI2C_SCL_L;
  FMI2C_SDA_L;		  //0±íê?ó|′e
  FMI2C_delay();
  FMI2C_SCL_H;
  FMI2C_delay();
  FMI2C_SCL_L;
}

void FMI2C_NAck(void)	 //FMI2C 非应答信号
{
  FMI2C_SCL_L;
  FMI2C_SDA_H;
  FMI2C_delay();
  FMI2C_SCL_H;
  FMI2C_delay();
  FMI2C_SCL_L;
}
void FMI2CsendByte(unsigned char txd)		 //FMI2C?写一个字节
{
  unsigned char t;
  FMI2C_SCL_L;
  for(t=0;t<8;t++)
  {
    if(txd&0x80)
      FMI2C_SDA_H;
    else
      FMI2C_SDA_L;
    txd<<=1;
    FMI2C_delay();
    FMI2C_SCL_H;
    FMI2C_delay();
    FMI2C_SCL_L;
    FMI2C_delay();
  }
  FMI2C_delay();
}
unsigned char FMI2CReadByte(unsigned char ack)  //FMI2C读一个字节
{
  unsigned char i,receive=0;
  //FMI2C_SDA_H;		  //单片机讲引脚置高可设为输入引脚
  FMSDA_IN;             //SDA设置为输入模式 等待接收从机返回数据
  FMI2C_delay();
  for(i=0;i<8;i++ )
  {
    FMI2C_SCL_L;
    FMI2C_delay();
    FMI2C_SCL_H;
    receive<<=1;
    if(FMI2C_SDA_READ) receive++;
    FMI2C_delay();
  }
  FMSDA_OUT;
  if (!ack)
    FMI2C_NAck();
  else
    FMI2C_Ack();
  return receive;
}
unsigned short RDA5807_ReadReg(unsigned char regAddr)			//芯片读寄存器
{
  unsigned short buf;
  FMI2Cstart();
  FMI2CsendByte(RDA_WRITE);	  //发送芯片地址 方向为写
  FMI2C_Wait_Ack();
  FMI2CsendByte(regAddr);	      //发送寄存器地址
  FMI2C_Wait_Ack();

  FMI2Cstart();
  FMI2CsendByte(RDA_READ); 	  // 发送芯片地址 方向为读
  FMI2C_Wait_Ack();
  buf = FMI2CReadByte(1);
  buf = buf<<8;
  buf =buf|FMI2CReadByte(0);
  FMI2Cstop();
  return buf;
}


void RDA5807_WriteReg(unsigned char regAddr,unsigned short val) //芯片写寄存器
{
  FMI2Cstart();
  FMI2CsendByte(RDA_WRITE);	//发送芯片地址 方向为写
  FMI2C_Wait_Ack();
  FMI2CsendByte(regAddr);	    //发送寄存器地址
  FMI2C_Wait_Ack();
  FMI2CsendByte(val>>8);
  FMI2C_Wait_Ack();
  FMI2CsendByte(val&0xFF);
  FMI2C_Wait_Ack();
  FMI2Cstop();
}
