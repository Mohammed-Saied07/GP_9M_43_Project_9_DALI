#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/GPIO/GPIO.h"
#include "../../MCAL/RCC/RCC.h"

#include "SWITCH.h"
#include "SWITCH_Cfg.h"



Switch_enuErrorStatus_t Switch_init(void) {

	Switch_enuErrorStatus_t Loc_enuErrorStatue = Switch_enuOk;
	u8 Loc_u8SwitchCount = sizeof(Switch_AstrCfg) / sizeof(Switch_AstrCfg[0]);
	u8 Loc_u8Index = 0;
	gpio_Cfg_t swc;
	for (Loc_u8Index = 0;
			Loc_u8Index < Loc_u8SwitchCount
					&& Loc_enuErrorStatue == Switch_enuOk; Loc_u8Index++) {
		if (Switch_AstrCfg[Loc_u8Index].mode == Switch_Mode_External_PD
				|| Switch_AstrCfg[Loc_u8Index].mode == Switch_Mode_External_PU
				|| Switch_AstrCfg[Loc_u8Index].mode == Switch_Mode_Internal_PD
				|| Switch_AstrCfg[Loc_u8Index].mode == Switch_Mode_Internal_PU) {
			if (Switch_AstrCfg[Loc_u8Index].mode == Switch_Mode_External_PD
					|| Switch_AstrCfg[Loc_u8Index].mode
							== Switch_Mode_External_PU) {
				swc.mode = GPIO_Mode_Input_Floating;
			} else if (Switch_AstrCfg[Loc_u8Index].mode
					== Switch_Mode_Internal_PD) {
				swc.mode = GPIO_Mode_Input_PD;
			} else if (Switch_AstrCfg[Loc_u8Index].mode
					== Switch_Mode_Internal_PU) {
				swc.mode = GPIO_Mode_Input_PU;
			} else {

			}
			swc.pin = Switch_AstrCfg[Loc_u8Index].pin;
			swc.port = Switch_AstrCfg[Loc_u8Index].port;
			swc.speed = Switch_AstrCfg[Loc_u8Index].speed;
			Loc_enuErrorStatue = gpio_initPin(&swc);
			if (Loc_enuErrorStatue != GPIO_enuOk) {
				Loc_enuErrorStatue = Switch_enuNotOk;

			}
		} else {
			Loc_enuErrorStatue = Switch_enuNotOk;
		}
	}

	return Loc_enuErrorStatue;
}


/*Switch_enuErrorStatus_t Switch_getValue(u8 Copy_u8Switch , u8* Add_u8State){

	Switch_enuErrorStatus_t Loc_enuErrorStatue = Switch_enuOk;
		if (Copy_u8Switch >= Switch_Count) {
			Loc_enuErrorStatue = Switch_enuInvalidSwitch;
		} else {
			u8 Loc_u8Temp;
			Loc_enuErrorStatue = gpio_getPinValue(Switch_AstrCfg[Copy_u8Switch].port,
					Switch_AstrCfg[Copy_u8Switch].pin,
					&Loc_u8Temp);
			if (Loc_enuErrorStatue != GPIO_enuOk)
			{
				Loc_enuErrorStatue = Switch_enuNotOk;

			}
			else {
				u8 mode = Switch_AstrCfg[Copy_u8Switch].mode & 1;
				Loc_u8Temp ^= mode ;
				* Add_u8State = Loc_u8Temp;
			}
		}
		return Loc_enuErrorStatue;
}
*/



static u8 switch_state[Switch_Count];

Switch_enuErrorStatus_t Switch_getValue (u8 cpy_enuSWCNumber, u8* Add_pu8SwitchStatus){
	 Switch_enuErrorStatus_t Loc_enuErrorState = Switch_enuOk ;
	if ( cpy_enuSWCNumber >= Switch_Count)
	{
		Loc_enuErrorState = Switch_enuNotOk ;
	}
	else {
		* Add_pu8SwitchStatus = switch_state[cpy_enuSWCNumber] ^ Switch_AstrCfg [cpy_enuSWCNumber].mode ;
	}
	return Loc_enuErrorState;

}


/* will be called each 5 msec */
void Switch_Task (void){
	static u8 counter[Switch_Count],previous[Switch_Count];
	u8 loc_u8current;
	for (u8 idx=0;idx<Switch_Count ;idx++){
		gpio_getPinValue(Switch_AstrCfg[idx].port,Switch_AstrCfg[idx].pin ,&loc_u8current);
		 if (loc_u8current == previous[idx]){
			 counter[idx]++;
		 }
		 else
		 {
			 counter[idx]=0;
		 }
		 if (counter[idx]==5){
			 switch_state[idx]=loc_u8current;
			 counter[idx]=0;
		 }
		 previous[idx]=loc_u8current;
	}
}
