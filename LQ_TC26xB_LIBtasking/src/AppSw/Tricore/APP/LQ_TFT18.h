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
#ifndef __LQ_SGP18T_TFTSPI_H__
#define __LQ_SGP18T_TFTSPI_H__	

#include "stdint.h"

/*******************接口定义******************************/

#define APP_LCD_WIDTH  LCD_WIDTH//液晶屏宽度
#define APP_LCD_HEIGHT LCD_HEIGHT//液晶屏高度

#define TFT18W        162
#define TFT18H        132

#define u16RED		0xf800
#define u16GREEN	0x07e0
#define u16BLUE	    0x001f
#define u16PURPLE	0xf81f		//紫色
#define u16YELLOW	0xffe0
#define u16CYAN	    0x07ff 		//蓝绿色
#define u16ORANGE	0xfc08
#define u16BLACK	0x0000
#define u16WHITE	0xffff

/*!
  * @brief    TFT18初始化
  *
  * @param    type ： 0:横屏  1：竖屏
  *
  * @return   无
  *
  * @note     如果修改管脚 需要修改初始化的管脚
  *
  * @see      TFTSPI_Init(1);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Init(uint8_t type);

/*!
  * @brief    写命令
  *
  * @param    cmd ：命令
  *
  * @return   无
  *
  * @note     内部调用
  *
  * @see      TFTSPI_Write_Cmd(0xb7); //LCD Driveing control
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Write_Cmd(uint8_t cmd);

/*!
  * @brief    写字节
  *
  * @param    dat ：数据
  *
  * @return   无
  *
  * @note     内部调用
  *
  * @see      TFTSPI_Write_Byte(0x00);	//CRL=0
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Write_Byte(uint8_t dat);


/*!
  * @brief    写半字
  *
  * @param    dat ：数据
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_Write_Word(0xFFFF);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Write_Word(uint16_t dat);


/*!
  * @brief    重新定位输入信息位置
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  *
  * @return   无
  *
  * @note     内部调用
  *
  * @see      TFTSPI_Set_Pos(10, 20, 30, 40);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Set_Pos(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye);

/*!
  * @brief    填充指定区域
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    color ：填充的颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_Fill_Area(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Fill_Area(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color);

/*!
  * @brief    全屏显示单色画面
  *
  * @param    color ：填充的颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_CLS(u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_CLS(uint16_t color);


/*!
  * @brief    填充矩形区域
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    color_dat ：填充的颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_Draw_Part(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Draw_Part(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color_dat);

/*!
  * @brief    画矩形边框
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    color_dat ：颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_Draw_Rectangle(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Draw_Rectangle(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color_dat);

/*!
  * @brief    画圆
  *
  * @param    x ：圆心x   (0-127)
  * @param    y ：圆心y   (0-159)
  * @param    r ：半径    (0-128)
  * @param    color_dat ：颜色
  *
  * @return   无
  *
  * @note     圆心坐标不要超出屏幕范围
  *
  * @see      TFTSPI_Draw_Circle(50, 50, 30, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Draw_Circle(uint8_t x,uint8_t y,uint8_t r,uint16_t color_dat);

/*!
  * @brief    画线
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    color_dat ：颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_Draw_Line(10, 20, 30, 40, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Draw_Line(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,uint16_t color_dat);

/*!
  * @brief    画点
  *
  * @param    x ：x
  * @param    y ：y
  * @param    color_dat ：颜色
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see      TFTSPI_Draw_Dot(10, 20, u16YELLOW);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Draw_Dot(uint8_t x,uint8_t y,uint16_t color_dat);

/*!
  * @brief    显示图片
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    ppic ：图片数据
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Show_Pic(uint8_t xs,uint8_t ys,uint8_t xe,uint8_t ye,const char *ppic);

/*!
  * @brief    液晶字符串输出(6*8字体)
  *
  * @param    x: 0 - 20 (行)
  * @param    y: 0 - 19 (列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_P6X8Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P6X8Str(uint8_t x, uint8_t y, char *s_dat,uint16_t word_color,uint16_t back_color);

/*!
  * @brief    液晶字符串输出(8*8字体)
  *
  * @param    x:0 - 15 (行)
  * @param    y:0 - 19 (列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_P8X8Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P8X8Str(uint8_t x, uint8_t y, char *s_dat,uint16_t word_color,uint16_t back_color);

/*!
  * @brief    液晶字符串输出(8*16字体)
  *
  * @param    x: x: 0 -15   (行)
  * @param    y: y: 0 -9  	 (列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_P8X16Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P8X16Str(uint8_t x, uint8_t y, char *s_dat,uint16_t word_color,uint16_t back_color);

/*!
  * @brief    液晶汉字字符串输出(8*16字体)
  *
  * @param    x: 0 - 7	(行)
  * @param    y: 0 -12	(列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     汉字只能是字库里的 字库没有的需要自行添加
  *
  * @see      TFTSPI_P16x12Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P16x12Str(uint8_t x, uint8_t y, char *s_dat,uint16_t word_color,uint16_t back_color);

/*!
  * @brief    液晶汉字字符串输出(16*16字体)
  *
  * @param    x: 0 - 7	(行)
  * @param    y: 0 - 9	(列)
  * @param    word_color: 字体颜色
  * @param    back_color: 背景颜色
  *
  * @return   无
  *
  * @note     汉字只能是字库里的 字库没有的需要自行添加
  *
  * @see      TFTSPI_P16x16Str(1, 1, "123456", u16YELLOW, u16RED);
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_P16x16Str(uint8_t x, uint8_t y, unsigned char *s_dat,uint16_t word_color,uint16_t back_color);


/*!
  * @brief    显示图片
  *
  * @param    xs ：起始x
  * @param    ys ：起始y
  * @param    xe ：结束x
  * @param    ys ：结束y
  * @param    ppic ：图片数据
  *
  * @return   无
  *
  * @note     起始、终止横坐标(0-127)，纵坐标(0-159),显示颜色uint16
  *
  * @see
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Show_Pic2(uint8_t xs,uint8_t ys,uint8_t w,uint8_t h,const char *ppic) ;


/*!
  * @brief    显示电池图标
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_Show_Battery_Icon();
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Show_Battery_Icon(void);

/*!
  * @brief    显示标签
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_Show_Title();
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Show_Title(void);

/*!
  * @brief    显示龙邱LOG
  *
  * @param    无
  *
  * @return   无
  *
  * @note     无
  *
  * @see      TFTSPI_Show_Logo();
  *
  * @date     2019/6/13 星期四
  */
void TFTSPI_Show_Logo(uint8_t xs,uint8_t ys);

/*!
  * @brief    TFT18屏 uint8_t 灰度数据显示
  *
  * @param    high_start ： 显示图像开始位置
  * @param    wide_start ： 显示图像开始位置
  * @param    high ： 显示图像高度
  * @param    wide ： 显示图像宽度
  * @param    Pixle： 显示图像数据地址
  *
  * @return   无
  *
  * @note     注意 屏幕左上为 （0，0）
  *
  * @see
  *
  * @date     2019/12/3 星期二
  */
void TFTSPI_Road(uint8_t high_start, uint8_t wide_start, uint8_t high, uint8_t wide, uint8_t *Pixle);

/*!
  * @brief    TFT18屏 uint8_t 二值化数据显示
  *
  * @param    high_start ： 显示图像开始位置
  * @param    wide_start ： 显示图像开始位置
  * @param    high ： 显示图像高度
  * @param    wide ： 显示图像宽度
  * @param    Pixle： 显示图像数据地址
  *
  * @return   无
  *
  * @note     注意 屏幕左上为 （0，0）
  *
  * @see
  *
  * @date     2019/12/3 星期二
  */
void TFTSPI_BinRoad(uint8_t high_start, uint8_t wide_start, uint8_t high, uint8_t wide, uint8_t *Pixle);


void Test_TFT18(void);


#endif /*SGP18T_ILI9163B.h*/
