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

#include <LQ_GPIO_LED.h>
#include <LQ_STM.h>
#include <LQ_UART.h>

//////////////////////////////////以下部分为功能测试/////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////




/*************************************************************************
*  函数名称：void Test_Bluetooth(void);
*  功能说明：UART测试函数
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：
*************************************************************************/
void Test_Bluetooth(void)
{
	UART_InitConfig(UART0_RX_P14_1,UART0_TX_P14_0, 115200);
	UART_InitConfig(UART1_RX_P02_3,UART1_TX_P02_2, 9600);
	UART_InitConfig(UART2_RX_P14_3,UART2_TX_P14_2, 19200);
	UART_InitConfig(UART3_RX_P00_1,UART3_TX_P00_0, 115200);
	while(1)
	{
		UART_PutChar(UART0, 'L');                      //发送 字节到UART口
		delayms(500);                                  //延时等待
		UART_PutChar(UART0,'Q');                       //发送 字节到UART口

		UART_PutStr(UART0,"UART0 LongQiu \r\n");                //发送字符串到上位机
		UART_PutStr(UART1,"UART1 LongQiu \r\n");                //发送字符串到上位机
		UART_PutStr(UART2,"UART2 LongQiu \r\n");                //发送字符串到上位机
		UART_PutStr(UART3,"UART3 LongQiu \r\n");                //发送字符串到上位机

    	LED_Ctrl(LED0,RVS);        //电平翻转,LED闪烁
		delayms(500);              //延时等待
	}
}

//
