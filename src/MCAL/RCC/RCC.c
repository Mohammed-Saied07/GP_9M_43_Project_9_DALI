#include "../../LIB/STD_TYPES.h"
#include "RCC.h"


/* for selecting system clock */
#define RCC_SW_Mask			0x00000003
#define RCC_SWS_Mask		0x0000000C
#define RCC_SWS_Shift		2


/* setting clock pins */

#define RCC_HSION_PIN		0
#define RCC_HSIRDY_PIN		1
#define RCC_HSEON_PIN		16
#define RCC_HSERDY_PIN		17
#define RCC_HSEBYP_PIN		18
#define RCC_PLLON_PIN		24
#define RCC_PLLRDY_PIN		25
#define RCC_PLLI2SON_PIN	26
#define RCC_PLLI2SRDY_PIN	27


/* PLL CONFIGURATION  */

#define RCC_PLL_Q_Min	2
#define RCC_PLL_Q_Max	15
#define RCC_PLL_Q_Mask	0x0F000000
#define RCC_PLL_Q_Shift	24

#define RCC_PLLSRC_Pin	22

// P value should be even
#define RCC_PLL_P_Min	2
#define RCC_PLL_P_Max	8
#define RCC_PLL_P_Mask	0x00030000
#define RCC_PLL_P_Shift	16

#define RCC_PLL_N_Min	192
#define RCC_PLL_N_Max	432
#define RCC_PLL_N_Mask	0x00007FA0
#define RCC_PLL_N_Shift	6

#define RCC_PLL_M_Min	2
#define RCC_PLL_M_Max	63
#define RCC_PLL_M_Mask	0x0000003F
#define RCC_PLL_M_Shift	0





/* buses prescaler masks */
#define RCC_AHB_Mask 		0x000000F0
#define RCC_AHB_Shift 		4
#define RCC_APB1_Mask 		0x00001C00
#define RCC_APB1_Shift 		10
#define RCC_APB2_Mask 		0x0000E000
#define RCC_APB2_Shift 		13


#define RCC_timeOut			((u16) 1000)

typedef struct {

	u32 RCC_CR;
	u32 RCC_PLLCFGR;
	u32 RCC_CFGR;
	u32 RCC_CIR;
	u32 RCC_AHB1RSTR;
	u32 RCC_AHB2RSTR;
	u32 RCC_RESERVED1;
	u32 RCC_RESERVED2;
	u32 RCC_APB1RSTR;
	u32 RCC_APB2RSTR;
	u32 RCC_RESERVED3;
	u32 RCC_RESERVED4;
	u32 RCC_AHB1ENR;
	u32 RCC_AHB2ENR;
	u32 RCC_RESERVED5;
	u32 RCC_RESERVED6;
	u32 RCC_APB1ENR;
	u32 RCC_APB2ENR;
	u32 RCC_RESERVED7;
	u32 RCC_RESERVED8;
	u32 RCC_AHB1LPENR;
	u32 RCC_AHB2LPENR;
	u32 RCC_RESERVED9;
	u32 RCC_RESERVED10;
	u32 RCC_APB1LPENR;
	u32 RCC_APB2LPENR;
	u32 RCC_RESERVED11;
	u32 RCC_RESERVED12;
	u32 RCC_BDCR;
	u32 RCC_CSR;
	u32 RCC_RESERVED13;
	u32 RCC_RESERVED14;
	u32 RCC_SSCGR;
	u32 RCC_PLLI2SCFGR;
	u32 RCC_RESERVED15;
	u32 RCC_DCKCFGR;

} RCC_strREG_t;

volatile RCC_strREG_t * const RCC =
		((volatile RCC_strREG_t * const ) 0x40023800);

RCC_enuErrorStatus_t RCC_enuSelectSystemClock(
		RCC_enuClockSource_t Copy_enuClockSource) {
	RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_enuOk;
	u16 Loc_Timeout = RCC_timeOut;
	u8 Loc_u8ClockState;
	Loc_enuErrorStatus = RCC_enuGetClockRdyStatus(Copy_enuClockSource,
			&Loc_u8ClockState);
	if (Loc_enuErrorStatus == RCC_enuOk) {
		if (Loc_u8ClockState == RCC_ClockNotReady) {
			Loc_enuErrorStatus = RCC_enuClockSrcNotReady;
		} else {
			RCC->RCC_CFGR &= (~RCC_SW_Mask);
			RCC->RCC_CFGR |= (RCC_SW_Mask & Copy_enuClockSource);
			/*  check SWS pins to see if the selected clock became the actual system clock */
			while (Loc_Timeout--
					&& !(((RCC->RCC_CFGR & RCC_SWS_Mask) >> RCC_SWS_Shift)
							== Copy_enuClockSource))
				;
			if (Loc_Timeout == 0) {
				Loc_enuErrorStatus = RCC_enuTimeOut;
			}
		}
	}

	return Loc_enuErrorStatus;
}


RCC_enuErrorStatus_t RCC_enuGetClockRdyStatus(u8 Copy_u8Clock,
		u8 *Add_pu8RtrnRdyStatus) {

	RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_enuOk;
	if (Add_pu8RtrnRdyStatus == NULL) {
		Loc_enuErrorStatus = RCC_enuNullPtr;
	} else {
		switch (Copy_u8Clock) {
		case (RCC_enuHSI):
		*Add_pu8RtrnRdyStatus = ((RCC->RCC_CR) >> RCC_HSIRDY_PIN) & 0x1;
			break;
		case (RCC_enuHSEBYP):
		case (RCC_enuHSE):
		*Add_pu8RtrnRdyStatus = ((RCC->RCC_CR) >> RCC_HSERDY_PIN) & 0x1;
			break;
		case (RCC_enuPLL):
		*Add_pu8RtrnRdyStatus = ((RCC->RCC_CR) >> RCC_PLLRDY_PIN) & 0x1;
			break;
		case (RCC_enuPLLI2S):
		*Add_pu8RtrnRdyStatus = ((RCC->RCC_CR) >> RCC_PLLI2SRDY_PIN) & 0x1;
			break;
		default:
			Loc_enuErrorStatus = RCC_enuInvalidClockSource;

		}
	}
	return Loc_enuErrorStatus;
}


void RCC_enuGetSystemClock(u8* Add_u8RtrnRdyStatus){
	* Add_u8RtrnRdyStatus = ((RCC->RCC_CFGR & RCC_SWS_Mask) >> RCC_SWS_Shift);

}
RCC_enuErrorStatus_t RCC_enuControlClockSource(
		RCC_enuClockSource_t Copy_enuClockSource,
		RCC_enuClockStatus_t Copy_enuClockStatus) {
	RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_enuOk;
	u16 Loc_Timeout = RCC_timeOut;
	if (((RCC->RCC_CFGR & RCC_SWS_Mask) >> RCC_SWS_Shift) == Copy_enuClockSource )
	{
		Loc_enuErrorStatus = RCC_enuSelectedSystemClock;
	}
	else {
	switch (Copy_enuClockSource) {
	case (RCC_enuHSI):
		switch (Copy_enuClockStatus) {
		case (RCC_enuClockOFF):
			RCC->RCC_CR &= ~(1 << RCC_HSION_PIN);
			break;
		case (RCC_enuClockON):
			RCC->RCC_CR |= (1 << RCC_HSION_PIN);
			while (Loc_Timeout-- && !(((RCC->RCC_CR) >> RCC_HSIRDY_PIN) & 0x1))
				;
			if (Loc_Timeout == 0) {
				Loc_enuErrorStatus = RCC_enuTimeOut;

			}
			break;
		default:
			Loc_enuErrorStatus = RCC_enuInvalidClockStatus;
		}

		break;
	case (RCC_enuHSEBYP):
		switch (Copy_enuClockStatus) {
		case (RCC_enuClockOFF):
			RCC->RCC_CR &= ~(1 << RCC_HSEBYP_PIN);
			break;
		case (RCC_enuClockON):
			RCC->RCC_CR |= (1 << RCC_HSEON_PIN);
			RCC->RCC_CR |= (1 << RCC_HSEBYP_PIN);
			while (Loc_Timeout-- && !(((RCC->RCC_CR) >> RCC_HSERDY_PIN) & 0x1))
				;
			if (Loc_Timeout == 0) {
				Loc_enuErrorStatus = RCC_enuTimeOut;

			}
			break;
		default:
			Loc_enuErrorStatus = RCC_enuInvalidClockStatus;
		}
		break;
	case (RCC_enuHSE):
		switch (Copy_enuClockStatus) {
		case (RCC_enuClockOFF):
			RCC->RCC_CR &= ~(1 << RCC_HSEON_PIN);
			break;
		case (RCC_enuClockON):
			RCC->RCC_CR |= (1 << RCC_HSEON_PIN);
			while (Loc_Timeout-- && !(((RCC->RCC_CR) >> RCC_HSERDY_PIN) & 0x1))
				;
			if (Loc_Timeout == 0) {
				Loc_enuErrorStatus = RCC_enuTimeOut;

			}
			break;
		default:
			Loc_enuErrorStatus = RCC_enuInvalidClockStatus;
		}

		break;
	case (RCC_enuPLL):
		switch (Copy_enuClockStatus) {
		case (RCC_enuClockOFF):
			RCC->RCC_CR &= ~(1 << RCC_PLLON_PIN);
			break;
		case (RCC_enuClockON):
			RCC->RCC_CR |= (1 << RCC_PLLON_PIN);
			while (Loc_Timeout-- && !(((RCC->RCC_CR) >> RCC_PLLRDY_PIN) & 0x1))
				;
			if (Loc_Timeout == 0) {
				Loc_enuErrorStatus = RCC_enuTimeOut;

			}
			break;
		default:
			Loc_enuErrorStatus = RCC_enuInvalidClockStatus;
		}

		break;
	case (RCC_enuPLLI2S):
		switch (Copy_enuClockStatus) {
		case (RCC_enuClockOFF):
			RCC->RCC_CR &= ~(1 << RCC_PLLI2SON_PIN);
			break;
		case (RCC_enuClockON):
			RCC->RCC_CR |= (1 << RCC_PLLI2SON_PIN);
			while (Loc_Timeout--
					&& !(((RCC->RCC_CR) >> RCC_PLLI2SRDY_PIN) & 0x1))
				;
			if (Loc_Timeout == 0) {
				Loc_enuErrorStatus = RCC_enuTimeOut;

			}
			break;
		default:
			Loc_enuErrorStatus = RCC_enuInvalidClockStatus;
		}

		break;
	default:
		Loc_enuErrorStatus = RCC_enuInvalidClockSource;

	}}

	return Loc_enuErrorStatus;

}

RCC_enuErrorStatus_t RCC_enuConfigurePLL(RCC_enuPLLSource_t Copy_enuPLLSource,
		u16 Copy_u16PValue, u16 Copy_u16QValue, u16 Copy_u16NValue,
		u16 Copy_u16MValue) {

	RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_enuOk;
	/* check if PLL is on " can't configure PLL while it's on "  */
	if ((RCC->RCC_CR >> RCC_PLLON_PIN )== 1){
		Loc_enuErrorStatus = RCC_enuPLLON;
	}
	else{
	/* check if  P value is valid */
	if (Copy_u16PValue >= RCC_PLL_P_Min && Copy_u16PValue <= RCC_PLL_P_Max
			&& Copy_u16PValue % 2 == 0) { /* Set P value */
		/* to convert 2 to 00 , 4 to 01 , 6 to 10 , 8 to 11 ==> divide the value of P by 2 then subtract 1  */
		Copy_u16PValue = (Copy_u16PValue / 2) - 1;
		Copy_u16PValue = (Copy_u16PValue << RCC_PLL_P_Shift);
		RCC->RCC_PLLCFGR |= (RCC_PLL_P_Mask & Copy_u16PValue);
		/* check if  Q value is valid */
		if (Copy_u16QValue >= RCC_PLL_Q_Min && Copy_u16QValue <= RCC_PLL_Q_Max) { /* Set Q value */
			Copy_u16QValue = (Copy_u16QValue << RCC_PLL_Q_Shift);
			RCC->RCC_PLLCFGR |= (RCC_PLL_Q_Mask & Copy_u16QValue);
			/* check if  N value is valid */
			if (Copy_u16NValue >= RCC_PLL_N_Min
					&& Copy_u16NValue <= RCC_PLL_N_Max) { /* Set N value */
				Copy_u16NValue = (Copy_u16NValue << RCC_PLL_N_Shift);
				RCC->RCC_PLLCFGR |= (RCC_PLL_N_Mask & Copy_u16NValue);
				/* check if  M value is valid */
				if (Copy_u16MValue >= RCC_PLL_M_Min
						&& Copy_u16MValue <= RCC_PLL_M_Max) { /* Set M value */
					Copy_u16MValue = (Copy_u16MValue << RCC_PLL_M_Shift);
					RCC->RCC_PLLCFGR |= (RCC_PLL_M_Mask & Copy_u16MValue);
					/* Check if the source is valid  */
					switch (Copy_enuPLLSource) {
					case (RCC_enuPLL_srcHSI):
						/* check if the desired source is enable and ready  */
						if ((((RCC->RCC_CR) >> RCC_HSIRDY_PIN) & 0x1) == 1) {
							/* put 0 in the PLLSRC Pin in RCC_PLLCFG register */
							RCC->RCC_PLLCFGR &= (~(1 << RCC_PLLSRC_Pin));
						} else {
							Loc_enuErrorStatus = RCC_enuPLLSrcNotReady;
						}
						break;
					case (RCC_enuPLL_srcHSE):
						/* check if the desired source is enable and ready  */
						if ((((RCC->RCC_CR) >> RCC_HSIRDY_PIN) & 0x1) == 1) {
							/* put 1 in the PLLSRC Pin in RCC_PLLCFG register */
							RCC->RCC_PLLCFGR |= (1 << RCC_PLLSRC_Pin);
						} else {
							Loc_enuErrorStatus = RCC_enuPLLSrcNotReady;
						}
						break;
					default:
						Loc_enuErrorStatus = RCC_enuInvalidClockSource;
					}

				} else {
					Loc_enuErrorStatus = RCC_enuInvalidPLLCfg;
				}

			} else {
				Loc_enuErrorStatus = RCC_enuInvalidPLLCfg;
			}

		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPLLCfg;
		}
	} else {
		Loc_enuErrorStatus = RCC_enuInvalidPLLCfg;
	}
	}
	return Loc_enuErrorStatus;
}

RCC_enuErrorStatus_t RCC_enuControlBusPrescaler(u32 Copy_u32Bus,
		RCC_enuBusPrescaler_t Copy_u32Prescaler) {
	RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_enuOk;
	switch (Copy_u32Bus) {
	case (RCC_AHB):
		if (Copy_u32Prescaler > RCC_enuSysClk_Div512
				|| Copy_u32Prescaler < RCC_enuSysClk_Div2) {
			Loc_enuErrorStatus = RCC_enuInvalidPrescaler;
		} else {
			RCC->RCC_CFGR |= (RCC_AHB_Mask
					& (Copy_u32Prescaler << RCC_AHB_Shift));
		}

		break;

	case (RCC_APB1):
		if (Copy_u32Prescaler > RCC_enuAHB_Div16
				|| Copy_u32Prescaler < RCC_enuAHB_Div2) {
			Loc_enuErrorStatus = RCC_enuInvalidPrescaler;
		} else {
			RCC->RCC_CFGR |= (RCC_APB1_Mask
					& (Copy_u32Prescaler << RCC_APB1_Shift));
		}

		break;
	case (RCC_APB2):
		if (Copy_u32Prescaler > RCC_enuAHB_Div16
				|| Copy_u32Prescaler < RCC_enuAHB_Div2) {
			Loc_enuErrorStatus = RCC_enuInvalidPrescaler;
		} else {
			RCC->RCC_CFGR |= (RCC_APB2_Mask
					& (Copy_u32Prescaler << RCC_APB2_Shift));
		}

		break;
		break;
	default:
		Loc_enuErrorStatus = RCC_enuInvalidBus;

	}
	return Loc_enuErrorStatus;

}

RCC_enuErrorStatus_t RCC_enuResetPeripheral(u32 Copy_u32Bus,
		u32 Copy_u32Peripheral) {
	RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_enuOk;
	switch (Copy_u32Bus) {
	case (RCC_AHB1):
		if ((Copy_u32Peripheral & (0x0F000000)) == RCC_AHB1) {
			Copy_u32Peripheral &= 0xF0FFFFFF;
			RCC->RCC_AHB1RSTR |= Copy_u32Peripheral;
		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPeripheral;
		}
		break;
	case (RCC_AHB2):
		if ((Copy_u32Peripheral & (0x0F000000)) == RCC_AHB2) {
			Copy_u32Peripheral &= 0xF0FFFFFF;
			RCC->RCC_AHB2RSTR |= Copy_u32Peripheral;
		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPeripheral;
		}
		break;

	case (RCC_APB1):
		if ((Copy_u32Peripheral & (0x0F000000)) == RCC_APB1) {
			Copy_u32Peripheral &= 0xF0FFFFFF;
			RCC->RCC_APB1RSTR |= Copy_u32Peripheral;
		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPeripheral;
		}
		break;
	case (RCC_APB2):
		if ((Copy_u32Peripheral & (0x0F000000)) == RCC_APB2) {
			Copy_u32Peripheral &= 0xF0FFFFFF;
			RCC->RCC_APB2RSTR |= Copy_u32Peripheral;
		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPeripheral;
		}
		break;
	default:
		Loc_enuErrorStatus = RCC_enuInvalidBus;

	}
	return Loc_enuErrorStatus;
}

RCC_enuErrorStatus_t RCC_enuEnablePeripheral(u32 Copy_u32Bus,
		u32 Copy_u32Peripheral) {
	RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_enuOk;
	switch (Copy_u32Bus) {
	case (RCC_AHB1):
		if ((Copy_u32Peripheral & (0x0F000000)) == RCC_AHB1) {
			Copy_u32Peripheral &= 0xF0FFFFFF;
			RCC->RCC_AHB1ENR |= Copy_u32Peripheral;
		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPeripheral;
		}
		break;
	case (RCC_AHB2):
		if ((Copy_u32Peripheral & (0x0F000000)) == RCC_AHB2) {
			Copy_u32Peripheral &= 0xF0FFFFFF;
			RCC->RCC_AHB2ENR |= Copy_u32Peripheral;
		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPeripheral;
		}
		break;

	case (RCC_APB1):
		if ((Copy_u32Peripheral & (0x0F000000)) == RCC_APB1) {
			Copy_u32Peripheral &= 0xF0FFFFFF;
			RCC->RCC_APB1ENR |= Copy_u32Peripheral;
		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPeripheral;
		}
		break;
	case (RCC_APB2):
		if ((Copy_u32Peripheral & (0x0F000000)) == RCC_APB2) {
			Copy_u32Peripheral &= 0xF0FFFFFF;
			RCC->RCC_APB2ENR |= Copy_u32Peripheral;
		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPeripheral;
		}
		break;
	default:
		Loc_enuErrorStatus = RCC_enuInvalidBus;

	}
	return Loc_enuErrorStatus;
}

RCC_enuErrorStatus_t RCC_enuDisablePeripheral(u32 Copy_u32Bus,
		u32 Copy_u32Peripheral) {
	RCC_enuErrorStatus_t Loc_enuErrorStatus = RCC_enuOk;
	switch (Copy_u32Bus) {
	case (RCC_AHB1):
		if ((Copy_u32Peripheral & (0x0F000000)) == RCC_AHB1) {
			Copy_u32Peripheral &= 0xF0FFFFFF;
			RCC->RCC_AHB1ENR &= (~Copy_u32Peripheral);
		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPeripheral;
		}
		break;
	case (RCC_AHB2):
		if ((Copy_u32Peripheral & (0x0F000000)) == RCC_AHB2) {
			Copy_u32Peripheral &= 0xF0FFFFFF;
			RCC->RCC_AHB2ENR &= (~Copy_u32Peripheral);
		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPeripheral;
		}
		break;

	case (RCC_APB1):
		if ((Copy_u32Peripheral & (0x0F000000)) == RCC_APB1) {
			Copy_u32Peripheral &= 0xF0FFFFFF;
			RCC->RCC_APB1ENR &= (~Copy_u32Peripheral);
		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPeripheral;
		}
		break;
	case (RCC_APB2):
		if ((Copy_u32Peripheral & (0x0F000000)) == RCC_APB2) {
			Copy_u32Peripheral &= 0xF0FFFFFF;
			RCC->RCC_APB2ENR &= (~Copy_u32Peripheral);
		} else {
			Loc_enuErrorStatus = RCC_enuInvalidPeripheral;
		}
		break;
	default:
		Loc_enuErrorStatus = RCC_enuInvalidBus;

	}
	return Loc_enuErrorStatus;
}
