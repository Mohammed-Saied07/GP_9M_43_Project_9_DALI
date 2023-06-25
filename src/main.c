
#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"


#include "LIB/STD_TYPES.h"
#include "MCAL/NVIC/NVIC.h"
#include "MCAL/SYSTICK/SYSTICK.h"
#include "MCAL/GPIO/GPIO.h"
#include "MCAL/RCC/RCC.h"
#include "MCAL/ADC/ADC.h"
#include "MCAL/USART/USART.h"
#include "HAL/DALI/DALI.h"
#include "HAL/LDR/LDR.h"
#include "HAL/SWITCH/SWITCH.h"
#include "Service/Sched/Sched.h"

static u8 message[2];

void App_Task(void){

	u8 SW_val ;
	LDR_enuVisibleStatus_t LDR_val;
	Switch_getValue(Switch_ON  , &SW_val);
	switch (SW_val){
	case Switch_State_Pressed:
		/*  automatic mode detecting intensity using LDR*/
		LDR_ReadStatus(ADC_IN0 , & LDR_val);
		switch (LDR_val){
		case LDR_Dark:
			dali_set_level(DALI_Broadcast,DALI_Maximum_Intensity);
			break;
		case LDR_Visible:
			dali_set_level(DALI_Broadcast,DALI_Minimum_Intensity);
			break;
		}
		break;
	case Switch_State_NotPressed:
		/*  manual mode control intensity from GUI */
		USART_enuRecieveBuffer(USART1,message, 2);
		u16 val = message[0] + (message[1]<<8);
		u8 map_val;
		if (val<=100){
			/* intensity of lamp 1 */
			map_val = val*255/100;
			dali_set_level(DALI_Lamp_1,map_val);
		}
		else if (val<=202){
			/* intensity of lamp 2 */
			val = val-101;
			map_val = val*255/100;
			dali_set_level(DALI_Lamp_2,map_val);
		}
		else if (val<=303){
			/* intensity of lamp 3 */
			val = val-203;
			map_val = val*255/100;
			dali_set_level(DALI_Lamp_3,map_val);
		}
		else if (val<=404){
			/* intensity of lamp 4 */
			val = val-304;
			map_val = val*255/100;
			dali_set_level(DALI_Lamp_4,map_val);
		}
		else if (val<=505){
			/* intensity of group1 */
			val = val-405;
			map_val = val*255/100;
			dali_set_level(DALI_Group_1,map_val);
		}
		else if (val<=606){
			/* intensity of group2 */
			val = val-506;
			map_val = val*255/100;
			dali_set_level(DALI_Group_2,map_val);
		}
		else if (val<=707){
			/* intensity of group3 */
			val = val-607;
			map_val = val*255/100;
			dali_set_level(DALI_Group_3,map_val);
		}
		else if (val<=808){
			/* intensity of all lamps */
			val = val-708;
			map_val = val*255/100;
			dali_set_level(DALI_Broadcast,map_val);
		}
		break;

	}

}

int main(void)
{
	RCC_enuControlClockSource(RCC_enuHSE, RCC_enuClockON);
	RCC_enuSelectSystemClock(RCC_enuHSE);
	RCC_enuEnablePeripheral(RCC_AHB1 , RCC_AHB1_GPIOA);
	RCC_enuEnablePeripheral(RCC_AHB1 , RCC_AHB1_GPIOB);
	NVIC_EnableAllInterrupts();
	gpio_Cfg_t ldr,uart_tx,uart_rx;
	ldr.mode= GPIO_Mode_Analog; //ldr pin connected to adc
	ldr.pin = GPIO_PIN_0;
	ldr.port = GPIO_PORT_A;
	ldr.speed = GPIO_Speed_VHigh;
	gpio_initPin(&ldr);
    uart_tx.mode = GPIO_Mode_AF_PP;
    uart_tx.pin = GPIO_PIN_6;
    uart_tx.port= GPIO_PORT_B;
    uart_tx.speed = GPIO_Speed_VHigh;
    gpio_initPin(&uart_tx);
    gpio_selectAF(GPIO_PORT_B, GPIO_PIN_6, GPIO_AF_7);
    uart_rx.mode = GPIO_Mode_AF_PP;
    uart_rx.pin = GPIO_PIN_7;
    uart_rx.port= GPIO_PORT_B;
    uart_rx.speed = GPIO_Speed_VHigh;
    gpio_initPin(&uart_rx);
    gpio_selectAF(GPIO_PORT_B, GPIO_PIN_7, GPIO_AF_7);
	dali_init();
	Switch_init();
	LDR_Init();
	USART_cfg_t uart;
	uart.Oversampling =USART_OVSAMPLING_8 ;
	uart.USART_Enable = USART_ENABLE;
	uart.WordLength = USART_8_BIT;
	uart.ParityControl =USART_NO_PARITY ;
	uart.PEIE_Int = USART_PEIE_DISABLE;
	uart.TXEIE_Int = USART_TXEIE_DISABLE;
	uart.TXCIE_Int = USART_TCIE_DISABLE;
	uart.TxEnable =USART_TE_DISABLE;
	uart.IDLEIE_Int =USART_IDLEIE_DISABLE;
	uart.RXNEIE_Int = USART_RXNEIE_ENABLE;
	uart.RxEnable = USART_RE_ENABLE;
	uart.StopBits =USART_1_STOP_BIT;
	uart.BaudRate = 9600;
	USART_vidInit(USART1, &uart);
	Sched_Init();
	Sched_Start();
  return 0;
}
