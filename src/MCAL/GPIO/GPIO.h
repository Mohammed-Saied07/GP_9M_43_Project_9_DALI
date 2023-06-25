#ifndef MCAL_GPIO_GPIO_H_
#define MCAL_GPIO_GPIO_H_

#include "../../LIB/STD_TYPES.h"

#define	GPIO_PORT_A 	((void *) 0x40020000)
#define	GPIO_PORT_B 	((void *) 0x40020400)
#define	GPIO_PORT_C 	((void *) 0x40020800)
#define	GPIO_PORT_D		((void *) 0x40020C00)
#define	GPIO_PORT_E 	((void *) 0x40021000)
#define	GPIO_PORT_H 	((void *) 0x40021C00)

typedef enum {
	GPIO_PIN_0 =  0x0F000001,
	GPIO_PIN_1 =  0x0F000002,
	GPIO_PIN_2 =  0x0F000004,
	GPIO_PIN_3 =  0x0F000008,
	GPIO_PIN_4 =  0x0F000010,
	GPIO_PIN_5 =  0x0F000020,
	GPIO_PIN_6 =  0x0F000040,
	GPIO_PIN_7 =  0x0F000080,
	GPIO_PIN_8 =  0x0F000100,
	GPIO_PIN_9 =  0x0F000200,
	GPIO_PIN_10 = 0x0F000400,
	GPIO_PIN_11 = 0x0F000800,
	GPIO_PIN_12 = 0x0F001000,
	GPIO_PIN_13 = 0x0F002000,
	GPIO_PIN_14 = 0x0F004000,
	GPIO_PIN_15 = 0x0F008000,
} GPIO_enuPINs_t;





typedef enum {
	GPIO_Speed_Low = 0, GPIO_Speed_Medium, GPIO_Speed_High, GPIO_Speed_VHigh,
} GPIO_enuSPEED_t;


#define	GPIO_Mode_Input_Floating 	  0x0000
#define	GPIO_Mode_Input_PU		   	  0x0010
#define	GPIO_Mode_Input_PD		  	  0x0020
#define	GPIO_Mode_GP_PP		 	   	  0x0100
#define	GPIO_Mode_GP_PP_PU   	      0x0110
#define	GPIO_Mode_GP_PP_PD   	      0x0120
#define	GPIO_Mode_GP_OD		 	      0x0101
#define	GPIO_Mode_GP_OD_PU	 	      0x0111
#define	GPIO_Mode_GP_OD_PD	 	      0x0121
#define	GPIO_Mode_AF_PP		 	      0x0200
#define	GPIO_Mode_AF_PP_PU 	 	      0x0210
#define	GPIO_Mode_AF_PP_PD	 	      0x0220
#define	GPIO_Mode_AF_OD      	      0x0201
#define	GPIO_Mode_AF_OD_PU   	      0x0211
#define	GPIO_Mode_AF_OD_PD   	      0x0221
#define	GPIO_Mode_Analog		      0x0300


typedef enum {
	GPIO_Value_Low = 0, GPIO_Value_High,
} GPIO_enuValue_t;

typedef enum {
	GPIO_AF_0 = 0, /*for pins from 0 to 15 ==>system		*/
	GPIO_AF_1,	   /*for pins from 0 to 15 ==>TIM1 / TIM2 	*/
	GPIO_AF_2,	   /*for pins from 0 to 15 ==>TIM3..5 		*/
	GPIO_AF_3,	   /*for pins from 0 to 15 ==>TIM9..11 		*/
	GPIO_AF_4,	   /*for pins from 0 to 15 ==>I2C1..3		*/
	GPIO_AF_5,	   /*for pins from 0 to 15 ==>SPI1..4 		*/
	GPIO_AF_6,	   /*for pins from 0 to 15 ==>SPI3	 		*/
	GPIO_AF_7,	   /*for pins from 0 to 15 ==>USART1..2 	*/
	GPIO_AF_8,	   /*for pins from 0 to 15 ==>USART6	 	*/
	GPIO_AF_9,	   /*for pins from 0 to 15 ==>I2C2..3		*/
	GPIO_AF_10,	   /*for pins from 0 to 15 ==>OTG_FS		*/
	GPIO_AF_11,
	GPIO_AF_12,	   /*for pins from 0 to 15 ==>SDIO			*/
	GPIO_AF_13,
	GPIO_AF_14,
	GPIO_AF_15,	   /*for pins from 0 to 15 ==>EVENTOUT		*/
} GPIO_enuAF_t;

typedef enum {
	GPIO_enuOk = 0,
	GPIO_enuNotOk,
	GPIO_enuNullPtr,
	GPIO_enuInvalidPin,
	GPIO_enuInvalidPort,
	GPIO_enuInvalidMode,
	GPIO_enuInvalidSpeed,
	GPIO_enuInvalidValue,
	GPIO_enuInvalidAFValue,
} GPIO_enuErrorStatus_t;

typedef struct {
	u32 pin;
	void * port;
	u16 mode;
	u8 speed;
} gpio_Cfg_t;

/***************************************************************************************/
/*  this function initialize the pin configuration 					          		   */
/*	input : the pin to configure  of type gpio_Cfg_t			                       */
/*	output :Error status (GPIO_enuOk  , GPIO_enuInvalidPin,	GPIO_enuInvalidPort		   */
/* 						 GPIO_enuInvalidSpeed ,GPIO_enuInvalidMode )     		 	   */
/***************************************************************************************/
GPIO_enuErrorStatus_t gpio_initPin(gpio_Cfg_t * Copy_strPin);


/***************************************************************************************/
/*  this function set the pin value in port	with High or Low 		          		   */
/*  note : this function can set more than one pin but they must be at the same port   */
/*  		and set them with the same value		          		 				   */
/*	input : the port of type GPIO_enuPORTs_t from GPIO_PORT_A to GPIO_PORT_H	       */
/*		 the pin of type GPIO_enuPINs_t from GPIO_PIN_0 to GPIO_PIN_15 			       */
/*		 		to set more than one pin use oring (|) between them  				   */
/*		 the value of type GPIO_enuValues_t can be  GPIO_Value_Low or GPIO_Value_HIGH  */
/*	output :Error status (GPIO_enuOk  , GPIO_enuInvalidPin,	GPIO_enuInvalidPort		   */
/* 						 GPIO_enuInvalidValue  )    					 		 	   */
/***************************************************************************************/

GPIO_enuErrorStatus_t gpio_writePinValue(void * Copy_vpPort, u32 Copy_enuPin, u8 Copy_enuValue);

/***************************************************************************************/
/*  this function set the port value 								          		   */
/*	input : the port of type GPIO_enuPORTs_t from GPIO_PORT_A to GPIO_PORT_H	       */
/*		 the value of type u16														   */
/*	output :Error status (GPIO_enuOk ,	GPIO_enuInvalidPort	, GPIO_enuNotOk)		   */
/***************************************************************************************/

GPIO_enuErrorStatus_t gpio_writePortValue(void * Copy_vpPort,
		u16 Copy_u16Value);

/***************************************************************************************/
/*  this function get the value of pin						 		          		   */
/*	input : the port of type GPIO_enuPORTs_t from GPIO_PORT_A to GPIO_PORT_H	       */
/*		 the pin of type GPIO_enuPINs_t from GPIO_PIN_0 to GPIO_PIN_15 			       */
/*		 pointer to the address where the pin value will be stored					   */
/*	output :Error status (GPIO_enuOk  , GPIO_enuInvalidPin,	GPIO_enuInvalidPort)	   */
/***************************************************************************************/

GPIO_enuErrorStatus_t gpio_getPinValue(void * Copy_vpPort,
		u32 Copy_enuPin, u8 *Add_pu8Value);

/***************************************************************************************/
/*  this function get the value of port 							          		   */
/*	input : the port of type GPIO_enuPORTs_t from GPIO_PORT_A to GPIO_PORT_H	       */
/*		 pointer to the address where the port value will be stored					   */
/*	output :Error status (GPIO_enuOk ,	GPIO_enuInvalidPort)		 				   */
/***************************************************************************************/

GPIO_enuErrorStatus_t gpio_getPortValue(void * Copy_vpPort,
		u16* Add_pu16Value);


/***************************************************************************************/
/*  this function select the Alternate function for the pin 		          		   */
/*	input : the port from GPIO_PORT_A to GPIO_PORT_H	       						   */
/*		 the pin from GPIO_PIN_0 to GPIO_PIN_15 								       */
/*		 the AF value of type GPIO_enuAF_t 										       */
/*	output :Error status (GPIO_enuOk  , GPIO_enuInvalidPin,	GPIO_enuInvalidPort		   */
/* 						 GPIO_enuInvalidAFValue  )    					 		 	   */
/***************************************************************************************/

GPIO_enuErrorStatus_t gpio_selectAF(void * Copy_vpPort, u32 Copy_enuPin, GPIO_enuAF_t Copy_enuAFValue);
#endif /* MCAL_GPIO_GPIO_H_ */
