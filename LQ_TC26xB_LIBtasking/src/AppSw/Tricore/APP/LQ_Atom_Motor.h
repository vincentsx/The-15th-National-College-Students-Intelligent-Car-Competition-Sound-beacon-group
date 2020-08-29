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
#ifndef __LQ_MT_H_
#define __LQ_MT_H_

#define ATOMSERVO1       IfxGtm_ATOM2_0_TOUT32_P33_10_OUT
#define ATOMSERVO2       IfxGtm_ATOM2_5_TOUT35_P33_13_OUT

//#define ATOMPWM0Pin         IfxGtm_ATOM0_2_TOUT107_P10_5_OUT //定义PWM输出管脚，通过P10.5输出PWM控制蓝灯呼吸
#define ATOMPWM0         IfxGtm_ATOM0_6_TOUT42_P23_1_OUT
#define ATOMPWM1         IfxGtm_ATOM0_5_TOUT40_P32_4_OUT
#define ATOMPWM2         IfxGtm_ATOM0_0_TOUT53_P21_2_OUT
#define ATOMPWM3         IfxGtm_ATOM0_4_TOUT50_P22_3_OUT
#define ATOMPWM4         IfxGtm_ATOM0_2_TOUT55_P21_4_OUT
#define ATOMPWM5         IfxGtm_ATOM0_1_TOUT54_P21_3_OUT
#define ATOMPWM6         IfxGtm_ATOM0_7_TOUT64_P20_8_OUT
#define ATOMPWM7         IfxGtm_ATOM0_3_TOUT56_P21_5_OUT

void LQ_Atom_Motor_8chPWM(void);
void LQ_ATom_Servo_2chPWM(void);
void LQ_Atom_8chPWM(void);

#endif
