#ifndef HAL_SWITCH_SWITCH_H_
#define HAL_SWITCH_SWITCH_H_

#include "SWITCH_Cfg.h"

#define Switch_Mode_Internal_PU			1
#define Switch_Mode_Internal_PD			0
#define Switch_Mode_External_PU			0x11
#define Switch_Mode_External_PD			0x10

#define Switch_State_Pressed			1
#define Switch_State_NotPressed			0


typedef struct sw {

	void* port;
	u32 pin;
	u16 mode; /* Internal or external Pull up or Pull down */
	u8 speed;
}Switch_strCfg_t;

typedef enum {
	Switch_enuOk = 0,
	Switch_enuNotOk,
	Switch_enuNullPtr,
	Switch_enuInvalidMode,
	Switch_enuInvalidSwitch,
} Switch_enuErrorStatus_t;

extern Switch_strCfg_t Switch_AstrCfg [Switch_Count];

/***************************************************************************************/
/*  this function initialize all switches configuration 					     	   */
/*	input : void												                       */
/*	output :Error status (Switch_enuOk  ,Switch_enuNotOk , Switch_enuInvalidMode)	   */
/***************************************************************************************/
Switch_enuErrorStatus_t Switch_init(void);


/***************************************************************************************/
/*  this function get the switch state pressed or not pressed		 	     		   */
/*	input : the switch  and pointer to variable that will store the switch state       */
/*	output :Error status (Switch_enuOk  ,Switch_enuNotOk , Switch_enuInvalidSwitch)	   */
/***************************************************************************************/
Switch_enuErrorStatus_t Switch_getValue(u8 Copy_u8Switch , u8* Add_u8State);


#endif /* HAL_SWITCH_SWITCH_H_ */
