#include "../../LIB/STD_TYPES.h"
#include "../../MCAL/GPIO/GPIO.h"
#include "../../MCAL/RCC/RCC.h"

#include "SWITCH.h"
#include  "SWITCH_Cfg.h"

Switch_strCfg_t Switch_AstrCfg[Switch_Count] = {
/* ex : [Switch_Toggle] = 		 	 */
/* { .port = GPIO_PORT_A ,			 */
/* .pin = GPIO_PIN_0,			 	 */
/* .speed = GPIO_Speed_Low,		 	 */
/* .mode = Switch_Mode_Internal_PU}, */

[Switch_ON] =
{ .port = GPIO_PORT_B, .pin = GPIO_PIN_0, .speed = GPIO_Speed_Low, .mode =
		Switch_Mode_Internal_PU},

[Switch_OFF] =
{ .port = GPIO_PORT_B, .pin = GPIO_PIN_1, .speed = GPIO_Speed_Low, .mode =
		Switch_Mode_Internal_PU }, };



