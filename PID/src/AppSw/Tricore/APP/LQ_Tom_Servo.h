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
#ifndef __LQ_SERVO_H_
#define __LQ_SERVO_H_


#define TOMSERVO1         IfxGtm_TOM0_0_TOUT32_P33_10_OUT
#define TOMSERVO2         IfxGtm_TOM0_13_TOUT35_P33_13_OUT


/*************************************************************************
*  函数名称：void LQ_Tom_Servo_2chPWM(void);
*  功能说明：测试舵机转动
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月22日
*  备    注：
*************************************************************************/
void LQ_Tom_Servo_2chPWM(void);



#endif
