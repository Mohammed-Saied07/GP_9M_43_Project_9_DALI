
#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/SYSTICK/SYSTICK.h"
#include "Sched.h"
#include "Sched_cfg.h"


typedef struct {
	TasksInfo_t * taskinfo;
	u32 remaining;
}Tasks_t;



static void SchedCBF (void);
static void Sched (void);

u8 flag =0;
Tasks_t tasks[Tasks_Number];

void Sched_Init(void){
	SysTick_EnableInterrupt();
	SysTick_SetPrescaler(STK_enuAHB);
	SysTick_setTickTimeMS(TickTime);
	SysTick_SetCBF(SchedCBF);
	for(int idx=0;idx<Tasks_Number;idx++){
		tasks[idx].taskinfo= &TasksInfo[idx];
		tasks[idx].remaining = TasksInfo[idx].StartDelayms;
	}
}

void Sched_Start(void){
	SysTick_Start();
	while(1){
		if (flag==1)
		{
			Sched();
			flag=0;
		}
	}
}


static void SchedCBF (void){
	flag=1;
}


static void Sched (void){
	for(int idx=0;idx<Tasks_Number;idx++){
		if (tasks[idx].remaining==0)
		{
			tasks[idx].taskinfo->runnable();
			tasks[idx].remaining= tasks[idx].taskinfo->TaskPeriodms;

		}
		tasks[idx].remaining -= TickTime;
	}
}





