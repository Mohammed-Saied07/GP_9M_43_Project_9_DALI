
#ifndef HAL_DALI_DALI_H_
#define HAL_DALI_DALI_H_

#include "../../LIB/STD_TYPES.h"
// Define the DALI bus timing parameters
#define DALI_BIT_TIME_US 416

// Define the maximum number of lamps the driver can support
#define MAX_Lamps 64

// Define the lamps addresses
#define DALI_Lamp_1 	0x01
#define DALI_Lamp_2 	0x02
#define DALI_Lamp_3 	0x03
#define DALI_Lamp_4 	0x04
#define DALI_Group_1 	0x41 // 7th bit is the Y bit ==> group 1 contain Lamp 1 &2
#define DALI_Group_2 	0x42 // group 2 contain Lamp 2 &3
#define DALI_Group_3 	0x43 // group 3 contain Lamp 3 &4
#define DALI_Broadcast 	0x7F

// Define the DALI commands
#define DALI_CMD_OFF 0x00
#define DALI_CMD_UP 0x01
#define DALI_CMD_DOWN 0x02

#define DALI_Minimum_Intensity	5
#define DALI_Maximum_Intensity	255
typedef enum
{
	DALI_enuOk=0,
	DALI_enuNotOk,
	DALI_enuNullPtr,
	DALI_enuBusy,
}DALI_enuErrorStatus_t;

// Initialize the DALI driver
DALI_enuErrorStatus_t dali_init(void);

u8 dali_getStatus(void);

DALI_enuErrorStatus_t dali_recieve(void);

// Send a DALI command to a specific lamp
DALI_enuErrorStatus_t dali_send_command(u8 lamp_address, u16 command);

// Set the level "intensity" of a specific lamp
DALI_enuErrorStatus_t dali_set_level(u8 lamp_address, u16 level);


#endif /* HAL_DALI_DALI_H_ */
