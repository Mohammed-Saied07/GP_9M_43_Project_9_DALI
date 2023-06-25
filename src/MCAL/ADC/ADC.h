#ifndef MCAL_ADC_ADC_H_
#define MCAL_ADC_ADC_H_

#include "../../LIB/STD_TYPES.h"

#define ENABLE  1
#define DISABLE 0

/*********************************Registers********************************/
#define ADC_BASE_ADDRESS 0x40012000

#define ADC_SR		(*(volatile u32*)(ADC_BASE_ADDRESS + 0x00))
#define EOC_BIT		1
#define JEOC_BIT	2

#define ADC_CR1		(*(volatile u32*)(ADC_BASE_ADDRESS + 0x04))
#define AWDCH_BIT 		0
#define EOCIE_BIT 		5	/* Enable Interrupt for regular Channels*/
#define JEOCIE_BIT 		7 	/* Enable Interrupt for injected Channels*/
#define JAWDEN_BIT 		22
#define AWDEN_BIT 		23
#define RES_BIT			24


#define ADC_CR2		(*(volatile u32*)(ADC_BASE_ADDRESS + 0x08))
#define ADON_BIT 		0
#define CONT_BIT 		1
#define DMA_BIT 		8
#define ALIGN_BIT 		11
#define JEXTSEL_BIT 	16
#define JEXTEN_BIT 		20
#define JSWSTART_BIT 	22
#define EXTSEL_BIT 		24
#define EXTEN_BIT 		28
#define SWSTART_BIT 	30

#define ADC_SMPR1	(*(volatile u32*)(ADC_BASE_ADDRESS + 0x0C))
#define ADC_SMPR2	(*(volatile u32*)(ADC_BASE_ADDRESS + 0x10))
#define ADC_HTR		(*(volatile u32*)(ADC_BASE_ADDRESS + 0x24))
#define ADC_LTR		(*(volatile u32*)(ADC_BASE_ADDRESS + 0x28))
#define ADC_SQR1	(*(volatile u32*)(ADC_BASE_ADDRESS + 0x2C))
#define ADC_SQR2	(*(volatile u32*)(ADC_BASE_ADDRESS + 0x30))
#define ADC_SQR3	(*(volatile u32*)(ADC_BASE_ADDRESS + 0x34))
#define ADC_JSQR	(*(volatile u32*)(ADC_BASE_ADDRESS + 0x38))
#define ADC_DR		(*(volatile u32*)(ADC_BASE_ADDRESS + 0x4C))
#define ADC_SQR3	(*(volatile u32*)(ADC_BASE_ADDRESS + 0x34))
#define ADC_CCR		(*(volatile u32*)(ADC_BASE_ADDRESS + 0x34))

#define ADC_JOFR_BASE		((volatile u32*)(ADC_BASE_ADDRESS + 0x14)) //Bits 11:0
#define ADC_JDR_BASE		((volatile u32*)(ADC_BASE_ADDRESS + 0x3C)) //JDATA[15:0]

#define ADCPRE_BIT 16
#define JL_BIT	20
#define L_BIT	20

/***************************************************************************/
typedef enum{
	ADC_enuOk = 0,
	ADC_enuNotOk,
}ADC_enuErrorStatus_t;

typedef enum {
	ADC_IN0=0,
	ADC_IN1,
	ADC_IN2,
	ADC_IN3,
	ADC_IN4,
	ADC_IN5,
	ADC_IN6,
	ADC_IN7,
	ADC_IN8,
	ADC_IN9,
	ADC_IN10,
	ADC_IN11,
	ADC_IN12,
	ADC_IN13,
	ADC_IN14,
	ADC_IN15,
	ADC_IN16,
	ADC_IN17,
	ADC_IN18
}ADC_enuChannel_t;

typedef enum {
	ADC_enuRegular = 0,
	ADC_enuninjected
}ADC_enuChannelsSelection_t;

typedef enum {
	ADC_enuSingleConversion = 0,
	ADC_enuContinuousConversion
}ADC_enuConversionMode_t;

typedef enum {
	ADC_enuDMADisable = 0,
	ADC_enuDMAEnable
}ADC_enuDMAMode_t;

typedef enum {
	ADC_enu3Cycles = 0,
	ADC_enu15Cycles,
	ADC_enu28Cycles,
	ADC_enu56Cycles,
	ADC_enu84Cycles,
	ADC_enu112Cycles,
	ADC_enu144Cycles,
	ADC_enu480Cycles
}ADC_enuSamplingTime_t;

typedef enum {
	ADC_enuRightAlignment = 0,
	ADC_enuLeftAlignment
}ADC_enuDataAlignment_t;

typedef enum {
	ADC_enuInjectedChannel0 =0,
	ADC_enuInjectedChannel1,
	ADC_enuInjectedChannel2,
	ADC_enuInjectedChannel3
}ADC_enuInjectedChannel_t;

typedef enum {
	ADC_enuInjected_1Conversion =0,
	ADC_enuInjected_2Conversions,
	ADC_enuInjected_3Conversions,
	ADC_enuInjected_4Conversions
}ADC_enuInjectedLength_t;

typedef enum {
	ADC_enuRegular_1Conversion =0,
	ADC_enuRegular_2Conversions,
	ADC_enuRegular_3Conversions,
	ADC_enuRegular_4Conversions,
	ADC_enuRegular_5Conversions,
	ADC_enuRegular_6Conversions,
	ADC_enuRegular_7Conversions,
	ADC_enuRegular_8Conversions,
	ADC_enuRegular_9Conversions,
	ADC_enuRegular_10Conversions,
	ADC_enuRegular_11Conversions,
	ADC_enuRegular_12Conversions,
	ADC_enuRegular_13Conversions,
	ADC_enuRegular_14Conversions,
	ADC_enuRegular_15Conversions,
	ADC_enuRegular_16Conversions
}ADC_enuRegularLength_t;

typedef enum {
	ADC_enuReqularSequence1 = 0,
	ADC_enuReqularSequence2,
	ADC_enuReqularSequence3,
	ADC_enuReqularSequence4,
	ADC_enuReqularSequence5,
	ADC_enuReqularSequence6,
	ADC_enuReqularSequence7,
	ADC_enuReqularSequence8,
	ADC_enuReqularSequence9,
	ADC_enuReqularSequence10,
	ADC_enuReqularSequence11,
	ADC_enuReqularSequence12,
	ADC_enuReqularSequence13,
	ADC_enuReqularSequence14,
	ADC_enuReqularSequence15,
	ADC_enuReqularSequence16
}ADC_enuReqularSequence_t;

typedef enum {
	ADC_enuInjectedSequence1 = 0,
	ADC_enuInjectedSequence2,
	ADC_enuInjectedSequence3,
	ADC_enuInjectedSequence4

}ADC_enuInjectedSequence_t;

typedef enum {
	ADC_enuRegularGroup =0,
	ADC_enuninjectedGroup
}ADC_enuChannelSelection_t;

typedef enum {
	 ADC_enuTriggerDisabled = 0,
	 ADC_enuRisingEdge,
	 ADC_enuFallingEdge,
	 ADC_enuRisingandFalling
}ADC_enuTriggerPolarity_t;

typedef enum {
	ADC_enu12bit = 0,
	ADC_enu10bit,
	ADC_enu8bit,
	ADC_enu6bit
}ADC_enuConversionResolution_t;

typedef enum {
	ADC_enuPrescalerDiv2 = 0,
	ADC_enuPrescalerDiv4,
	ADC_enuPrescalerDiv6,
	ADC_enuPrescalerDiv8
}ADC_enuPrescaler_t;

typedef enum {
	ADC_enuTimer1_CC1 = 0,
	ADC_enuTimer1_CC2,
	ADC_enuTimer1_CC3,
	ADC_enuTimer2_CC2,
	ADC_enuTimer2_CC3,
	ADC_enuTimer2_CC4,
	ADC_enuTimer2_TRGO,
	ADC_enuTimer3_CC1,
	ADC_enuTimer3_TRGO,
	ADC_enuTimer4_CC4,
	ADC_enuTimer5_CC1,
	ADC_enuTimer5_CC2,
	ADC_enuTimer5_CC3,
	ADC_enuEXTI_line11 = 15
}ADC_enuExternalEvent_t;


ADC_enuErrorStatus_t ADC_enuEnable();

ADC_enuErrorStatus_t ADC_enuDisable();

ADC_enuErrorStatus_t ADC_enuSetConversionMode(ADC_enuConversionMode_t cpy_enuConversionMode);

ADC_enuErrorStatus_t ADC_enuSetDMAMode(ADC_enuDMAMode_t cpy_enuDMAMode);

ADC_enuErrorStatus_t ADC_enuSetDataAlignment(ADC_enuDataAlignment_t cpy_enuDataAlignment);

ADC_enuErrorStatus_t ADC_enuSetSamplingTime(ADC_enuChannel_t cpy_enuChannel, ADC_enuSamplingTime_t cpy_enuSamplingTime);

ADC_enuErrorStatus_t ADC_enuSetDataOffsetInjectedChannel(ADC_enuInjectedChannel_t cpy_enuInjectedChannel, u16 cpy_u16Offset);

ADC_enuErrorStatus_t ADC_enuSetExternalEventforInjectedGroup(ADC_enuExternalEvent_t cpy_enuExternalEvent);

ADC_enuErrorStatus_t ADC_enuSetExternalEventforRegularGroup(ADC_enuExternalEvent_t cpy_enuExternalEvent);

ADC_enuErrorStatus_t ADC_enuSetRegularExternalTriggerPolarity(ADC_enuTriggerPolarity_t cpy_enuTriggerPolarity);

ADC_enuErrorStatus_t ADC_enuSetInjectedExternalTriggerPolarity(ADC_enuTriggerPolarity_t cpy_enuTriggerPolarity);

ADC_enuErrorStatus_t ADC_enuSetDataOffsetInjectedChannel(ADC_enuInjectedChannel_t cpy_enuInjectedChannel, u16 cpy_u16Offset);

ADC_enuErrorStatus_t ADC_enuSetWatchdogHigherThreshold(u16 cpy_u16WatchdogHigherThreshold);

ADC_enuErrorStatus_t ADC_enuSetWatchdogLowerThreshold(u16 cpy_u16WatchdogLowerThreshold);

ADC_enuErrorStatus_t ADC_enuInit(void);

ADC_enuErrorStatus_t ADC_enustartConversion(ADC_enuChannelsSelection_t cpy_enuChannelsSelection);

ADC_enuErrorStatus_t ADC_enuGetConvertedData(u32* ADCReading);

ADC_enuErrorStatus_t ADC_enuSetPrescaler (ADC_enuPrescaler_t cpy_enuPrescaler);

ADC_enuErrorStatus_t ADC_enuSetChannelNumberofInjectedSequence(ADC_enuInjectedSequence_t cpy_enuInjectedSequence, ADC_enuChannel_t cpy_enuChannel);

ADC_enuErrorStatus_t ADC_enuSetChannelNumberofRegularSequence(ADC_enuReqularSequence_t cpy_enuReqularSequence, ADC_enuChannel_t cpy_enuChannel);

ADC_enuErrorStatus_t ADC_enuSetRegularSequenceLength(ADC_enuRegularLength_t cpy_enuRegularLength);

ADC_enuErrorStatus_t ADC_enuSetInjectedSequenceLength(ADC_enuInjectedLength_t cpy_enuInjectedLength);

ADC_enuErrorStatus_t ADC_enuSetConversionResolution(ADC_enuConversionResolution_t cpy_enuConversionResolution);



#endif /* MCAL_ADC_ADC_H_ */
