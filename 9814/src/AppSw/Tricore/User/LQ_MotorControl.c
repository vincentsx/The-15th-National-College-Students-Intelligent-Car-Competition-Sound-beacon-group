/*!
  * @file     LQ_MotorControl.c
  *
  * @brief    电机控制
  *
  * @company  北京龙邱智能科技
  *
  * @author   LQ-005
  *
  * @note     Tab键 4个空格
  *
  * @version  V1.0
  *
  * @par URL  http://shop36265907.taobao.com
  *           http://www.lqist.cn
  *
  * @date     2020年6月5日
  */ 

#include <ANO_DT.h>
#include <IfxGtm_PinMap.h>
#include <LQ_GPT12_ENC.h>
#include <LQ_GTM.h>
#include <LQ_PID.h>
#include <stdint.h>

/*
 * 电机频率
 */
#define MOTOR_FREQUENCY    12500


/*
 * 电机pid参数        对应电机如下
 *
 *     3(左前)   2(右前)
 *
 *     4(左后)   1(右后)
 */
pid_param_t pid1, pid2, pid3, pid4;

/**
 * 电机PWM 宏定义
 */
#define MOTOR1_P          IfxGtm_ATOM0_3_TOUT56_P21_5_OUT
#define MOTOR1_N          IfxGtm_ATOM0_7_TOUT64_P20_8_OUT

#define MOTOR2_P          IfxGtm_ATOM0_6_TOUT42_P23_1_OUT
#define MOTOR2_N          IfxGtm_ATOM0_5_TOUT40_P32_4_OUT

#define MOTOR3_P          IfxGtm_ATOM0_0_TOUT53_P21_2_OUT
#define MOTOR3_N          IfxGtm_ATOM0_4_TOUT50_P22_3_OUT

#define MOTOR4_P          IfxGtm_ATOM0_1_TOUT54_P21_3_OUT
#define MOTOR4_N          IfxGtm_ATOM0_2_TOUT55_P21_4_OUT



/*!
  * @brief    电机初始化函数
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2020/6/8
  */
void MotorInit(void)
{
	ATOM_PWM_InitConfig(MOTOR1_P, 0, MOTOR_FREQUENCY);
	ATOM_PWM_InitConfig(MOTOR1_N, 0, MOTOR_FREQUENCY);
	ATOM_PWM_InitConfig(MOTOR2_P, 0, MOTOR_FREQUENCY);
	ATOM_PWM_InitConfig(MOTOR2_N, 0, MOTOR_FREQUENCY);
	ATOM_PWM_InitConfig(MOTOR3_P, 0, MOTOR_FREQUENCY);
	ATOM_PWM_InitConfig(MOTOR3_N, 0, MOTOR_FREQUENCY);
	ATOM_PWM_InitConfig(MOTOR4_P, 0, MOTOR_FREQUENCY);
	ATOM_PWM_InitConfig(MOTOR4_N, 0, MOTOR_FREQUENCY);

	PidInit(&pid1);
	PidInit(&pid2);
	PidInit(&pid3);
	PidInit(&pid4);

	/* PID 参数需要自行调节 */
	pid1.kp = 150;
	pid1.ki = 5;

	pid2.kp = 150;
	pid2.ki = 5;

	pid3.kp = 150;
	pid3.ki = 5;

	pid4.kp = 150;
	pid4.ki = 5;

}

/*!
  * @brief    编码器初始化函数
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2020/6/8
  */
void EncInit(void)
{
	ENC_InitConfig(ENC2_InPut_P33_7, ENC2_Dir_P33_6);
	ENC_InitConfig(ENC4_InPut_P02_8, ENC4_Dir_P33_5);
	ENC_InitConfig(ENC5_InPut_P10_3, ENC5_Dir_P10_1);
	ENC_InitConfig(ENC6_InPut_P20_3, ENC6_Dir_P20_0);
}


/*!
  * @brief    电机控制函数
  *
  * @param    motor1   ： 电机1占空比
  * @param    motor2   ： 电机2占空比
  * @param    motor3   ： 电机3占空比
  * @param    motor4   ： 电机4占空比
  *
  * @return   无
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2020/6/8
  */
void MotorCtrl(float motor1, float motor2, float motor3, float motor4)
{

	/* 限幅  防止电机转速反差过大  */
	static float motor1Last = 0, motor2Last = 0, motor3Last = 0, motor4Last = 0;
	motor1 = motor1Last + constrain_float(motor1 - motor1Last, -1000, 1000);
	motor2 = motor2Last + constrain_float(motor2 - motor2Last, -1000, 1000);
	motor3 = motor3Last + constrain_float(motor3 - motor3Last, -1000, 1000);
	motor4 = motor4Last + constrain_float(motor4 - motor4Last, -1000, 1000);

	/* 限幅  */
	constrain_float(motor1, -ATOM_PWM_MAX, ATOM_PWM_MAX);
	constrain_float(motor2, -ATOM_PWM_MAX, ATOM_PWM_MAX);
	constrain_float(motor3, -ATOM_PWM_MAX, ATOM_PWM_MAX);
	constrain_float(motor4, -ATOM_PWM_MAX, ATOM_PWM_MAX);

	motor1Last = motor1;
	motor2Last = motor2;
	motor3Last = motor3;
	motor4Last = motor4;

	if(motor1 > 0)
	{
		ATOM_PWM_SetDuty(MOTOR1_P, (uint32_t)motor1, MOTOR_FREQUENCY);
		ATOM_PWM_SetDuty(MOTOR1_N, 0, MOTOR_FREQUENCY);
	}
	else
	{
		ATOM_PWM_SetDuty(MOTOR1_P, 0, MOTOR_FREQUENCY);
		ATOM_PWM_SetDuty(MOTOR1_N, (uint32_t)(-motor1), MOTOR_FREQUENCY);
	}

	if(motor2 > 0)
	{
		ATOM_PWM_SetDuty(MOTOR2_P, (uint32_t)motor2, MOTOR_FREQUENCY);
		ATOM_PWM_SetDuty(MOTOR2_N, 0, MOTOR_FREQUENCY);
	}
	else
	{
		ATOM_PWM_SetDuty(MOTOR2_P, 0, MOTOR_FREQUENCY);
		ATOM_PWM_SetDuty(MOTOR2_N, (uint32_t)(-motor2), MOTOR_FREQUENCY);
	}

	if(motor3 > 0)
	{
		ATOM_PWM_SetDuty(MOTOR3_P, (uint32_t)motor3, MOTOR_FREQUENCY);
		ATOM_PWM_SetDuty(MOTOR3_N, 0, MOTOR_FREQUENCY);
	}
	else
	{
		ATOM_PWM_SetDuty(MOTOR3_P, 0, MOTOR_FREQUENCY);
		ATOM_PWM_SetDuty(MOTOR3_N, (uint32_t)(-motor3), MOTOR_FREQUENCY);
	}

	if(motor4 > 0)
	{
		ATOM_PWM_SetDuty(MOTOR4_P, (uint32_t)motor4, MOTOR_FREQUENCY);
		ATOM_PWM_SetDuty(MOTOR4_N, 0, MOTOR_FREQUENCY);
	}
	else
	{
		ATOM_PWM_SetDuty(MOTOR4_P, 0, MOTOR_FREQUENCY);
		ATOM_PWM_SetDuty(MOTOR4_N, (uint32_t)(-motor4), MOTOR_FREQUENCY);
	}
}


/*!
  * @brief    速度控制函数
  *
  * @param    x   ： x方向速度                          小车正右方
  * @param    y   ： y方向速度                          小车正前方
  * @param    z   ： 逆时针方向速度
  *
  * @return   无
  *
  * @note     无
  *
  * @see      无
  *
  * @date     2020/6/8
  */
void SpeedCtrl(int16_t x, int16_t y, int16_t z)
{
	/* 四个轮子的期望速度  */
	int16_t targetSpeed1 = 0, targetSpeed2 = 0, targetSpeed3 = 0, targetSpeed4 = 0;

	/* 四个轮子的实际速度  */
	int16_t  realSpeed1 = 0, realSpeed2 = 0, realSpeed3 = 0, realSpeed4 = 0;

	/* 根据小车速度大小和方向  计算每个轮子的期望转速  */
	targetSpeed1 =  x + y + z;
	targetSpeed2 = -x + y + z;
	targetSpeed3 =  x + y - z;
	targetSpeed4 = -x + y - z;

	/* 获取编码器值 */
	realSpeed1 = ENC_GetCounter(ENC2_InPut_P33_7);
	realSpeed2 = ENC_GetCounter(ENC4_InPut_P02_8);
	realSpeed3 = -ENC_GetCounter(ENC5_InPut_P10_3);
	realSpeed4 = -ENC_GetCounter(ENC6_InPut_P20_3);


	/* PID处理 */
	PidIncCtrl(&pid1, targetSpeed1 - realSpeed1);
	PidIncCtrl(&pid2, targetSpeed2 - realSpeed2);
	PidIncCtrl(&pid3, targetSpeed3 - realSpeed3);
	PidIncCtrl(&pid4, targetSpeed4 - realSpeed4);

	/* 电机控制 */
	MotorCtrl(pid1.out, pid2.out, pid3.out, pid4.out);

	/* 发送数据到匿名上位机画波形   方便调试PID参数 */
//	ANO_DT_send_int16(targetSpeed1, realSpeed1, (int16_t)(pid1.out), 0, 0, 0, 0, 0);

}
















