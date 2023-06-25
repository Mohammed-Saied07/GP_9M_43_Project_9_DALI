#include "../../LIB/STD_TYPES.h"
#include "NVIC.h"

#define NVIC_BaseAddress 	0xE000E100
#define ClearPriorityMask   0xFF
#define NVIC_MaxPriority	0x0F

#define SCB_BaseAddress 	0xE000ED00
#define SCB_VTOR			(*(volatile u32* const )(SCB_BaseAddress+0x08))
#define SCB_AIRCR 			(*(volatile u32* const) (SCB_BaseAddress+0x0C))
#define PASSWORD_MASK 	 	0X05FA0000

typedef struct {
	u32 NVIC_ISER[8];
	u32 NVIC_Reserved1[24];
	u32 NVIC_ICER[8];
	u32 NVIC_Reserved2[24];
	u32 NVIC_ISPR[8];
	u32 NVIC_Reserved3[24];
	u32 NVIC_ICPR[8];
	u32 NVIC_Reserved4[24];
	u32 NVIC_IABR[8];
	u32 NVIC_Reserved5[56];
	u32 NVIC_IPR[60];
	u32 NVIC_Reserved6[580];
	u32 NVIC_STIR;
} NVIC_strReg_t;

volatile NVIC_strReg_t * const NVIC =
		((volatile NVIC_strReg_t * const ) 0xE000E100);

NVIC_enuErrorStatus_t NVIC_EnableIRQ(u8 IRQn) {
	NVIC_enuErrorStatus_t Loc_enuErrorStatus = NVIC_enuOk;
	u8 Reg_indx = IRQn / 32;
	u8 Pin_no = IRQn % 32;
	if (IRQn < 240) {
		NVIC->NVIC_ISER[Reg_indx] = (1 << Pin_no);
	} else {
		Loc_enuErrorStatus = NVIC_enuNotOk;
	}
	return Loc_enuErrorStatus;
}

NVIC_enuErrorStatus_t NVIC_DisableIRQ(u8 IRQn) {
	NVIC_enuErrorStatus_t Loc_enuErrorStatus = NVIC_enuOk;
	u8 Reg_indx = IRQn / 32;
	u8 Pin_no = IRQn % 32;
	if (IRQn < 240) {
		NVIC->NVIC_ICER[Reg_indx] = (1 << Pin_no);
	} else {
		Loc_enuErrorStatus = NVIC_enuNotOk;
	}
	return Loc_enuErrorStatus;
}

NVIC_enuErrorStatus_t NVIC_SetPendingIRQ(u8 IRQn) {
	NVIC_enuErrorStatus_t Loc_enuErrorStatus = NVIC_enuOk;
	u8 Reg_indx = IRQn / 32;
	u8 Pin_no = IRQn % 32;
	if (IRQn < 240) {
		NVIC->NVIC_ISPR[Reg_indx] = (1 << Pin_no);
	} else {
		Loc_enuErrorStatus = NVIC_enuNotOk;
	}
	return Loc_enuErrorStatus;
}

NVIC_enuErrorStatus_t NVIC_ClearPendingIRQ(u8 IRQn) {
	NVIC_enuErrorStatus_t Loc_enuErrorStatus = NVIC_enuOk;
	u8 Reg_indx = IRQn / 32;
	u8 Pin_no = IRQn % 32;
	if (IRQn < 240) {
		NVIC->NVIC_ICPR[Reg_indx] = (1 << Pin_no);
	} else {
		Loc_enuErrorStatus = NVIC_enuNotOk;
	}
	return Loc_enuErrorStatus;
}

NVIC_enuErrorStatus_t NVIC_GetPendingIRQ(u8 IRQn, u32* Add_PendingVal) {
	NVIC_enuErrorStatus_t Loc_enuErrorStatus = NVIC_enuOk;
	u8 Reg_indx = IRQn / 32;
	u8 Pin_no = IRQn % 32;
	if (IRQn < 240) {
		* Add_PendingVal= (NVIC->NVIC_ICPR[Reg_indx] >> Pin_no)&1;
	} else {
		Loc_enuErrorStatus = NVIC_enuNotOk;
	}
	return Loc_enuErrorStatus;
}

NVIC_enuErrorStatus_t NVIC_GetActiveStatusIRQ(u8 IRQn, u32* Add_PendingVal) {
	NVIC_enuErrorStatus_t Loc_enuErrorStatus = NVIC_enuOk;
	u8 Reg_indx = IRQn / 32;
	u8 Pin_no = IRQn % 32;
	if (IRQn < 240) {
		*Add_PendingVal = (NVIC->NVIC_IABR[Reg_indx] >> Pin_no) & 1;
	} else {
		Loc_enuErrorStatus = NVIC_enuNotOk;
	}
	return Loc_enuErrorStatus;
}

NVIC_enuErrorStatus_t NVIC_SetPriority(u8 IRQn, u32 priority) {
	NVIC_enuErrorStatus_t Loc_enuErrorStatus = NVIC_enuOk;
	u8 Reg_indx = ((u32) (IRQn * 8) / 32);
	u8 Pin_no = (IRQn % 8) * 8;
	if (IRQn < 240 && priority <= NVIC_MaxPriority) {
		NVIC->NVIC_IPR[Reg_indx] &= ~(ClearPriorityMask << Pin_no);
		NVIC->NVIC_IPR[Reg_indx] |= (priority << Pin_no);
	} else {
		Loc_enuErrorStatus = NVIC_enuNotOk;
	}
	return Loc_enuErrorStatus;
}

NVIC_enuErrorStatus_t NVIC_GetPriority(u8 IRQn, u32* Add_PriorityVal) {
	NVIC_enuErrorStatus_t Loc_enuErrorStatus = NVIC_enuOk;
	u8 Reg_indx = ((u32) (IRQn * 8) / 32);
	u8 Pin_no = (IRQn % 8) * 8;
	if (IRQn < 240) {
		* Add_PriorityVal =( NVIC->NVIC_IPR[Reg_indx] >> Pin_no )& 0xFF;
	} else {
		Loc_enuErrorStatus = NVIC_enuNotOk;
	}
	return Loc_enuErrorStatus;
}

NVIC_enuErrorStatus_t NVIC_GenerateSWinterrupt(u8 IRQn) {
	NVIC_enuErrorStatus_t Loc_enuErrorStatus = NVIC_enuOk;
	if (IRQn < 240) {
		NVIC->NVIC_STIR = IRQn;
	} else {
		Loc_enuErrorStatus = NVIC_enuNotOk;
	}
	return Loc_enuErrorStatus;
}

NVIC_enuErrorStatus_t NVIC_SetVectorTableLocation(NVIC_enuMemory_t memory, u32 offset){
	NVIC_enuErrorStatus_t Loc_enuErrorStatus = NVIC_enuOk;
	if (memory != NVIC_enuCode || memory != NVIC_enuSRAM || (offset&(0x1FFFFE00)!=offset)){
		Loc_enuErrorStatus = NVIC_enuNotOk;
	}
	else {
		memory &=0x0F;
		SCB_VTOR = (memory<<29) | offset;
	}
	return Loc_enuErrorStatus;
}

NVIC_enuErrorStatus_t NVIC_SetPriorityGrouping(u8 priority_grouping)
{    NVIC_enuErrorStatus_t Loc_enuErrorStatus = NVIC_enuOk;
	if(priority_grouping > 7 )
	{
		Loc_enuErrorStatus = NVIC_enuNotOk;
	}
	else
	{
		SCB_AIRCR=(priority_grouping<<8)|PASSWORD_MASK;
	}
	return Loc_enuErrorStatus;

}


void NVIC_DisableAllInterrupts(void)
{
	asm("MOV R0, #1");
	asm("MSR PRIMASK, R0");
}


void NVIC_EnableAllInterrupts(void)
{
	asm("MOV R0, #0");
	asm("MSR PRIMASK, R0");
}

void NVIC_DisableHardFault(void)
{
	asm("MOV R0, #1");
	asm("MSR FAULTMASK, R0");
}


void NVIC_EnableHardFault(void)
{
	asm("MOV R0, #0");
	asm("MSR FAULTMASK, R0");
}


void NVIC_SetBASEPRI(u8 priority)
{
	/* the priority is the first argument so it's moved to r0*/
	//asm("LSLS R0,R0,#4");
	asm("MSR  BASEPRI, R0");
}
