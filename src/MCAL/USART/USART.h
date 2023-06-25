
#ifndef MCAL_USART_USART_H_
#define MCAL_USART_USART_H_


#define SYSCLK              (25000000)

/* configuration for USART  */
/* Oversampling */
#define USART_OVSAMPLING_8              0X8000
#define USART_OVSAMPLING_16             0X0
/* USART_Enable */
#define USART_ENABLE					0X2000
#define USART_DISABLE					0X0
/* WordLength */
#define USART_8_BIT						0X0
#define USART_9_BIT						0X1000
/* ParityControl */
#define USART_NO_PARITY   	            0X0
#define USART_EVEN_PARITY               0X400
#define USART_ODD_PARITY                0X600

#define USART_PEIE_ENABLE				0X100
#define USART_PEIE_DISABLE 				0X0

#define USART_TXEIE_ENABLE				0X80
#define USART_TXEIE_DISABLE				0X0

#define USART_TCIE_ENABLE				0X40
#define USART_TCIE_DISABLE				0X0

#define USART_RXNEIE_ENABLE				0X20
#define USART_RXNEIE_DISABLE			0X0

#define USART_IDLEIE_ENABLE				0X10
#define USART_IDLEIE_DISABLE			0X0

#define USART_TE_ENABLE					0X8
#define USART_TE_DISABLE				0X0

#define USART_RE_ENABLE					0X4
#define USART_RE_DISABLE				0X0

#define USART_1_STOP_BIT				0X0
#define USART_2_STOP_BITS				0X2000

typedef struct
{

	u32 Oversampling;
	u32 USART_Enable;
	u32 WordLength;
	u32 ParityControl;
	u32 PEIE_Int;
	u32 TXEIE_Int;  /* transmission Data register empty interrupt enable */
	u32 TXCIE_Int; /* transmission complete interrupt enable */
	u32 RXNEIE_Int; /* Receive Date Register Empty interrupt enable */
	u32 IDLEIE_Int;	/* detect IDLE line interrupt enable*/
	u32 TxEnable;  /*  transmission enable*/
	u32 RxEnable; /* receiver enable */
	u32 StopBits;
	u16 BaudRate;

}USART_cfg_t;

typedef enum
{
	USART_enuOk=0,
	USART_enuNotOk,
	USART_enuNullPtr,
	USART_enuBusy,
}USART_enuErrorStatus_t;

typedef enum
{
	USART1 = 0,
	USART2,
	USART6,
	USART_Numbers
}USART_Num_t;

typedef void(*USART_CallBack_t)(void);

USART_enuErrorStatus_t USART_vidInit(u8 USART_num, const USART_cfg_t* ConfigPtr);
USART_enuErrorStatus_t USART_enuSendBuffer(u8 USART_Num,u8 *buffer_in, u32 buffer_size);
USART_enuErrorStatus_t USART_enuRecieveBuffer(u8 USART_Num,u8 *buffer_in, u32 buffer_size);
USART_enuErrorStatus_t USART_enuRegisterCallBack_TXE(u8 USART_Num,USART_CallBack_t cbf);
USART_enuErrorStatus_t USART_enuRegisterCallBack_RX(u8 USART_Num,USART_CallBack_t cbf);

#endif /* MCAL_USART_USART_H_ */
