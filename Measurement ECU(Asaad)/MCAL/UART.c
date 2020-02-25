/*- INCLUDES ----------------------------------------------*/

#include"UART_PreLConfig.h"
#include"UART.h"
#include"String_Manipulation.h"
#include"Atmega32Registers.h"
#include"BitManipulation.h"

/*- LOCAL MACROS ------------------------------------------*/
#define  UART_RXC_Enable_INT()          SET_BIT(UART_CNTRL_STATUS_REG_B,UART_RX_Complete_Interrupt_Enable) 
#define  UART_TXC_Enable_INT()          SET_BIT(UART_CNTRL_STATUS_REG_B,UART_TX_Complete_Interrupt_Enable)
#define  UART_UDRE_Enable_INT()         SET_BIT(UART_CNTRL_STATUS_REG_B,UART_Data_REG_Empty_Interrupt_Enable)
#define  UART_UDRE_Disable_INT()        CLR_BIT(UART_CNTRL_STATUS_REG_B,UART_Data_REG_Empty_Interrupt_Enable)
/*- LOCAL Dataypes ----------------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/

void __vector_13(void) __attribute((signal,used));
void __vector_14(void) __attribute((signal,used));
void __vector_15(void) __attribute((signal,used));

/*- GLOBAL STATIC VARIABLES -------------------------------*/

static volatile uint8_t  sgu8_UART_REC_BYTE=0;
static volatile PntrToFunc_t gPntrToFunc_UART_TX_ISR=NullPointer;
static volatile PntrToFunc_t gPntrToFunc_UART_RX_ISR=NullPointer;
static volatile PntrToFunc_t gPntrToFunc_UART_UDRE_ISR=NullPointer;
static uint8_t sgu8_UART_Execution_Mode=0;
static uint8_t sgu8_UART_Receiver_enable=0;
static uint8_t sgu8_UART_Transimter_enable=0;
static volatile uint8_t sgu8_Uart_TX_Interrupt_flag=0U;
static volatile uint8_t sgu8_Uart_UDRE_Interrupt_flag=0U;

/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/

void __vector_13(void)
{
	
	if(gPntrToFunc_UART_RX_ISR==NullPointer)
	{ 		

	}
	else
	{
		gPntrToFunc_UART_RX_ISR();
	}


}
void __vector_14(void)
{	
	UART_TXC_Enable_INT();
	UART_UDRE_Disable_INT();
	sgu8_Uart_UDRE_Interrupt_flag=1U;
}
void __vector_15(void)
{
	if(gPntrToFunc_UART_TX_ISR!=NullPointer )
	{ 	
		gPntrToFunc_UART_TX_ISR();	
        		
    } 
    else
	{
		

	}		
}

/*- APIs IMPLEMENTATION -----------------------------------*/

/*_______________________________________________________________________________________________________________________________*/
/*Description: It initiates the UART from pointer to configuration of UART
 * Input     : (char_t* pchar_index)It takes pointer to char
 * Output    : Error Checking
 *_______________________________________________________________________________________________________________________________*/

UART_Error_t UART_Init(const UART_Confg_Stuct_t* pstr_Config_UART)
{

    if(pstr_Config_UART!=NullPointer)
	{
		/**Set baud rate**/
		UART_BAUD_RATE_REG_HIGH = (uint8_t)((pstr_Config_UART->BaudRate)>>8);
		UART_BAUD_RATE_REG_LOW =  (uint8_t) (pstr_Config_UART->BaudRate);
		/**Enable receiver and transmitter**/
		if(pstr_Config_UART->Trasmit==UART_trasmit_Enable)
		{
		    sgu8_UART_Transimter_enable=UART_trasmit_Enable;
			SET_BIT(UART_CNTRL_STATUS_REG_B,UART_transimter_EN_BIT);
			
		}
		else
		{
		    sgu8_UART_Transimter_enable=UART_trasmit_Disable;
		}
		if(pstr_Config_UART->Reciever==UART_Receive_Enable)
		{
			

			sgu8_UART_Receiver_enable=UART_Receive_Enable;
			SET_BIT(UART_CNTRL_STATUS_REG_B,UART_Reciever_EN_BIT);
			
		}	
		else
		{
			sgu8_UART_Receiver_enable=UART_Receive_Disable;
		}		
		/*** Check for parity ***/
		if(pstr_Config_UART->Parity==UART_Parity_Disabled)
		{
			UART_CNTRL_STATUS_REG_C|=(1<<UART_CHOOSE_UART_CNTRL_STATUS_REG_C_BIT)|(0<<UART_Parity_Bit_1)|(0<<UART_Parity_Bit_0);
		}
		else if(pstr_Config_UART->Parity==UART_Parity_Even)
		{
			UART_CNTRL_STATUS_REG_C|=(1<<UART_CHOOSE_UART_CNTRL_STATUS_REG_C_BIT)|(1<<UART_Parity_Bit_1)|(0<<UART_Parity_Bit_0);			
		}
		else if(pstr_Config_UART->Parity==UART_Parity_Odd)
		{
			UART_CNTRL_STATUS_REG_C|=(1<<UART_CHOOSE_UART_CNTRL_STATUS_REG_C_BIT)|(1<<UART_Parity_Bit_1)|(1<<UART_Parity_Bit_0);				
		}
		else
		{
		}
		/*** Set frame format: Stop bit***/
		if(pstr_Config_UART->Parity==UART_two_Stop_bit)
		{
			UART_CNTRL_STATUS_REG_C|=(1<<UART_CHOOSE_UART_CNTRL_STATUS_REG_C_BIT)|(1<<UART_STOP_setting_Bit);				
		}
		else if(pstr_Config_UART->Parity==UART_One_Stop_bit)
		{
			UART_CNTRL_STATUS_REG_C|=(1<<UART_CHOOSE_UART_CNTRL_STATUS_REG_C_BIT)|(0<<UART_STOP_setting_Bit);
		}
		else
		{
		}
		/* Set frame format:Data_size */
		if(pstr_Config_UART->Data_size==UART_Frame_5_bit_)
		{
			UART_CNTRL_STATUS_REG_C|=(1<<UART_CHOOSE_UART_CNTRL_STATUS_REG_C_BIT)|(0<<UART_Bit_Data_Size_bit_2)|(0<<UART_Bit_Data_Size_bit_1)|(0<<UART_Bit_Data_Size_bit_0);
		}
		else if(pstr_Config_UART->Data_size==UART_Frame_6_bit_)
		{
			UART_CNTRL_STATUS_REG_C|=(1<<UART_CHOOSE_UART_CNTRL_STATUS_REG_C_BIT)|(0<<UART_Bit_Data_Size_bit_2)|(0<<UART_Bit_Data_Size_bit_1)|(1<<UART_Bit_Data_Size_bit_0);
		}
		else if(pstr_Config_UART->Data_size==UART_Frame_7_bit_)
		{
			UART_CNTRL_STATUS_REG_C|=(1<<UART_CHOOSE_UART_CNTRL_STATUS_REG_C_BIT)|(0<<UART_Bit_Data_Size_bit_2)|(1<<UART_Bit_Data_Size_bit_1)|(0<<UART_Bit_Data_Size_bit_0);
		}			
		else if(pstr_Config_UART->Data_size==UART_Frame_8_bit_)
		{
			UART_CNTRL_STATUS_REG_C|=(1<<UART_CHOOSE_UART_CNTRL_STATUS_REG_C_BIT)|(0<<UART_Bit_Data_Size_bit_2)|(1<<UART_Bit_Data_Size_bit_1)|(1<<UART_Bit_Data_Size_bit_0);			
		}	
		else if(pstr_Config_UART->Data_size==UART_Frame_9_bit_)
		{
			UART_CNTRL_STATUS_REG_C|=(1<<UART_CHOOSE_UART_CNTRL_STATUS_REG_C_BIT)|(1<<UART_Bit_Data_Size_bit_2)|(1<<UART_Bit_Data_Size_bit_1)|(1<<UART_Bit_Data_Size_bit_0);					
		}	
	    else
		{
		}
		/**Check whether it is interrupt or polling mode**/
		if(pstr_Config_UART->InterruptMode==UART_Interrupt_mode_enable)
		{
			sgu8_UART_Execution_Mode=UART_Interrupt_mode_enable;
			/*Enable Global Interrupt*/
			EnableGeneralInterrupt();
			/*Enable Receiver Interrupt*/
		    if(pstr_Config_UART->Reciever==UART_Receive_Enable)
			{
				
		    	UART_RXC_Enable_INT();

			}
			else
			{
			}
			/*Enable Transimter Interrupt*/
		    if(pstr_Config_UART->Trasmit==UART_trasmit_Enable)
			{	
/* 				UART_TXC_Enable_INT();		 */		
			}
			else
			{
			}
		}
		else
		{
			sgu8_UART_Execution_Mode=UART_Polling_mode_enable;			
		}
	}
	else 
	{
		return UART_ERROR_NULL_POINTER;
	}

}
/*_______________________________________________________________________________________________________________________________*/
/*Description: Set call back funtion for transimter
 * Input     : PntrToFunc_t PntrToFunc_Copy_UART_TX_USER_ISR->copy of pointer to function
 * Output    : Uart Error Checking
 *_______________________________________________________________________________________________________________________________*/
UART_Error_t UART_Trans_SetCallBack(PntrToFunc_t PntrToFunc_Copy_UART_TX_USER_ISR)
{
  if(UART_Interrupt_mode_enable==sgu8_UART_Execution_Mode)
  {

	if(NullPointer!=PntrToFunc_Copy_UART_TX_USER_ISR)
	{
		gPntrToFunc_UART_TX_ISR=PntrToFunc_Copy_UART_TX_USER_ISR;	
        UART_UDRE_Enable_INT();
		
	}
	else
	{
	}

  }
  else
  {
  }

}
/*_______________________________________________________________________________________________________________________________*/
/*Description: It transmites character through UART
 * Input     : (char_t pchar_index)It takes  char
 * Output    : Error Checking
 *_______________________________________________________________________________________________________________________________*/
UART_Error_t UART_SendByte(uint8_t u8_Byte_UART)
{
    if(sgu8_UART_Transimter_enable==UART_trasmit_Enable)
	{

		if(sgu8_UART_Execution_Mode==UART_Polling_mode_enable)
		{
			while ( !( UART_CNTRL_STATUS_REG_A & (1<<UART_DATA_REGISTER_EMPETY_FLAG)) );
            UART_DATA_REGISTER=u8_Byte_UART;
		}
		else if(sgu8_UART_Execution_Mode==UART_Interrupt_mode_enable)
		{
			if(1U==sgu8_Uart_UDRE_Interrupt_flag&&( UART_CNTRL_STATUS_REG_A & (1<<UART_DATA_REGISTER_EMPETY_FLAG)) ) UART_DATA_REGISTER=u8_Byte_UART;
		}
		else
		{


		}
		
	}
	else
	{
	}
	
}
/*_______________________________________________________________________________________________________________________________*/
/*Description: It transmites string through UART
 * Input     : Two paraameters 
 *              1-(uint8_t* pchar_index)It takes pointer to char 
 * Output    : Error Checking
 *_______________________________________________________________________________________________________________________________*/
UART_Error_t UART_SendString(uint8_t* pchar_index)
{
  if(sgu8_UART_Execution_Mode==UART_Polling_mode_enable)
  {
	while(*(pchar_index))
	{
	UART_Sendchar(*pchar_index++);
	}
  }
  else
  {
 
  }
}
/*_______________________________________________________________________________________________________________________________*/
/*Description: It receives character from  UART
 * Input     : (uint8_t* pchar_index)It takes pointer to char
 * Output    : Error Checking
 *_______________________________________________________________________________________________________________________________*/

UART_Error_t UART_RecByte(uint8_t volatile * volatile pchar_index)
{
    if(sgu8_UART_Receiver_enable==UART_Receive_Enable)
	{
		if(sgu8_UART_Execution_Mode==UART_Polling_mode_enable)
		{
			while ( !( UART_CNTRL_STATUS_REG_A & (1<<UART_Receive_Complete_FLAG)) );
			
		}
		else if(sgu8_UART_Execution_Mode==UART_Interrupt_mode_enable)
		{

			/*Receive UDR*/
			*pchar_index=UDR;


		}
		else
		{
		}
		*pchar_index=UART_DATA_REGISTER;
	}
	else
	{
	}

}