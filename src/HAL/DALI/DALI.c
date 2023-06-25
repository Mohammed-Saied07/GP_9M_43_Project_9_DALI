#include "../../LIB/STD_TYPES.h"
#include "DALI.h"
#include "DALI_cfg.h"
#include "../../MCAL/SYSTICK/SYSTICK.h"
#include "../../MCAL/GPIO/GPIO.h"
#include "../../MCAL/RCC/RCC.h"


typedef unsigned long uint32;
typedef unsigned short uint16;
typedef unsigned char uint8;
typedef unsigned long long uint64;


//#define REVERSE_bITS 	5
typedef enum
{
	DALI_Idle = 0,
	DALI_Busy
}DALI_status_t;

typedef union
	{
        uint64 FullFrame;
#ifdef REVERSE_bITS
        struct D{
		    uint64 StopBits : 6;
		    uint64 DataBits : 32;
		    uint64 SBit		: 2;
		    uint64 AddBits  : 12;
		    uint64 YBit		: 2;
		    uint64 startBit : 2;
        }B;
#else
        struct D{
		    uint64 startBit : 2;
		    uint64 YBit		: 2;
		    uint64 AddBits  : 12;
		    uint64 SBit		: 2;
		    uint64 DataBits : 32;
		    uint64 StopBits : 6;
        }B;
#endif

}DALI_Frame;


static uint32 manchester_Encoder(uint16 sendBitFrame, uint8 length);
void sendframe(void);

//start Bit is rising edge which represent logical 1 , stop bits is ideal line ==> sending 1
static DALI_Frame Frame = {.B= { .startBit = 0b01, .StopBits = -1}};


static DALI_status_t status= DALI_Idle;

static u8 Bit_idx=0;
static u8 max_idx=56;

static u8 wait_tx=20;
static u8 receive = 24;
static u8 wait_rx= 6;


static uint32 manchester_Encoder(uint16 sendBitFrame, uint8 length)
{

	uint32 retValue = 0;
	uint8  mask;
	uint8 i;

	for(i = 0; i < length; i++)
	{
		mask = (sendBitFrame>>i) & 1;

		switch(mask)
		{
			case 0:
				retValue |= 0b10<<(i*2);  // falling edge which represent logical 0
				break;
			case 1:
				retValue |= 0b01<<(i*2); // rising edge which represent logical 1
				break;
			default:
				break;

		}
	}
	return retValue;
}

void sendframe(void)
{   if (status == DALI_Busy){
	if (Bit_idx < max_idx)
	{
		gpio_writePinValue( DALI_PORT, DALI_PIN, (Frame.FullFrame>>Bit_idx) & 1);
		Bit_idx ++;
	}
	else if (wait_tx >0)
	{
		wait_tx --;
	}
	else if (receive >0)
		{
			receive --;
		}
	else if (wait_rx >0)
		{
			wait_rx --;
		}

	else {
		Bit_idx =0;
		  wait_tx=20;
		   receive = 24;
 	 	   wait_rx= 6;
		status = DALI_Idle;

	}
}
}

DALI_enuErrorStatus_t dali_init(void){
	DALI_enuErrorStatus_t error = DALI_enuOk;


	// intiallize the Dali pin

	//1- enable the clock of the dali port

	if (DALI_PORT == GPIO_PORT_A)
	{
		RCC_enuEnablePeripheral(RCC_AHB1 , RCC_AHB1_GPIOA);
	}
	else if (DALI_PORT == GPIO_PORT_B)
	{
		RCC_enuEnablePeripheral(RCC_AHB1 , RCC_AHB1_GPIOB);
	}
	else if (DALI_PORT == GPIO_PORT_C)
	{
			RCC_enuEnablePeripheral(RCC_AHB1 , RCC_AHB1_GPIOC);
	}
	else if (DALI_PORT == GPIO_PORT_D)
	{
			RCC_enuEnablePeripheral(RCC_AHB1 , RCC_AHB1_GPIOD);
	}
	else if (DALI_PORT == GPIO_PORT_E)
	{
			RCC_enuEnablePeripheral(RCC_AHB1 , RCC_AHB1_GPIOE);
	}
	else if (DALI_PORT == GPIO_PORT_H)
	{
			RCC_enuEnablePeripheral(RCC_AHB1 , RCC_AHB1_GPIOH);
	}
	else {
		error = DALI_enuNotOk;
	}
	if (error == DALI_enuOk)
	{
	//2- intiallize the gpio pin
	gpio_Cfg_t dali;
	dali.pin = DALI_PIN;
	dali.port = DALI_PORT;
	dali.speed = DALI_SPEED;
	dali.mode = GPIO_Mode_GP_PP;
	error = gpio_initPin(&dali);
	}
	return error;
}

DALI_enuErrorStatus_t dali_send_command(u8 lamp_address, u16 command){
	DALI_enuErrorStatus_t error = DALI_enuOk;
	if (status == DALI_Busy)
	{
		error = DALI_enuBusy;
	}
	else {
		status = DALI_Busy;
		// set the address part and data part
		Frame.B.YBit = (uint8)manchester_Encoder((lamp_address>>7)&1, 1UL) & 0x03;
		Frame.B.AddBits = manchester_Encoder(lamp_address & 0x3F, 6UL) & 0x0FFF;
		Frame.B.SBit = 0b01; // logical 1 for sending command "rising edge"
		Frame.B.DataBits = manchester_Encoder(command, 16UL);
		// send the first bit
		gpio_writePinValue( DALI_PORT, DALI_PIN, (Frame.FullFrame>>Bit_idx) & 1);
		Bit_idx ++;

	}
	return error;
}


DALI_enuErrorStatus_t dali_set_level(u8 lamp_address, u16 level){
	DALI_enuErrorStatus_t error = DALI_enuOk;
		if (status == DALI_Busy)
		{
			error = DALI_enuBusy;
		}
		else {
			status = DALI_Busy;
			// set the address part and data part
			//Frame.B.YBit = (uint8)manchester_Encoder((lamp_address>>7)&1, 1UL) & 0x03;
			Frame.B.YBit = 0b10;
			Frame.B.AddBits = manchester_Encoder(lamp_address & 0x3F, 6UL) & 0x0FFF;
			Frame.B.SBit = 0b10; // logical 0 for sending data "falling edge"
			Frame.B.DataBits = manchester_Encoder(level, 16UL);
			// send the first bit
			gpio_writePinValue( DALI_PORT, DALI_PIN, (Frame.FullFrame>>Bit_idx) & 1);
			Bit_idx ++;

		}
		return error;
}
