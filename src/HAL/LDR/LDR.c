#include "../../MCAL/ADC/ADC.h"
#include "LDR.h"


 LDR_enuErorrStatus_t  LDR_Init(void)
{
	 LDR_enuErorrStatus_t LocalErorrStatus = LDR_OK;
	 ADC_enuErrorStatus_t LocalADC_ErorrStatus;
	 LocalADC_ErorrStatus = ADC_enuInit();
	 if(LocalADC_ErorrStatus == ADC_enuNotOk)
	 {
		 LocalErorrStatus = LDR_NOK;
	 }
	 return LocalErorrStatus;

}

LDR_enuErorrStatus_t  LDR_ReadStatus(ADC_enuChannel_t Copy_enuChannel , LDR_enuVisibleStatus_t * Add_status)
{
	LDR_enuErorrStatus_t LocalErorrStatus = LDR_OK;
	u32 Local_ADCReturnValue;
	ADC_enuErrorStatus_t LocalADC_ErorrStatus;
	LocalADC_ErorrStatus = ADC_enustartConversion(Copy_enuChannel);
	if(LocalADC_ErorrStatus == ADC_enuNotOk)
	{
		LocalErorrStatus = LDR_NOK;
	}
	else
	{
		LocalADC_ErorrStatus = ADC_enuGetConvertedData(& Local_ADCReturnValue);
		if(LocalADC_ErorrStatus == ADC_enuNotOk)
		{
			LocalErorrStatus = LDR_NOK;
		}
		else
		{
			if(Local_ADCReturnValue < LDR_compare_value)
			{
				*Add_status = LDR_Dark;
			}
			else
			{
				*Add_status = LDR_Visible;
			}
		}
	}
	return LocalErorrStatus;
}




