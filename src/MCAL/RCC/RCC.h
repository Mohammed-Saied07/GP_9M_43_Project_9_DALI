#ifndef MCAL_RCC_RCC_H_
#define MCAL_RCC_RCC_H_

/*  Buses  */
#define RCC_AHB 			0x00000000
#define RCC_AHB1 			0x01000000
#define RCC_AHB2 			0x02000000
#define RCC_APB1 			0x04000000
#define RCC_APB2 			0x08000000

/* AHB1 Peripherals masks */

#define RCC_AHB1_GPIOA		0x01000001
#define RCC_AHB1_GPIOB		0x01000002
#define RCC_AHB1_GPIOC		0x01000004
#define RCC_AHB1_GPIOD		0x01000008
#define RCC_AHB1_GPIOE		0x01000010
#define RCC_AHB1_GPIOH		0x01000080
#define RCC_AHB1_CRCRST		0x01001000
#define RCC_AHB1_DMA1		0x01200000
#define RCC_AHB1_DMA2		0x01400000

/* AHB2 Peripherals masks */

#define RCC_AHB2_OTGFS 		0x02000080

/* APB1 Peripherals masks */

#define RCC_APB1_TIM2 		0x04000001
#define RCC_APB1_TIM3 		0x04000002
#define RCC_APB1_TIM4 		0x04000004
#define RCC_APB1_TIM5 		0x04000008
#define RCC_APB1_WWDG 		0x04000800
#define RCC_APB1_SPI2 		0x04004000
#define RCC_APB1_SPI3 		0x04008000
#define RCC_APB1_USART2 	0x04020000
#define RCC_APB1_I2C1 		0x04200000
#define RCC_APB1_I2C2 		0x04400000
#define RCC_APB1_I2C3 		0x04800000
#define RCC_APB1_PWR 		0x14000000

/* APB2 Peripherals masks */

#define RCC_APB2_TIM1 		0x08000001
#define RCC_APB2_USART1 	0x08000010
#define RCC_APB2_USART6 	0x08000020
#define RCC_APB2_ADC1 		0x08000100
#define RCC_APB2_SDIO 		0x08000800
#define RCC_APB2_SPI1 		0x08001000
#define RCC_APB2_SPI4 		0x08002000
#define RCC_APB2_SYSCFG 	0x08004000
#define RCC_APB2_TIM9 		0x08010000
#define RCC_APB2_TIM10 		0x08020000
#define RCC_APB2_TIM11		0x08040000

typedef enum {
	RCC_enuOk = 0,
	RCC_enuNotOk,
	RCC_enuNullPtr,
	RCC_enuClockSrcNotReady,
	RCC_enuInvalidClockSource,
	RCC_enuInvalidClockStatus,
	RCC_enuSelectedSystemClock,
	RCC_enuTimeOut,
	RCC_enuInvalidPLLCfg,
	RCC_enuPLLSrcNotReady,
	RCC_enuPLLON,
	RCC_enuInvalidBus,
	RCC_enuInvalidPrescaler,
	RCC_enuInvalidPeripheral,
} RCC_enuErrorStatus_t;

typedef enum {
	RCC_enuHSI = 0, RCC_enuHSE, RCC_enuPLL, RCC_enuHSEBYP, RCC_enuPLLI2S
} RCC_enuClockSource_t;

typedef enum {
	RCC_enuClockON = 0, RCC_enuClockOFF,

} RCC_enuClockStatus_t;

typedef enum {
	RCC_ClockNotReady = 0, RCC_ClockReady,

} RCC_enuClockReadyStatus_t;

typedef enum {
	RCC_enuPLL_srcHSI = 0, RCC_enuPLL_srcHSE,

} RCC_enuPLLSource_t;

typedef enum {
	RCC_enuAHB_Div2 = 4,
	RCC_enuAHB_Div4,
	RCC_enuAHB_Div8,
	RCC_enuAHB_Div16,
	RCC_enuSysClk_Div2 = 8,
	RCC_enuSysClk_Div4,
	RCC_enuSysClk_Div8,
	RCC_enuSysClk_Div16,
	RCC_enuSysClk_Div64,
	RCC_enuSysClk_Div128,
	RCC_enuSysClk_Div256,
	RCC_enuSysClk_Div512,

} RCC_enuBusPrescaler_t;



/***************************************************************************************/
/*  this function select system clock source						          		   */
/*	input : the system clock source of type RCC_enuClockSource_t                       */
/* 				(the available options are RCC_enuHSI ,RCC_enuHSE ,RCC_enuPLL)	       */
/*	output :Error status (RCC_enuOk  , RCC_enuInvalidClockSource,					   */
/* 						 RCC_enuClockSrcNotReady ,RCC_enuTimeOut )    		   	 	   */
/***************************************************************************************/

RCC_enuErrorStatus_t RCC_enuSelectSystemClock(
		RCC_enuClockSource_t Copy_enuClockSource);

/***************************************************************************************/
/*  this function return status of the clock ready or not ready			       		   */
/*	input : the clock source and pointer to the variable store the status              */
/*	output :Error status (RCC_enuOk  , RCC_enuInvalidClockSource)					   */
/***************************************************************************************/

RCC_enuErrorStatus_t RCC_enuGetClockRdyStatus(u8 Copy_u8Clock , u8* Add_pu8RtrnRdyStatus);

/***************************************************************************************/
/*  this function return the selected system clock						       		   */
/*	input :  pointer to the variable store the selected clock			               */
/*	output :void																	   */
/***************************************************************************************/
void RCC_enuGetSystemClock(u8* Add_pu8RtrnRdyStatus);

/***************************************************************************************/
/*  this function control the clock source	enable or disable clock	          		   */
/*	input : the clock to control of type RCC_enuClockSource_t                          */
/* 				and the clock status (on or off) of type RCC_enuClockStatus_t	       */
/*	output :Error status (RCC_enuOk  , RCC_enuInvalidClockSource,					   */
/* 						 RCC_enuInvalidClockStatus ,RCC_enuTimeOut )    		   	   */
/***************************************************************************************/

RCC_enuErrorStatus_t RCC_enuControlClockSource(
		RCC_enuClockSource_t Copy_enuClockSource,
		RCC_enuClockStatus_t Copy_enuClockStatus);

/***************************************************************************************/
/*  this function configure PLL Clock								           		   */
/*	input :The PLL Source (HSI or HSE ) of type RCC_enuPLLSource_t                     */
/* 				the P value should be 2 or 4 or 6 or 8		                           */
/* 			    the Q value should be integer between 2 to 15 						   */
/* 				the N value should be integer between 192 to 432					   */
/* 				the M value should be integer between 2 to 63 						   */
/*	output :Error status (RCC_enuOk  , RCC_enuInvalidPLLCfg,RCC_enuPLLSrcNotReady,	   */
/*						  RCC_enuPLLON,RCC_enuInvalidClockSource)    				   */
/***************************************************************************************/

RCC_enuErrorStatus_t RCC_enuConfigurePLL(RCC_enuPLLSource_t Copy_enuPLLSource,
		u16 Copy_u16PValue, u16 Copy_u16QValue, u16 Copy_u16NValue,
		u16 Copy_u16MValue);

/***************************************************************************************/
/*  this function control the prescaler of buses				           			   */
/*	input : the bus to set the prescaler choose from :                                 */
/* 				RCC_AHB , RCC_APB1 , RCC_APB2 	                                       */
/* 			    and the prescaler of type 	RCC_enuBusPrescaler_t					   */
/*  			for AHB bus choose prescaler value:									   */
/*  			         from RCC_enuSysClk_Div2 to RCC_enuSysClk_Div512 			   */
/*  			for APB1 and APB2 buses choose prescaler value: 					   */
/*  					 from RCC_enuAHB_Div2 to RCC_enuAHB_Div16 					   */
/*	output :Error status (RCC_enuOk  , RCC_enuInvalidBus, RCC_enuInvalidPrescaler)     */
/***************************************************************************************/

RCC_enuErrorStatus_t RCC_enuControlBusPrescaler(u32 Copy_u32Bus,
		RCC_enuBusPrescaler_t Copy_u32Prescaler);

/***************************************************************************************/
/*  this function reset one or more peripherals on the same bus           			   */
/*	input : the bus the peripherals are located choose from :                          */
/* 				RCC_AHB1 , RCC_AHB2 , RCC_APB1 , RCC_APB2 	                           */
/* 			    and the peripheral to reset											   */
/*  note : to reset more than one per. in the same bus use oring betwwen them (|)      */
/*		===>	per. in  RCC_AHB1 : RCC_AHB1_GPIOA , RCC_AHB1_GPIOB , RCC_AHB1_GPIOC   */
/*					RCC_AHB1_GPIOD , RCC_AHB1_GPIOE , RCC_AHB1_GPIOH ,RCC_AHB1_CRCRST  */
/*					RCC_AHB1_DM1 , RCC_AHB1_DM2										   */
/*		===>	per. in  RCC_AHB2 : RCC_AHB2_OTGFS									   */
/*		===>	per. in  RCC_APB1: RCC_APB1_TIM2 , RCC_APB1_TIM3 , RCC_APB1_TIM4	   */
/*					RCC_APB1_TIM5 , RCC_APB1_WWDG , RCC_APB1_SPI2 , RCC_APB1_SPI3 	   */
/*					RCC_APB1_USART2 , RCC_APB1_I2C1 , RCC_APB1_I2C2	  				   */
/*					RCC_APB1_I2C3 , RCC_APB1_PWR								 	   */
/*		===>	per. in  RCC_APB2: RCC_APB2_TIM1, RCC_APB2_USART1 , RCC_APB2_USART6	   */
/*					RCC_APB2_ADC1 , RCC_APB2_SDIO , RCC_APB2_SPI1 , RCC_APB2_SPI4 	   */
/*					RCC_APB2_SYSCFG , RCC_APB2_TIM9 , RCC_APB2_TIM10 , RCC_APB2_TIM11  */
/*	output :Error status (RCC_enuOk  , RCC_enuInvalidBus, RCC_enuInvalidPeripheral)    */
/***************************************************************************************/

RCC_enuErrorStatus_t RCC_enuResetPeripheral(u32 Copy_u32Bus,
		u32 Copy_u32Peripheral);

/***************************************************************************************/
/*  this function Enable one or more peripherals on the same bus           			   */
/*	input : the bus the peripherals are located choose from :                          */
/* 				RCC_AHB1 , RCC_AHB2 , RCC_APB1 , RCC_APB2 	                           */
/* 			    and the peripheral to reset											   */
/*  note : to Enable more than one per. in the same bus use oring betwwen them (|)      */
/*		===>	per. in  RCC_AHB1 : RCC_AHB1_GPIOA , RCC_AHB1_GPIOB , RCC_AHB1_GPIOC   */
/*					RCC_AHB1_GPIOD , RCC_AHB1_GPIOE , RCC_AHB1_GPIOH ,RCC_AHB1_CRCRST  */
/*					RCC_AHB1_DM1 , RCC_AHB1_DM2										   */
/*		===>	per. in  RCC_AHB2 : RCC_AHB2_OTGFS									   */
/*		===>	per. in  RCC_APB1: RCC_APB1_TIM2 , RCC_APB1_TIM3 , RCC_APB1_TIM4	   */
/*					RCC_APB1_TIM5 , RCC_APB1_WWDG , RCC_APB1_SPI2 , RCC_APB1_SPI3 	   */
/*					RCC_APB1_USART2 , RCC_APB1_I2C1 , RCC_APB1_I2C2	  				   */
/*					RCC_APB1_I2C3 , RCC_APB1_PWR								 	   */
/*		===>	per. in  RCC_APB2: RCC_APB2_TIM1, RCC_APB2_USART1 , RCC_APB2_USART6	   */
/*					RCC_APB2_ADC1 , RCC_APB2_SDIO , RCC_APB2_SPI1 , RCC_APB2_SPI4 	   */
/*					RCC_APB2_SYSCFG , RCC_APB2_TIM9 , RCC_APB2_TIM10 , RCC_APB2_TIM11  */
/*	output :Error status (RCC_enuOk  , RCC_enuInvalidBus, RCC_enuInvalidPeripheral)    */
/***************************************************************************************/

RCC_enuErrorStatus_t RCC_enuEnablePeripheral(u32 Copy_u32Bus,
		u32 Copy_u32Peripheral);

/***************************************************************************************/
/*  this function Disable one or more peripherals on the same bus           		   */
/*	input : the bus the peripherals are located choose from :                          */
/* 				RCC_AHB1 , RCC_AHB2 , RCC_APB1 , RCC_APB2 	                           */
/* 			    and the peripheral to reset											   */
/*  note : to Disable more than one per. in the same bus use oring betwwen them (|)    */
/*		===>	per. in  RCC_AHB1 : RCC_AHB1_GPIOA , RCC_AHB1_GPIOB , RCC_AHB1_GPIOC   */
/*					RCC_AHB1_GPIOD , RCC_AHB1_GPIOE , RCC_AHB1_GPIOH ,RCC_AHB1_CRCRST  */
/*					RCC_AHB1_DM1 , RCC_AHB1_DM2										   */
/*		===>	per. in  RCC_AHB2 : RCC_AHB2_OTGFS									   */
/*		===>	per. in  RCC_APB1: RCC_APB1_TIM2 , RCC_APB1_TIM3 , RCC_APB1_TIM4	   */
/*					RCC_APB1_TIM5 , RCC_APB1_WWDG , RCC_APB1_SPI2 , RCC_APB1_SPI3 	   */
/*					RCC_APB1_USART2 , RCC_APB1_I2C1 , RCC_APB1_I2C2	  				   */
/*					RCC_APB1_I2C3 , RCC_APB1_PWR								 	   */
/*		===>	per. in  RCC_APB2: RCC_APB2_TIM1, RCC_APB2_USART1 , RCC_APB2_USART6	   */
/*					RCC_APB2_ADC1 , RCC_APB2_SDIO , RCC_APB2_SPI1 , RCC_APB2_SPI4 	   */
/*					RCC_APB2_SYSCFG , RCC_APB2_TIM9 , RCC_APB2_TIM10 , RCC_APB2_TIM11  */
/*	output :Error status (RCC_enuOk  , RCC_enuInvalidBus, RCC_enuInvalidPeripheral)    */
/***************************************************************************************/

RCC_enuErrorStatus_t RCC_enuDisablePeripheral(u32 Copy_u32Bus,
		u32 Copy_u32Peripheral);


#endif /* MCAL_RCC_RCC_H_ */
