
#include "../../LIB/STD_TYPES.h"
#include "Sched.h"
#include "Sched_cfg.h"


extern void Switch_Task (void);
extern void sendframe(void);
extern void App_Task(void);
const TasksInfo_t TasksInfo[Tasks_Number]={
		[0]={
				.name = "send bit of dali frame",
				.runnable = sendframe,
				.TaskPeriodms = 417,
				.StartDelayms =0,
		},
		[1]={
				.name = "update switch reading",
				.runnable = Switch_Task,
				.TaskPeriodms = 5004,
				.StartDelayms =0,
		},
		[2]={
						.name = "app",
						.runnable = App_Task,
						.TaskPeriodms = 50000,
						.StartDelayms =0,
		},


};
