#include "../../LIB/STD_TYPES.h"
#include "../RCC/RCC.h"
#include "GPIO.h"

#define GPIO_ValidPinMask 		0x0F000000

#define GPIO_Mode_Mask 			0x00000003
#define GPIO_Mode_Shift 		8
#define GPIO_Speed_Mask 		0x00000003
#define GPIO_PUPD_Mask 			0x00000003
#define GPIO_PUPD_Shift			4

#define GPIO_Reset_Shift		16

#define GPIO_AF_Mask 			0x0000000F

typedef struct {
	u32 GPIO_MODER;
	u32 GPIO_OTYPER;
	u32 GPIO_OSPEEDR;
	u32 GPIO_PUPDR;
	u32 GPIO_IDR;
	u32 GPIO_ODR;
	u32 GPIO_BSRR;
	u32 GPIO_LCKR;
	u32 GPIO_AFRL;
	u32 GPIO_AFRH;
} GPIO_strReg_t;

GPIO_enuErrorStatus_t gpio_initPin(gpio_Cfg_t * Copy_strPin) {

	GPIO_enuErrorStatus_t Loc_enuErrorStatus = GPIO_enuOk;
	if ((Copy_strPin->pin & GPIO_ValidPinMask) != GPIO_ValidPinMask) {
		Loc_enuErrorStatus = GPIO_enuInvalidPin;
	} else if (Copy_strPin->mode > GPIO_Mode_Analog) {
		Loc_enuErrorStatus = GPIO_enuInvalidMode;
	} else if (Copy_strPin->speed > GPIO_Speed_VHigh) {
		Loc_enuErrorStatus = GPIO_enuInvalidSpeed;
	} else if (Copy_strPin->port == NULL
			|| (Copy_strPin->port != GPIO_PORT_A
					&& Copy_strPin->port != GPIO_PORT_B
					&& Copy_strPin->port != GPIO_PORT_C
					&& Copy_strPin->port != GPIO_PORT_D
					&& Copy_strPin->port != GPIO_PORT_E
					&& Copy_strPin->port != GPIO_PORT_H)) {
		Loc_enuErrorStatus = GPIO_enuInvalidPort;
	} else {
		u8 Loc_u8PinNum = -1;
		u32 Loc_u32Pin = Copy_strPin->pin;
		Loc_u32Pin &= ~ GPIO_ValidPinMask;
		while (Loc_u32Pin) {
			Loc_u8PinNum++;
			Loc_u32Pin >>= 1;
		}
		volatile GPIO_strReg_t * const GPIO =
				(volatile GPIO_strReg_t * const ) Copy_strPin->port;


		u16 Loc_PUPD = Copy_strPin->mode ;
		Loc_PUPD >>=  GPIO_PUPD_Shift;
		Loc_PUPD &= GPIO_PUPD_Mask;

		u32 Loc_u32Temp = GPIO->GPIO_PUPDR;
		/*clear the PUPD bits for the desired pin*/
		Loc_u32Temp &= ~(GPIO_PUPD_Mask << (2 * Loc_u8PinNum));
		Loc_u32Temp |= (Loc_PUPD << (2 * Loc_u8PinNum));
		GPIO->GPIO_PUPDR = Loc_u32Temp;

		u16 Loc_Mode = Copy_strPin->mode ;
		Loc_Mode >>=  GPIO_Mode_Shift;

		 Loc_u32Temp = GPIO->GPIO_MODER;
		/*clear the mode bits for the desired pin*/
		Loc_u32Temp &= ~(GPIO_Mode_Mask << (2 * Loc_u8PinNum));
		Loc_u32Temp |= (Loc_Mode << (2 * Loc_u8PinNum));
		/* set the configuration input or GP output or AF or Analog*/
		GPIO->GPIO_MODER = Loc_u32Temp;

		/*set the output type PP or OD */
		Loc_u32Temp = GPIO->GPIO_OTYPER;
		u8 Loc_PP_OD = Copy_strPin->mode & 0x01;
		/* clear the output type for the Pin*/
		Loc_u32Temp &= ~(1 << Loc_u8PinNum);
		Loc_u32Temp |= (Loc_PP_OD << Loc_u8PinNum);
		GPIO->GPIO_OTYPER = Loc_u32Temp;

		Loc_u32Temp = GPIO->GPIO_OSPEEDR;
		/*clear the speed bits for the desired pin*/
		Loc_u32Temp &= ~(GPIO_Speed_Mask << (2 * Loc_u8PinNum));
		/*set the speed bits with the desired speed*/
		Loc_u32Temp |= (Copy_strPin->speed << (2 * Loc_u8PinNum));
		GPIO->GPIO_OSPEEDR = Loc_u32Temp;

	}
	return Loc_enuErrorStatus;

}

GPIO_enuErrorStatus_t gpio_writePinValue(void * Copy_vpPort, u32 Copy_enuPin,
		GPIO_enuValue_t Copy_enuValue) {
	GPIO_enuErrorStatus_t Loc_enuErrorStatus = GPIO_enuOk;
	if ((Copy_enuPin & GPIO_ValidPinMask) != GPIO_ValidPinMask) {
		Loc_enuErrorStatus = GPIO_enuInvalidPin;
	} else if (Copy_vpPort != GPIO_PORT_A && Copy_vpPort != GPIO_PORT_B
			&& Copy_vpPort != GPIO_PORT_C && Copy_vpPort != GPIO_PORT_D
			&& Copy_vpPort != GPIO_PORT_E && Copy_vpPort != GPIO_PORT_H) {
		Loc_enuErrorStatus = GPIO_enuInvalidPort;
	} else if (Copy_enuValue != GPIO_Value_Low
			&& Copy_enuValue != GPIO_Value_High) {
		Loc_enuErrorStatus = GPIO_enuInvalidValue;
	} else {
		volatile GPIO_strReg_t * const GPIO =
				(volatile GPIO_strReg_t * const ) Copy_vpPort;
		u32 Loc_u32Pin = Copy_enuPin;
		Loc_u32Pin &= ~GPIO_ValidPinMask;
		if (Copy_enuValue == GPIO_Value_High) {
			GPIO->GPIO_BSRR = Loc_u32Pin;
		} else {
			GPIO->GPIO_BSRR = (Loc_u32Pin << GPIO_Reset_Shift);
		}
	}

	return Loc_enuErrorStatus;
}

GPIO_enuErrorStatus_t gpio_writePortValue(void * Copy_vpPort, u16 Copy_u16Value) {
	GPIO_enuErrorStatus_t Loc_enuErrorStatus = GPIO_enuOk;
	if (Copy_vpPort != GPIO_PORT_A && Copy_vpPort != GPIO_PORT_B
			&& Copy_vpPort != GPIO_PORT_C && Copy_vpPort != GPIO_PORT_D
			&& Copy_vpPort != GPIO_PORT_E && Copy_vpPort != GPIO_PORT_H) {
		Loc_enuErrorStatus = GPIO_enuInvalidPort;

	} else {
		volatile GPIO_strReg_t * const GPIO =
				(volatile GPIO_strReg_t * const ) Copy_vpPort;
		GPIO->GPIO_ODR = Copy_u16Value;
		if (GPIO->GPIO_IDR != Copy_u16Value) {
			Loc_enuErrorStatus = GPIO_enuNotOk;
		}
	}

	return Loc_enuErrorStatus;
}

GPIO_enuErrorStatus_t gpio_getPinValue(void * Copy_vpPort, u32 Copy_enuPin,
		u8 *Add_pu8Value) {
	GPIO_enuErrorStatus_t Loc_enuErrorStatus = GPIO_enuOk;
	if ((Copy_enuPin & GPIO_ValidPinMask) != GPIO_ValidPinMask) {
		Loc_enuErrorStatus = GPIO_enuInvalidPin;
	} else if (Copy_vpPort != GPIO_PORT_A && Copy_vpPort != GPIO_PORT_B
			&& Copy_vpPort != GPIO_PORT_C && Copy_vpPort != GPIO_PORT_D
			&& Copy_vpPort != GPIO_PORT_E && Copy_vpPort != GPIO_PORT_H) {
		Loc_enuErrorStatus = GPIO_enuInvalidPort;
	} else {
		u8 Loc_u8PinNum = -1;
		u32 Loc_u32Pin = Copy_enuPin;
		Loc_u32Pin &= ~ GPIO_ValidPinMask;
		while (Loc_u32Pin) {
			Loc_u8PinNum++;
			Loc_u32Pin >>= 1;
		}
		volatile GPIO_strReg_t * const GPIO =
				(volatile GPIO_strReg_t * const ) Copy_vpPort;
		*Add_pu8Value = (GPIO->GPIO_IDR >> Loc_u8PinNum) & 0x01;
	}

	return Loc_enuErrorStatus;

}

GPIO_enuErrorStatus_t gpio_getPortValue(void* Copy_vpPort, u16* Add_pu16Value) {
	GPIO_enuErrorStatus_t Loc_enuErrorStatus = GPIO_enuOk;
	if (Copy_vpPort != GPIO_PORT_A && Copy_vpPort != GPIO_PORT_B
			&& Copy_vpPort != GPIO_PORT_C && Copy_vpPort != GPIO_PORT_D
			&& Copy_vpPort != GPIO_PORT_E && Copy_vpPort != GPIO_PORT_H) {
		Loc_enuErrorStatus = GPIO_enuInvalidPort;
	} else {
		volatile GPIO_strReg_t * const GPIO =
				(volatile GPIO_strReg_t * const ) Copy_vpPort;
		*Add_pu16Value = GPIO->GPIO_IDR;
	}

	return Loc_enuErrorStatus;
}

GPIO_enuErrorStatus_t gpio_selectAF(void* Copy_vpPort, u32 Copy_enuPin,
		GPIO_enuAF_t Copy_enuAFValue) {
	GPIO_enuErrorStatus_t Loc_enuErrorStatus = GPIO_enuOk;
	if (Copy_vpPort != GPIO_PORT_A && Copy_vpPort != GPIO_PORT_B
			&& Copy_vpPort != GPIO_PORT_C && Copy_vpPort != GPIO_PORT_D
			&& Copy_vpPort != GPIO_PORT_E && Copy_vpPort != GPIO_PORT_H) {
		Loc_enuErrorStatus = GPIO_enuInvalidPort;

	} else if ((Copy_enuPin & GPIO_ValidPinMask) != GPIO_ValidPinMask) {
		Loc_enuErrorStatus = GPIO_enuInvalidPin;
	} else if (Copy_enuAFValue > GPIO_AF_15 || Copy_enuAFValue < GPIO_AF_0) {
		Loc_enuErrorStatus = GPIO_enuInvalidAFValue;
	} else {
		u8 Loc_u8PinNum = -1;
		u32 Loc_u32Pin = Copy_enuPin;
		Loc_u32Pin &= ~ GPIO_ValidPinMask;
		while (Loc_u32Pin) {
			Loc_u8PinNum++;
			Loc_u32Pin >>= 1;
		}
		volatile GPIO_strReg_t * const GPIO =
				(volatile GPIO_strReg_t * const ) Copy_vpPort;
		if (Copy_enuPin < GPIO_PIN_8) {
			u32 Loc_u32Temp = GPIO->GPIO_AFRL;
			/*clear the AF bits for the desired pin*/
			Loc_u32Temp &= ~(GPIO_AF_Mask << (4 * Loc_u8PinNum));
			Loc_u32Temp |= (Copy_enuAFValue << (4 * Loc_u8PinNum));
			GPIO->GPIO_AFRL = Loc_u32Temp;
		} else {
			u32 Loc_u32Temp = GPIO->GPIO_AFRH;
			/*clear the AF bits for the desired pin*/
			Loc_u32Temp &= ~(GPIO_AF_Mask << (4 * Loc_u8PinNum));
			Loc_u32Temp |= (Copy_enuAFValue << (4 * Loc_u8PinNum));
			GPIO->GPIO_AFRH = Loc_u32Temp;
		}
	}
	return Loc_enuErrorStatus;
}
