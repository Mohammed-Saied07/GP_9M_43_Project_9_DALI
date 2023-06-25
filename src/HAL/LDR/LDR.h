#ifndef HAL_LDR_LDR_H_
#define HAL_LDR_LDR_H_


#define LDR_compare_value	(0xFFFFFFFF/2)

typedef enum
{
	LDR_OK,
	LDR_NOK
}LDR_enuErorrStatus_t;

typedef enum{
	LDR_Visible,
	LDR_Dark
}LDR_enuVisibleStatus_t;

LDR_enuErorrStatus_t  LDR_Init(void);

LDR_enuErorrStatus_t  LDR_ReadStatus(ADC_enuChannel_t Copy_enuChannel , LDR_enuVisibleStatus_t * Add_status);




#endif /* HAL_LDR_LDR_H_ */
