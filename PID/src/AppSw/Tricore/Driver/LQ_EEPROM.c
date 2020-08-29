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
eeprom 一共96Kb 分为12个扇区   每个扇区8K  每个扇区分为1024页   每页8个字节
QQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQQ*/

#include <LQ_EEPROM.h>


/*************************************************************************
*  函数名称：void EEPROM_EraseSector(unsigned char sector)
*  功能说明：eeprom擦出扇区
*  参数说明：sector   ：  扇区范围  0-11
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：EEPROM_EraseSector(0);   //擦出扇区0
*************************************************************************/
void EEPROM_EraseSector(unsigned char sector)
{
	/* 计算扇区起始地址 */
	unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;

	unsigned short endinitSfty_pw;

	endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPassword();

	/* 擦出扇区 */
	IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
	IfxFlash_eraseSector(sector_addr);
	IfxScuWdt_setSafetyEndinit(endinitSfty_pw);

	IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
}

/*************************************************************************
*  函数名称：void EEPROM_EraseSector(unsigned char sector)
*  功能说明：eeprom写入数据
*  参数说明：
  * @param    sector   ：  扇区   范围  0-11
  * @param    page     ：  页     范围  0-1023
  * @param    buff     ：  存放写入数据
  * @param    len      ：  写入数据个数
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：EEPROM_Write(0, 0, u32wBuff, 24);   //向扇区0  第0页  写入24个unsigned long数据
*************************************************************************/
void EEPROM_Write(unsigned char sector, unsigned short page, unsigned long * buff, unsigned short len)
{
	int i;
	/* 计算扇区起始地址 */
	unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;

	unsigned short endinitSfty_pw;

	endinitSfty_pw = IfxScuWdt_getSafetyWatchdogPassword();

	for(i = 0; i < len/2; i++)
	{
		/* 要写入页地址 */
		unsigned long pageAddr = sector_addr + (page + i)* IFXFLASH_DFLASH_PAGE_LENGTH;

		IfxFlash_enterPageMode(pageAddr);

		/* 察忙 */
		IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

		/* 写入缓冲区  */
		IfxFlash_loadPage2X32(pageAddr, buff[2*i], buff[2*i + 1]);

		/* 写入eeprom  */
		IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
		IfxFlash_writePage(pageAddr);
		IfxScuWdt_setSafetyEndinit(endinitSfty_pw);

		IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);
	}

	if(len%2)
	{
		/* 要写入页地址 */
		unsigned long pageAddr = sector_addr + (page + len/2)* IFXFLASH_DFLASH_PAGE_LENGTH;

		IfxFlash_enterPageMode(pageAddr);

		/* 察忙 */
		IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

		/* 写入缓冲区  */
		IfxFlash_loadPage2X32(pageAddr, buff[len], 0);

		/* 写入eeprom  */
		IfxScuWdt_clearSafetyEndinit(endinitSfty_pw);
		IfxFlash_writePage(pageAddr);
		IfxScuWdt_setSafetyEndinit(endinitSfty_pw);

		IfxFlash_waitUnbusy(0, IfxFlash_FlashType_D0);

	}
}


/*************************************************************************
*  函数名称：void EEPROM_Read(unsigned char sector, unsigned short page, unsigned long * rbuff, unsigned short len)
*  功能说明：eeprom读取
*  参数说明：
  * @param    sector   ：  扇区   范围  0-11
  * @param    page     ：  页     范围  0-1023
  * @param    buff     ：  存放写入数据
  * @param    len      ：  写入数据个数
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：EEPROM_Read(0, 0, u32rBuff, 24);   //读取扇区0  第0页  开始的24个unsigned long数据
*************************************************************************/
void EEPROM_Read(unsigned char sector, unsigned short page, unsigned long * rbuff, unsigned short len)
{
	int i;
	/* 计算扇区起始地址 */
	unsigned long sector_addr = IfxFlash_dFlashTableEepLog[sector].start;

	/* 要读页地址 */
	volatile unsigned long * pageAddr = (unsigned long*)(sector_addr + page * IFXFLASH_DFLASH_PAGE_LENGTH);

	for(i = 0; i < len; i++)
	{
		rbuff[i] = *(pageAddr++);
	}
}





