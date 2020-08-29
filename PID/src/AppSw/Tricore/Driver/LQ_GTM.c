/*LLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL
【平    台】北京龙邱智能科技TC264DA核心板
【编    写】ZYF/chiusir
【E-mail  】chiusir@163.com
【软件版本】V1.1 版权所有，单位使用请先联系授权
【最后更新】2020年5月10日
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
通用定时器模块（GTM）
有4个ATOM子模块，每个子模块有8路可独立配置的通道
有3个TIM子模块，每个子模块有8路可独立配置的通道
有2个TOM子模块，每个子模块有16路可独立配置的通道
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/


#include <LQ_GTM.h>


/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////ATOM PWM功能函数    //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

//PWM 配置结构体
IfxGtm_Atom_Pwm_Driver g_AtomDriverPWM[32];
IfxGtm_Atom_Pwm_Config g_atomConfig;

/*************************************************************************
*  函数名称：void ATOM_PWM_InitConfig(IfxGtm_Atom_ToutMap pin, uint32_t duty, uint32_t pwmFreq_Hz)
*  功能说明：初始化ATOM为PWM输出功能
*  参数说明：
  * @param    pin            ：    PWM通道  IfxGtm_PinMap.h中 已经定义了对应的PWM管脚
  * @param    duty           ：    占空比 * ATOM_PWM_MAX
  * @param    pwmFreq_Hz     ：    频率
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：PWM_InitConfig(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 5000, 100);//初始化P21_2 作为PWM输出口 频率100Hz 占空比 百分之(5000/ATOM_PWM_MAX)*100
*************************************************************************/
void ATOM_PWM_InitConfig(IfxGtm_Atom_ToutMap pin, uint32_t duty, uint32_t pwmFreq_Hz)
{
	static uint8_t count = 0;

	if(count == 0)
	{
		IfxGtm_enable(&MODULE_GTM); /* 使能 GTM */

		IfxGtm_Cmu_setClkFrequency(&MODULE_GTM, IfxGtm_Cmu_Clk_0, ATOM_PWM_CLK); //设置 CMU clock 100M 频率
		IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_CLK0);             //使能 CMU clock 0

		count ++;
	}


	IfxGtm_Atom_Pwm_initConfig(&g_atomConfig, &MODULE_GTM);

	g_atomConfig.atom = pin.atom;                                //选择PWM输出管脚
	g_atomConfig.atomChannel = pin.channel;                      //选择PWM输出管脚对应的通道
	g_atomConfig.pin.outputPin = &pin;                           //设置输出管脚
	g_atomConfig.period = ATOM_PWM_CLK / pwmFreq_Hz;             //设置输出周期
	g_atomConfig.dutyCycle = (uint32)(duty * ((float)g_atomConfig.period / ATOM_PWM_MAX));//设置占空比
	g_atomConfig.synchronousUpdateEnabled = TRUE;                        //使能PWM同步更新

	uint8 tempNum = (uint8)pin.atom * 8 + (uint8)pin.channel;

	IfxGtm_Atom_Pwm_init(&g_AtomDriverPWM[tempNum], &g_atomConfig);      //ATOM_PWM初始化
	IfxGtm_Atom_Pwm_start(&g_AtomDriverPWM[tempNum], TRUE);              //开始输出PWM


}

/*************************************************************************
*  函数名称：void ATOM_PWM_SetDuty(IfxGtm_Atom_ToutMap pin, uint32_t duty, uint32_t pwmFreq_Hz)
*  功能说明：设置PWM频率和占空比
*  参数说明：
   pin            ：PWM通道 IfxGtm_PinMap.h中 已经定义了对应的PWM管脚
   duty           ：占空比 * ATOM_PWM_MAX
   pwmFreq_Hz     : PWM频率    注意此函数不可以设置频率  如果要设置频率    需打开设置频率代码注释
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：ATOM_PWM_SetDuty(IfxGtm_ATOM0_0_TOUT53_P21_2_OUT, 5000, 1000);//设置P21_2  频率 1000hz 占空比 百分之(5000/ATOM_PWM_MAX)*100
*************************************************************************/
void ATOM_PWM_SetDuty(IfxGtm_Atom_ToutMap pin, uint32_t duty, uint32_t pwmFreq_Hz)
{
	uint32 period = ATOM_PWM_CLK / pwmFreq_Hz;                         //设置输出周期
	uint32 dutyCycle = (uint32)(duty * ((float)period / ATOM_PWM_MAX));//设置占空比

	Ifx_GTM_ATOM     *atomSFR = &MODULE_GTM.ATOM[pin.atom];

	//设置频率
//	IfxGtm_Atom_Ch_setCompareZeroShadow(atomSFR, pin.channel, period);

	//设置占空比
	IfxGtm_Atom_Ch_setCompareOneShadow(atomSFR, pin.channel, dutyCycle);
}






/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////TOM PWM功能函数    //////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////


//PWM 配置结构体
IfxGtm_Tom_Pwm_Config tomConfig;
IfxGtm_Tom_Pwm_Driver tomDriverPWM[32];
/*************************************************************************
*  函数名称：void TOM_PWM_InitConfig(IfxGtm_Atom_ToutMap pin, uint32_t duty, uint32_t pwmFreq_Hz)
*  功能说明：初始化ATOM为PWM输出功能
*  参数说明：
  * @param    pin            ：    PWM通道  IfxGtm_PinMap.h中 已经定义了对应的PWM管脚
  * @param    duty           ：    占空比 * TOM_PWM_MAX
  * @param    pwmFreq_Hz     ：    频率
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：TOM_PWM_InitConfig(IfxGtm_TOM0_11_TOUT63_P20_7_OUT, 5000, 125);//初始化P20_7 作为PWM输出口 频率100Hz 占空比 百分之(5000/TOM_PWM_MAX)*100
*************************************************************************/
void TOM_PWM_InitConfig(IfxGtm_Tom_ToutMap pin, uint32_t duty, uint32_t pwmFreq_Hz)
{
	static uint8_t count = 0;

	if(count == 0)
	{
		IfxGtm_enable(&MODULE_GTM); /* 使能 GTM */

		IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_FXCLK);       /* Enable the FXU clocks               */

		count ++;
	}


    IfxGtm_Tom_Pwm_initConfig(&tomConfig, &MODULE_GTM);                 /* Initialize default parameters            */

	tomConfig.tom = pin.tom;                                 //选择PWM输出管脚
	tomConfig.tomChannel = pin.channel;                      //选择PWM输出管脚对应的通道
	tomConfig.pin.outputPin = &pin;                           //设置输出管脚
	tomConfig.period = TOM_PWM_CLK / pwmFreq_Hz;              //设置输出周期
	tomConfig.dutyCycle = (uint32)(duty * ((float)tomConfig.period / TOM_PWM_MAX));//设置占空比
    tomConfig.synchronousUpdateEnabled = TRUE;                        //使能PWM同步更新
	tomConfig.clock = IfxGtm_Tom_Ch_ClkSrc_cmuFxclk1;                   /* Select the FXU clock 1              */

    uint8 tempNum = (uint8)pin.tom * 16 + (uint8)pin.channel;
    IfxGtm_Tom_Pwm_init(&tomDriverPWM[tempNum], &tomConfig);         /* Initialize the module                    */
    IfxGtm_Tom_Pwm_start(&tomDriverPWM[tempNum], TRUE);              /* Start the generation of the PWM signal   */

}
/*************************************************************************
*  函数名称：void TOM_PWM_SetDuty(IfxGtm_Tom_ToutMap pin, uint32_t duty, uint32_t pwmFreq_Hz)
*  功能说明：设置PWM频率和占空比
*  参数说明：
   pin            ：PWM通道 IfxGtm_PinMap.h中 已经定义了对应的PWM管脚
   duty           ：占空比 * TOM_PWM_MAX
   pwmFreq_Hz     : PWM频率   注意此函数不可以设置频率  如果要设置频率    需打开设置频率代码注释
*  函数返回：无
*  修改时间：2020年4月1日
*  备    注：TOM_PWM_SetDuty(IfxGtm_TOM0_11_TOUT63_P20_7_OUT, 5000, 1000);//设置P20_7  频率 1000hz 占空比 百分之(5000/TOM_PWM_MAX)*100
*************************************************************************/
void TOM_PWM_SetDuty(IfxGtm_Tom_ToutMap pin, uint32_t duty, uint32_t pwmFreq_Hz)
{
	uint32 period = TOM_PWM_CLK / pwmFreq_Hz;                         //设置输出周期
	uint32 dutyCycle = (uint32)(duty * ((float)period / TOM_PWM_MAX));//设置占空比

	Ifx_GTM_TOM     *atomSFR = &MODULE_GTM.TOM[pin.tom];

	//设置频率
//	IfxGtm_Tom_Ch_setCompareZeroShadow(atomSFR, pin.channel, period);

	//设置占空比
	IfxGtm_Tom_Ch_setCompareOneShadow(atomSFR, pin.channel, dutyCycle);
}





/////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////TIM 脉冲捕捉功能函数    ///////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////

IfxGtm_Tim_In g_driverTIM[24];                          /* TIM driver structure                                     */
IfxGtm_Tim_In_Config configTIM;


/*************************************************************************
*  函数名称：void TIM_InitConfig(void)
*  功能说明：TIM初始化
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月22日
*  备    注：P33.12作为TIM输入口
*************************************************************************/
void TIM_InitConfig(IfxGtm_Tim_TinMap pin)
{
	static uint8_t count = 0;

	if(count == 0)
	{
		IfxGtm_enable(&MODULE_GTM);                                         /* Enable the GTM                           */
		IfxGtm_Cmu_enableClocks(&MODULE_GTM, IFXGTM_CMU_CLKEN_CLK0);        // Enable the CMU clock 0

		count++;
	}


    uint8 tempNum = (uint8)pin.tim * 8 + (uint8)pin.channel;

    IfxGtm_Tim_In_initConfig(&configTIM, &MODULE_GTM);                  /* Initialize default parameters            */
    configTIM.filter.inputPin = &pin;                                   /* Select input port pin                    */
    configTIM.filter.inputPinMode = IfxPort_InputMode_pullDown;         /* Select input port pin mode               */
    IfxGtm_Tim_In_init(&g_driverTIM[tempNum], &configTIM);                       /* Initialize the TIM                       */
}


/*************************************************************************
*  函数名称：void TIM_GetPwm(void)
*  功能说明：获取TIM输入信号的周期和占空比
*  参数说明：pin        TIM管脚
*  参数说明：Period     周期
*  参数说明：Duty       占空比
*  函数返回：无
*  修改时间：2020年3月22日
*  备    注：
*************************************************************************/
void TIM_GetPwm(IfxGtm_Tim_TinMap pin, float32 *PwmFreq_Hz, float32 *Duty)
{
	uint8  g_dataCoherent = FALSE;
	uint8 tempNum = (uint8)pin.tim * 8 + (uint8)pin.channel;
    IfxGtm_Tim_In_update(&g_driverTIM[tempNum]);                                         /* Update the measured data         */
    *PwmFreq_Hz = 1.0f/IfxGtm_Tim_In_getPeriodSecond(&g_driverTIM[tempNum]);             /* Get the period of the PWM signal */
    *Duty = IfxGtm_Tim_In_getDutyPercent(&g_driverTIM[tempNum], &g_dataCoherent);        /* Get the duty cycle     */
}






/////////////////////////////////////////////////////////////////////////////////////

