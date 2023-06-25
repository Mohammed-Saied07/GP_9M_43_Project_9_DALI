#ifndef MCAL_SYSTICK_SYSTICK_H_
#define MCAL_SYSTICK_SYSTICK_H_

typedef enum {
	STK_enuAHB_div8 =0,
	STK_enuAHB,
}STK_enuPrescaler_t;

typedef enum {
	STK_enuOk = 0,
	STK_enuNotOk,
	STK_enuNullPtr,
}STK_enuErrorStatus_t;


typedef void(*SysTickCbf_t)(void);

void SysTick_EnableInterrupt(void);

void SysTick_DisableInterrupt(void);

void SysTick_Start(void);

void SysTick_Stop(void);

STK_enuErrorStatus_t SysTick_SetPrescaler(STK_enuPrescaler_t Copy_SysTickSource);

void SysTick_setTickTimeMS(u32 TickTime);

STK_enuErrorStatus_t SysTick_SetCBF(SysTickCbf_t CBF);

#endif /* MCAL_SYSTICK_SYSTICK_H_ */
