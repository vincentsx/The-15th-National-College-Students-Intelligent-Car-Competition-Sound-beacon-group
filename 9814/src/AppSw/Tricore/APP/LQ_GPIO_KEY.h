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
#ifndef __LQ_KEY_H_
#define __LQ_KEY_H_

//定义模块号
typedef enum
{
    KEY0=0,  //母板上按键0
    KEY1=1,
    KEY2=2,
	DSW0,    //母板上拨码开关0
	DSW1
} KEYn_e;

typedef enum
{
    LOW=0,  //按下
    HIGH=1, //松开
    FAIL=0xff,//错误
}KEYs_e;

typedef enum //
{
	NOKEYDOWN=0,
	KEY0DOWN=0x01,  //母板上按键0
	KEY1DOWN=0x02,
	KEY2DOWN=0x04,
	KEY01DOWN=0x03,  //母板上按键0
	KEY02DOWN=0x05,
	KEY12DOWN=0x06,
	KEY012DOWN=0x07,
	KEYError=0xFF,

}KEYdown_e;
//定义的管脚要对应实际按键
#define KEY0p      P22_0  //母板上按键0
#define KEY1p      P22_1  //母板上按键1
#define KEY2p      P22_2  //母板上按键2
#define DSW0p      P33_9  //母板上拨码开关0
#define DSW1p      P33_11 //母板上拨码开关1
/*********************** UART功能函数 **************************/
//初始化
void GPIO_KEY_Init(void);
unsigned char KEY_Read(KEYn_e KEYno);
unsigned char KEY_Read_All(void);
void Test_GPIO_KEY(void);
void Test_ComKEY_Tft(void);

#endif/* 0_APPSW_TRICORE_APP_LQ_ASC_H_ */
