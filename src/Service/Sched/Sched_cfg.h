
#ifndef SERVICE_SCHED_SCHED_CFG_H_
#define SERVICE_SCHED_SCHED_CFG_H_

#define Tasks_Number		3
#define TickTime	417

typedef struct {
	char* name;
	RunnableCBF_t runnable;
	u32 TaskPeriodms;
	u32 StartDelayms;

}TasksInfo_t;

extern const TasksInfo_t TasksInfo[Tasks_Number];


#endif /* SERVICE_SCHED_SCHED_CFG_H_ */
