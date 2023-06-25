
#include "../../LIB/STD_TYPES.h"
#include "SYSTICK.h"


#define SYSCLK              (25000000)

#define SysTick_BaseAddress 	0xE000E010
#define STK_CLKSrc_Pin			2
#define STK_TICKINT_Pin			1
#define STK_Enable_Pin			0


typedef struct {
	u32 STK_CTRL;
	u32 STK_LOAD;
	u32 STK_VAL;
	u32 STK_CALIB;
} SysTick_strReg_t;

volatile SysTick_strReg_t * const SysTick=
		((volatile SysTick_strReg_t * const )  SysTick_BaseAddress);

static SysTickCbf_t systickCbf;

void SysTick_EnableInterrupt(void){
	SysTick->STK_CTRL |= (1<<STK_TICKINT_Pin);

}

void SysTick_DisableInterrupt(void){
	SysTick->STK_CTRL &= (~(1<<STK_TICKINT_Pin));

}

void SysTick_Start(void){
	SysTick->STK_VAL=0;
	SysTick->STK_CTRL |= (1<<STK_Enable_Pin);
}

void SysTick_Stop(void){
	SysTick->STK_CTRL &= (~(1<<STK_Enable_Pin));

}

STK_enuErrorStatus_t SysTick_SetPrescaler(STK_enuPrescaler_t Copy_SysTickSource){
	STK_enuErrorStatus_t Loc_ErrorStatus = STK_enuOk;
	if (Copy_SysTickSource != STK_enuAHB || Copy_SysTickSource != STK_enuAHB_div8){
		Loc_ErrorStatus = STK_enuNotOk;
	}
	else {
		/*clear the old configuration */
		SysTick->STK_CTRL &= (~(1<<STK_CLKSrc_Pin));
		/*set the configuration*/
		SysTick->STK_CTRL |= (Copy_SysTickSource<<STK_CLKSrc_Pin);
	}

	return Loc_ErrorStatus;
}


void SysTick_setTickTimeMS(u32 TickTime)
{
	u8 prescaler =1;
	if (!(SysTick->STK_CTRL>> STK_CLKSrc_Pin)){
		prescaler =8;
	}
	//SysTick->STK_LOAD = (u32)(SYSCLK/(prescaler*1000)) * TickTime ;
	SysTick->STK_LOAD = (u32) (SYSCLK/(prescaler*1000000)) * TickTime;
}


STK_enuErrorStatus_t SysTick_SetCBF(SysTickCbf_t CBF)
{
	STK_enuErrorStatus_t Loc_ErrorStatus = STK_enuOk;
	if (CBF) {
		systickCbf = CBF;
	} else {
		Loc_ErrorStatus = STK_enuNullPtr;
	}
	return Loc_ErrorStatus;
}

void SysTick_Handler(void)
{
	if (systickCbf)
		systickCbf();
}
