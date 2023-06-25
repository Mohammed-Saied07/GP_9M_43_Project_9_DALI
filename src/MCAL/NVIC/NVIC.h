#ifndef MCAL_NVIC_NVIC_H_
#define MCAL_NVIC_NVIC_H_

#define NVIC_MaxPriority	0x0F

typedef enum {
	NVIC_enuOk = 0,
	NVIC_enuNotOk,
	NVIC_enuNullPtr,
}NVIC_enuErrorStatus_t;

typedef enum {
	NVIC_enuCode = 0x30,
	NVIC_enuSRAM = 0x31,
}NVIC_enuMemory_t;


NVIC_enuErrorStatus_t NVIC_EnableIRQ(u8 IRQn);

NVIC_enuErrorStatus_t NVIC_DisableIRQ(u8 IRQn);

NVIC_enuErrorStatus_t NVIC_SetPendingIRQ(u8 IRQn);

NVIC_enuErrorStatus_t NVIC_ClearPendingIRQ(u8 IRQn);

NVIC_enuErrorStatus_t NVIC_GetPendingIRQ(u8 IRQn, u32* Add_PendingVal);

NVIC_enuErrorStatus_t NVIC_GetActiveStatusIRQ(u8 IRQn, u32* Add_PendingVal);

NVIC_enuErrorStatus_t NVIC_SetPriority(u8 IRQn, u32 priority);

NVIC_enuErrorStatus_t NVIC_GetPriority(u8 IRQn,u32* Add_PriorityVal);

NVIC_enuErrorStatus_t NVIC_GenerateSWinterrupt(u8 IRQn);

/*********************************************************************/
/* this function reallocate the vector table					   	 */
/* input : the memory (NVIC_enuCode or NVIC_enuSRAM) 				 */
/* 			the offset and should be aligned to 128 word	 		 */
/* output : the error status (ok or not ok in case of invalid inputs)*/
/*********************************************************************/
NVIC_enuErrorStatus_t NVIC_SetVectorTableLocation(NVIC_enuMemory_t memory, u32 offset);

/*********************************************************************/
/* this function set the number of bits implemented for grouping	 */
/* input : the number of bits for group from 0 to 7 				 */
/* note : the number of non implemented bits will be included		 */
/* 		  in grouping bits ex: if grouping bits = 4 (from 0 to 4) 	 */
/* 		  and first 4 bits are not implemented (from 0 to 3)	 	 */
/* 		  there will be one bit for grouping and 3 bits preemtion  	 */
/* output : the error status (ok or not ok in case of invalid inputs)*/
/*********************************************************************/
NVIC_enuErrorStatus_t NVIC_SetPriorityGrouping(u8 priority_grouping);

void NVIC_DisableAllInterrupts(void);

void NVIC_EnableAllInterrupts(void);

void NVIC_DisableHardFault(void);

void NVIC_EnableHardFault(void);

void NVIC_SetBASEPRI(u8 priority);

#endif /* MCAL_NVIC_NVIC_H_ */
