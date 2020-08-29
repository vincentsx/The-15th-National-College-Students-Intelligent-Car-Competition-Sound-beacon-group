/*!
  * @file     LQ_EMEM.c
  *
  * @brief    TC264 DA后缀的芯片才有EMEM的512K ram
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
  * @date     2020年4月18日
  */ 



#include <IfxEmem.h>
#include "LQ_EMEM.h"

/*************************************************************************
*  函数名称：EMEM_InitConfig(void)
*  功能说明：初始化芯片内部 EMEM内存  512K
*  参数说明：无
*  函数返回：无
*  修改时间：2020年3月10日
*  备    注：TC264 DA后缀的芯片才有EMEM的512K ram
*************************************************************************/
void EMEM_InitConfig(void)
{
	IfxEmem_setClockEnableState(IfxEmem_State_enabled);

	if (IfxEmem_LockedState_locked == IfxEmem_getLockedState())
	{
		/* apply unlock sequence */
		IfxEmem_setUnlockStandbyLockFlag(0x1);
		IfxEmem_setUnlockStandbyLockFlag(0x3);
		IfxEmem_setUnlockStandbyLockFlag(0x7);
		/* wait one cycle for unlock */

		for(int i = 0; i < 10000; i++)
		{
			__asm("NOP");
		}
	}

	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_0);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_1);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_2);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_3);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_4);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_5);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_6);
	IfxEmem_setTileConfigMode(IfxEmem_TileConfigMode_calibMode,IfxEmem_TileNumber_7);
}
