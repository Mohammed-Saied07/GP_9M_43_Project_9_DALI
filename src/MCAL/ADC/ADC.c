#include "../../LIB/STD_TYPES.h"
#include "../RCC/RCC.h"
#include "ADC.h"

ADC_enuChannelsSelection_t Global_enuChannelsSelection;

ADC_enuErrorStatus_t ADC_enuInit()
{
	ADC_enuErrorStatus_t Loc_enuErrorStatus = ADC_enuOk;

	/* Enable ADC Clock*/
	RCC_enuEnablePeripheral(RCC_APB2,RCC_APB2_ADC1);

	/* Enable ADC */
	ADC_enuEnable();

	/* Set Prescaler */
	ADC_enuSetPrescaler(ADC_enuPrescalerDiv2);

	/* Set ADC Conversion Resolution */
	ADC_enuSetConversionResolution(ADC_enu12bit);

	/* Configure Data Alignment */
	ADC_enuSetDataAlignment(ADC_enuRightAlignment);

	/* Setting Conversion Resolution */
	ADC_enuSetConversionResolution(ADC_enu12bit);

	/* Enable Interrupt for Injected Channels*/
	ADC_CR1 |= (1 << JEOCIE_BIT);

	/* Enable Interrupt for regular Channels*/
	ADC_CR1 |= (1 << EOCIE_BIT);

	/* External Trigger Enable for injected Channels */
	ADC_enuSetInjectedExternalTriggerPolarity(ADC_enuRisingEdge);

	/* Select Timer 2 TRGO as External event for injected group*/
	ADC_enuSetExternalEventforInjectedGroup(ADC_enuTimer2_TRGO);

	/* Select Channel 5 Regular */
	ADC_enuSetRegularSequenceLength(ADC_enuRegular_1Conversion);
	ADC_enuSetChannelNumberofRegularSequence(ADC_enuReqularSequence1, ADC_IN0);

	/* Select Channel 5 Regular */
	ADC_enuSetInjectedSequenceLength(ADC_enuInjected_1Conversion);
	ADC_enuSetChannelNumberofRegularSequence(ADC_enuInjectedSequence4, ADC_IN13);

	/* Enable ADC interrupt (NVIC) In main*/

	return Loc_enuErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuEnable()
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;

	/* Enable ADC: Set ADON bit*/
	ADC_CR2 |= (1 << ADON_BIT);

	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuDisable()
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;

	/* Disable ADC: Clear ADON bit*/
	ADC_CR2 &= ~(1 << ADON_BIT);

	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetConversionMode(ADC_enuConversionMode_t cpy_enuConversionMode)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADC_CR2_Value = ADC_CR2;

	ADC_CR2_Value &= ~(0b1 <<CONT_BIT);
	ADC_CR2_Value |= (cpy_enuConversionMode << CONT_BIT);
	ADC_CR2 = ADC_CR2_Value;

	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetDMAMode(ADC_enuDMAMode_t cpy_enuDMAMode)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADC_CR2_Value = ADC_CR2;

	ADC_CR2_Value &= ~(0b1 << DMA_BIT);
	ADC_CR2_Value |= (cpy_enuDMAMode << DMA_BIT);
	ADC_CR2 = ADC_CR2_Value;

	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetDataAlignment(ADC_enuDataAlignment_t cpy_enuDataAlignment)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADC_CR2_Value = ADC_CR2;

	ADC_CR2_Value &= ~(0b1 <<ALIGN_BIT);
	ADC_CR2_Value |= (cpy_enuDataAlignment << ALIGN_BIT);
	ADC_CR2 = ADC_CR2_Value;

	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetExternalEventforInjectedGroup(ADC_enuExternalEvent_t cpy_enuExternalEvent)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADC_CR2_Value = ADC_CR2;

	ADC_CR2_Value &= ~(0b1111 <<JEXTSEL_BIT);
	ADC_CR2_Value |= (cpy_enuExternalEvent << JEXTSEL_BIT);
	ADC_CR2 = ADC_CR2_Value;

	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetInjectedExternalTriggerPolarity(ADC_enuTriggerPolarity_t cpy_enuTriggerPolarity)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADC_CR2_Value = ADC_CR2;

	ADC_CR2_Value &= ~(0b11 << JEXTEN_BIT);
	ADC_CR2_Value |= (cpy_enuTriggerPolarity << JEXTEN_BIT);
	ADC_CR2 = ADC_CR2_Value;

	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetExternalEventforRegularGroup(ADC_enuExternalEvent_t cpy_enuExternalEvent)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADC_CR2_Value = ADC_CR2;

	ADC_CR2_Value &= ~(0b1111 <<EXTSEL_BIT);
	ADC_CR2_Value |= (cpy_enuExternalEvent << EXTSEL_BIT);
	ADC_CR2 = ADC_CR2_Value;

	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetRegularExternalTriggerPolarity(ADC_enuTriggerPolarity_t cpy_enuTriggerPolarity)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADC_CR2_Value = ADC_CR2;

	ADC_CR2_Value &= ~(0b11 << EXTEN_BIT);
	ADC_CR2_Value |= (cpy_enuTriggerPolarity << EXTEN_BIT);
	ADC_CR2 = ADC_CR2_Value;

	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetAnalogWatchdogChannel(ADC_enuChannel_t cpy_enuChannel)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADC_CR1_Value = ADC_CR1;

	ADC_CR1_Value &= ~(0b11111 <<AWDCH_BIT);
	ADC_CR1_Value |= (cpy_enuChannel << AWDCH_BIT);
	ADC_CR1 = ADC_CR1_Value;

	/* Analog Watchdog Enable*/
	if(Global_enuChannelsSelection == ADC_enuRegular)
	{
		ADC_CR1 |= (1 << AWDEN_BIT);
	}
	else if(Global_enuChannelsSelection == ADC_enuninjected)
	{
		ADC_CR1 |= (1 << JAWDEN_BIT);
	}
	else
	{
		Loc_ErrorStatus = ADC_enuNotOk;
	}
	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetSamplingTime(ADC_enuChannel_t cpy_enuChannel, ADC_enuSamplingTime_t cpy_enuSamplingTime)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADC_SMPR_Value;
	if(cpy_enuChannel >= ADC_IN10)
	{
		ADC_SMPR_Value = ADC_SMPR1;
		ADC_SMPR_Value &= ~(0b111 << 3*(cpy_enuChannel - ADC_IN10));
		ADC_SMPR_Value |= (cpy_enuSamplingTime << 3*(cpy_enuChannel - ADC_IN10));
		ADC_SMPR1 = ADC_SMPR_Value;
	}
	else
	{
		ADC_SMPR_Value = ADC_SMPR2;
		ADC_SMPR_Value &= ~(0b111 << 3*cpy_enuChannel);
		ADC_SMPR_Value |= (cpy_enuSamplingTime << 3*cpy_enuChannel);
		ADC_SMPR2 = ADC_SMPR_Value;
	}

	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetDataOffsetInjectedChannel(ADC_enuInjectedChannel_t cpy_enuInjectedChannel, u16 cpy_u16Offset)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;

	ADC_JOFR_BASE[cpy_enuInjectedChannel] = cpy_u16Offset;

	return Loc_ErrorStatus;
}
ADC_enuErrorStatus_t ADC_enuSetWatchdogHigherThreshold(u16 cpy_u16WatchdogHigherThreshold)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	ADC_HTR = cpy_u16WatchdogHigherThreshold;

	return Loc_ErrorStatus;

}


ADC_enuErrorStatus_t ADC_enuSetWatchdogLowerThreshold(u16 cpy_u16WatchdogLowerThreshold)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	ADC_LTR = cpy_u16WatchdogLowerThreshold;

	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetRegularSequenceLength(ADC_enuRegularLength_t cpy_enuRegularLength)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADC_SQR1_Value = ADC_SQR1;
	if(cpy_enuRegularLength > ADC_enuRegular_15Conversions)
	{
		Loc_ErrorStatus = ADC_enuNotOk;
	}
	else
	{
		ADC_SQR1_Value &= ~(0b1111 << L_BIT);
		ADC_SQR1_Value |= (cpy_enuRegularLength << L_BIT);
		ADC_SQR1 = ADC_SQR1_Value;
	}
	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetChannelNumberofRegularSequence(ADC_enuReqularSequence_t cpy_enuReqularSequence, ADC_enuChannel_t cpy_enuChannel)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADC_SQR_Value;
	if(cpy_enuReqularSequence > ADC_enuReqularSequence13)
	{
		ADC_SQR_Value = ADC_SQR1;
		ADC_SQR_Value &= ~(0b11111 << 5*(cpy_enuReqularSequence-ADC_enuReqularSequence13));
		ADC_SQR_Value |= (cpy_enuChannel << 5*(cpy_enuReqularSequence-ADC_enuReqularSequence13));
		ADC_SQR1 = ADC_SQR_Value;
	}
	else if(cpy_enuReqularSequence > ADC_enuReqularSequence5)
	{
		ADC_SQR_Value = ADC_SQR2;
		ADC_SQR_Value &= ~(0b11111 << 5*(cpy_enuReqularSequence-ADC_enuReqularSequence7));
		ADC_SQR_Value |= (cpy_enuChannel << 5*(cpy_enuReqularSequence-ADC_enuReqularSequence7));
		ADC_SQR2 = ADC_SQR_Value;
	}
	else
	{
		ADC_SQR_Value = ADC_SQR3;
		ADC_SQR_Value &= ~(0b11111 << 5*cpy_enuReqularSequence);
		ADC_SQR_Value |= (cpy_enuChannel << 5*cpy_enuReqularSequence);
		ADC_SQR3 = ADC_SQR_Value;
	}
	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetChannelNumberofInjectedSequence(ADC_enuInjectedSequence_t cpy_enuInjectedSequence, ADC_enuChannel_t cpy_enuChannel)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;

	u32 ADC_JSQR_Value = ADC_JSQR;
	ADC_JSQR_Value &= ~(0b11111 << 5*cpy_enuInjectedSequence);
	ADC_JSQR_Value |= (cpy_enuChannel << 5*cpy_enuInjectedSequence);
	ADC_JSQR = ADC_JSQR_Value;

	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetInjectedSequenceLength(ADC_enuInjectedLength_t cpy_enuInjectedLength)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADC_JSQR_Value = ADC_JSQR;
	if(cpy_enuInjectedLength > ADC_enuInjected_4Conversions)
	{
		Loc_ErrorStatus = ADC_enuNotOk;
	}
	else
	{
		ADC_JSQR_Value &= ~(0b11 << JL_BIT);
		ADC_JSQR_Value |= (cpy_enuInjectedLength << JL_BIT);
		ADC_JSQR = ADC_JSQR_Value;
	}
	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetPrescaler(ADC_enuPrescaler_t cpy_enuPrescaler)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
	u32 ADCCCR_Value = ADC_CCR;
	if(cpy_enuPrescaler > ADC_enuPrescalerDiv8)
	{
		Loc_ErrorStatus = ADC_enuNotOk;
	}
	else
	{
		ADCCCR_Value &= ~(0b11 << ADCPRE_BIT);
		ADCCCR_Value |= (cpy_enuPrescaler << ADCPRE_BIT);
		ADC_CCR = ADCCCR_Value;
	}
	return Loc_ErrorStatus;
}

ADC_enuErrorStatus_t ADC_enustartConversion(ADC_enuChannelsSelection_t cpy_enuChannelsSelection)
{
	ADC_enuErrorStatus_t Loc_enuErrorStatus = ADC_enuOk;

	Global_enuChannelsSelection = cpy_enuChannelsSelection;
	/*Conversion starts when either the SWSTART or the JSWSTART bit is set*/
	if(cpy_enuChannelsSelection == ADC_enuRegular)
	{
		ADC_CR2 |= (1 << SWSTART_BIT);
	}
	else if(cpy_enuChannelsSelection == ADC_enuninjected)
	{
		ADC_CR2 |= (1 << JSWSTART_BIT);
	}
	else
	{
		Loc_enuErrorStatus = ADC_enuNotOk;
	}
	/* Enable End of Conversion Interrupt */
	ADC_CR1 |= (1 << EOCIE_BIT);

	return Loc_enuErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuGetConvertedData(u32* ADCReading)
{
	ADC_enuErrorStatus_t Loc_enuErrorStatus = ADC_enuOk;

	/* Check if Regular conversion complete */
	if((ADC_SR = (ADC_SR >> EOC_BIT) & 1)== 1)
	{
		/* Read analog Data*/
		*ADCReading = ADC_DR;

		/* Start Conversion*/
		ADC_CR2 |= (1 << SWSTART_BIT);
	}
	else{
		/* Regular conversion is not done */
	}
	/* Check if Injected conversion complete */
	if((ADC_SR = (ADC_SR >> JEOC_BIT) & 1)== 1)
	{
		/* Read analog Data*/
		*ADCReading = ADC_JDR_BASE[0];

		/* Clear JEOC Flag */
		ADC_CR2 &= ~(1 << JEOC_BIT);

		/* Start Conversion*/
		ADC_CR2 |= (1 << JSWSTART_BIT);
	}
	else
	{
		/* Injected conversion is not done */
	}
	return Loc_enuErrorStatus;
}

ADC_enuErrorStatus_t ADC_enuSetConversionResolution(ADC_enuConversionResolution_t cpy_enuConversionResolution)
{
	ADC_enuErrorStatus_t Loc_ErrorStatus = ADC_enuOk;
		u32 ADC_CR1_Value = ADC_CR1;

		ADC_CR1_Value &= ~(0b11 << RES_BIT);
		ADC_CR1_Value |= (cpy_enuConversionResolution << RES_BIT);
		ADC_CR1 = ADC_CR1_Value;

		return Loc_ErrorStatus;
}





