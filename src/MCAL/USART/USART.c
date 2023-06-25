#include "../../LIB/STD_TYPES.h"
#include "../RCC/RCC.h"
#include "USART.h"
/* flags to check in the usart handler */
#define CTS_flag	0x200
#define LBD_flag	0x100
#define TXE_flag	0x080
#define TC_flag		0x040
#define RXNE_flag	0x020
#define IDLE_flag	0x010
#define ORE_flag	0x008
#define NF_flag		0x004
#define FE_flag		0x002
#define PE_flag		0x001

typedef enum
{
	USART_Idle = 0,
	USART_Busy
}USART_status_t;

typedef struct {

	u32 USART_SR;
	u32 USART_DR;
	u32 USART_BRR;
	u32 USART_CR1;
	u32 USART_CR2;
	u32 USART_CR3;
	u32 USART_GTPR;
} USART_strReg_t;

/*					USART Peripherals available  			*/
#define USART1_BaseAdd                    ((USART_strReg_t *) (0x40011000))
#define USART2_BaseAdd                    ((USART_strReg_t *) (0x40004400))
#define USART6_BaseAdd                    ((USART_strReg_t *) (0x40011400))

volatile USART_strReg_t *  USART[USART_Numbers] = {USART1_BaseAdd,USART2_BaseAdd,USART6_BaseAdd};

USART_status_t USART_TX_status[USART_Numbers]={USART_Idle,USART_Idle,USART_Idle};
USART_status_t USART_RX_status[USART_Numbers]={USART_Idle,USART_Idle,USART_Idle};
 /*	Pointer to Transmission Done Function */
USART_CallBack_t TxC_CALL_Back[USART_Numbers] ={NULL,NULL,NULL};

/*	Pointer to Receive Done Function */
USART_CallBack_t Rx_CALL_Back[USART_Numbers] ={NULL,NULL,NULL};

/*	USART Data Buffer (Transmit) */
u8 *Buffer_ptr_TR[USART_Numbers]={NULL,NULL,NULL};
u32 Buffer_size_TR[USART_Numbers] = {0,0,0};
u32 Buffer_idx_TR[USART_Numbers] = {0,0,0};


/*	USART Data Buffer (Receive) */
u8 *Buffer_ptr_RE[USART_Numbers]={NULL,NULL,NULL};
u32 Buffer_size_RE[USART_Numbers] = {0,0,0};
u32 Buffer_idx_RE[USART_Numbers] = {0,0,0};


USART_enuErrorStatus_t USART_vidInit(u8 USART_num, const USART_cfg_t* ConfigPtr){
	USART_enuErrorStatus_t Loc_Error = USART_enuOk;
	if (USART_num>= USART_Numbers){
		Loc_Error = USART_enuNotOk;
	}
	else if (ConfigPtr == NULL){
		Loc_Error = USART_enuNullPtr;
	}
	else {
		switch(USART_num)
			{
				case USART1:
					RCC_enuEnablePeripheral(RCC_APB2,RCC_APB2_USART1);
					break;
				case USART2:
					RCC_enuEnablePeripheral(RCC_APB1,RCC_APB1_USART2);
					break;
				case USART6:
					RCC_enuEnablePeripheral(RCC_APB2,RCC_APB2_USART6);
					break;

			}

		u32 Loc_confg = ConfigPtr->Oversampling | ConfigPtr->USART_Enable | ConfigPtr->WordLength
				| ConfigPtr->ParityControl | ConfigPtr->PEIE_Int | ConfigPtr->TXEIE_Int | ConfigPtr->TXCIE_Int
				| ConfigPtr->RXNEIE_Int | ConfigPtr->IDLEIE_Int |ConfigPtr->TxEnable |ConfigPtr->RxEnable;
		USART[USART_num]->USART_CR1 = Loc_confg;
		Loc_confg =0;
		if(ConfigPtr->Oversampling == USART_OVSAMPLING_8)
			{
			Loc_confg = (SYSCLK)/(8*ConfigPtr->BaudRate);
			}
			else if(ConfigPtr->Oversampling == USART_OVSAMPLING_16)
			{
				Loc_confg = (SYSCLK)/(16*ConfigPtr->BaudRate);
			}
		USART[USART_num]->USART_BRR = Loc_confg << 4;
		Loc_confg = USART[USART_num]->USART_CR2;
		Loc_confg |= ConfigPtr->StopBits;
		USART[USART_num]->USART_CR2 = Loc_confg;


	}
	return Loc_Error;
}

USART_enuErrorStatus_t USART_enuSendBuffer(u8 USART_Num,u8 *buffer_in, u32 buffer_size){
	USART_enuErrorStatus_t Loc_error = USART_enuOk;
	if (USART_Num>= USART_Numbers){
			Loc_error = USART_enuNotOk;
		}
		else if (buffer_in == NULL){
			Loc_error = USART_enuNullPtr;
		}
		else if (USART_TX_status[USART_Num]==USART_Busy){
			Loc_error = USART_enuBusy;
		}
		else {
			USART_TX_status[USART_Num]=USART_Busy;
			Buffer_ptr_TR[USART_Num]=buffer_in;
			Buffer_size_TR[USART_Num] = buffer_size;
			Buffer_idx_TR[USART_Num] = 0;
			USART[USART1]->USART_CR1 |= USART_TCIE_ENABLE;
			USART[USART_Num]->USART_DR = Buffer_ptr_TR[USART_Num][Buffer_idx_TR[USART_Num]];
			Buffer_idx_TR[USART_Num] ++;
		}
	return Loc_error;
}

USART_enuErrorStatus_t USART_enuRecieveBuffer(u8 USART_Num,u8 *buffer_in, u32 buffer_size){
	USART_enuErrorStatus_t Loc_error = USART_enuOk;
		if (USART_Num>= USART_Numbers){
				Loc_error = USART_enuNotOk;
			}
			else if (buffer_in == NULL){
				Loc_error = USART_enuNullPtr;
			}
			else if (USART_RX_status[USART_Num]==USART_Busy){
				Loc_error = USART_enuBusy;
			}
			else {
				USART_RX_status[USART_Num]=USART_Busy;
				Buffer_ptr_RE[USART_Num]=buffer_in;
				Buffer_size_RE[USART_Num] = buffer_size;
				Buffer_idx_RE[USART_Num] = 0;
				USART[USART1]->USART_CR1 |= USART_RXNEIE_ENABLE;

			}
		return Loc_error;
}


USART_enuErrorStatus_t USART_enuRegisterCallBack_TXE(u8 USART_Num,USART_CallBack_t cbf){
	USART_enuErrorStatus_t Loc_error = USART_enuOk;
			if (USART_Num>= USART_Numbers){
					Loc_error = USART_enuNotOk;
				}
				else if (cbf == NULL){
					Loc_error = USART_enuNullPtr;
				}
				else {
					TxC_CALL_Back[USART_Num]=cbf;

				}
			return Loc_error;
}
USART_enuErrorStatus_t USART_enuRegisterCallBack_RX(u8 USART_Num,USART_CallBack_t cbf){
	USART_enuErrorStatus_t Loc_error = USART_enuOk;
				if (USART_Num>= USART_Numbers){
						Loc_error = USART_enuNotOk;
					}
					else if (cbf == NULL){
						Loc_error = USART_enuNullPtr;
					}
					else {
						Rx_CALL_Back[USART_Num]=cbf;

					}
				return Loc_error;
}


void USART1_IRQHandler(void)
{
	// Check Transmition register empty , without DMA
	if(((USART[USART1]->USART_SR) & TC_flag	) && (USART_TX_status[USART1] == USART_Busy))
	{
		/*Check Buffer size */
		if(Buffer_idx_TR[USART1] < Buffer_size_TR[USART1])
		{
			trace_printf("Byte %d Sent\n",Buffer_idx_TR[USART1]-1);
			USART[USART1]->USART_DR = Buffer_ptr_TR[USART1][Buffer_idx_TR[USART1]];
			Buffer_idx_TR[USART1]++;
		}
		else
		{
			trace_printf("Byte %d Sent\n",Buffer_idx_TR[USART1]-1);
			trace_printf("Transmit Complete , Calling Callback Function ...... \n");
			/*	Transmition  Done!	*/
			USART_TX_status[USART1] = USART_Idle;
			// Disable Interrupt
			USART[USART1]->USART_CR1 &= ~USART_TCIE_ENABLE;
			TxC_CALL_Back[USART1]();
		}
	}

	if(((USART[USART1]->USART_SR) & RXNE_flag) &&
			(USART_RX_status[USART1] == USART_Busy))
	{
		Buffer_ptr_RE[USART1][Buffer_idx_RE[USART1]] = USART[USART1]->USART_DR;
		Buffer_idx_RE[USART1]++;

		if(Buffer_idx_RE[USART1] == Buffer_size_RE[USART1])
		{
			/*	Receive  Done!	*/
			trace_printf("Receive Complete  \n");
			USART_RX_status[USART1] = USART_Idle;
			// Disable Interrupt
			USART[USART1]->USART_CR1 &= ~USART_RXNEIE_ENABLE;
			if(Rx_CALL_Back!=NULL)
				Rx_CALL_Back[USART1]();
		}
	}
}


void USART2_IRQHandler(void)
{
	// Check Transmition register empty , without DMA
	if(((USART[USART2]->USART_SR) & TC_flag	) && (USART_TX_status[USART2] == USART_Busy))
	{
		/*Check Buffer size */
		if(Buffer_idx_TR[USART2] < Buffer_size_TR[USART2])
		{
			trace_printf("Byte %d Sent\n",Buffer_idx_TR[USART2]-1);
			USART[USART2]->USART_DR = Buffer_ptr_TR[USART2][Buffer_idx_TR[USART2]];
			Buffer_idx_TR[USART2]++;
		}
		else
		{
			trace_printf("Byte %d Sent\n",Buffer_idx_TR[USART2]-1);
			trace_printf("Transmit Complete , Calling Callback Function ...... \n");
			/*	Transmition  Done!	*/
			USART_TX_status[USART2] = USART_Idle;
			// Disable Interrupt
			USART[USART2]->USART_CR1 &= ~USART_TCIE_ENABLE;
			TxC_CALL_Back[USART2]();
		}
	}

	if(((USART[USART2]->USART_SR) & RXNE_flag) &&
			(USART_RX_status[USART2] = USART_Busy))
	{
		Buffer_ptr_RE[USART2][Buffer_idx_RE[USART2]] = USART[USART2]->USART_DR;
		Buffer_idx_RE[USART2]++;

		if(Buffer_idx_RE[USART2] == Buffer_size_RE[USART2])
		{
			/*	Receive  Done!	*/
			USART_RX_status[USART2] = USART_Idle;
			// Disable Interrupt
			USART[USART2]->USART_CR1 &= ~USART_RXNEIE_ENABLE;
			Rx_CALL_Back[USART2]();
		}
	}
}

void USART6_IRQHandler(void)
{
	// Check Transmition register empty , without DMA
	if(((USART[USART6]->USART_SR) & TC_flag	) && (USART_TX_status[USART6] == USART_Busy))
	{
		/*Check Buffer size */
		if(Buffer_idx_TR[USART6] < Buffer_size_TR[USART6])
		{
			trace_printf("Byte %d Sent\n",Buffer_idx_TR[USART6]-1);
			USART[USART6]->USART_DR = Buffer_ptr_TR[USART6][Buffer_idx_TR[USART6]];
			Buffer_idx_TR[USART6]++;
		}
		else
		{
			trace_printf("Byte %d Sent\n",Buffer_idx_TR[USART6]-1);
			trace_printf("Transmit Complete , Calling Callback Function ...... \n");
			/*	Transmition  Done!	*/
			USART_TX_status[USART6] = USART_Idle;
			// Disable Interrupt
			USART[USART6]->USART_CR1 &= ~USART_TCIE_ENABLE;
			TxC_CALL_Back[USART6]();
		}
	}

	if(((USART[USART6]->USART_SR) & RXNE_flag) &&
			(USART_RX_status[USART6] = USART_Busy))
	{
		Buffer_ptr_RE[USART6][Buffer_idx_RE[USART6]] = USART[USART6]->USART_DR;
		Buffer_idx_RE[USART6]++;

		if(Buffer_idx_RE[USART6] == Buffer_size_RE[USART6])
		{
			/*	Receive  Done!	*/
			USART_RX_status[USART6] = USART_Idle;
			// Disable Interrupt
			USART[USART6]->USART_CR1 &= ~USART_RXNEIE_ENABLE;
			Rx_CALL_Back[USART6]();
		}
	}
}
