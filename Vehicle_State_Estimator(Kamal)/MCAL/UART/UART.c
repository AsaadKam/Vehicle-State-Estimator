/*
 * UART.c
 *
 * Created: 
 *  Author: 
 */ 

#include "UART.h"

/*
*	Static variable that tells if the UART module is initialized or not 
*/
static uint8_t gu8Is_UART_initialized = UART_E_NOT_INITIALIZED_YET ;

/*
*	Static variable that holds the minimum acceptable baud rate which in case of using 8MHZ oscillator will be 1953
*   The equations exist in the data sheet (Page : 143) -> Regarding using only normal mode
*/
static uint32_t gu32Minimum_baudrate =  ( (F_CPU)/(16*(255+1)) );

/*
*	Static variable that holds the maximum acceptable baud rate which in case of using 8MHZ oscillator will be 500000
*   The equations exist in the data sheet (Page : 143) -> Regarding using only normal mode
*/
static uint32_t gu32Maximum_baudrate =  ( (F_CPU)/(16*(0+1)) ) ;

/*
*	Static global variable that holds the used operation mode ( Transmitter , Receiver , Transceiver )
*/
static uint8_t gu8Operation_mode = 0 ;

/*
*	Static global variable that holds the used Interrupt mode ( Interrupt , or polling )
*/
static uint8_t gu8Interrupt_mode = 0 ;

/*
*	Static variable holds the desired data to receive
*/
static uint8_t gu8Sent_data = 0 ;

/** 
 * \brief UART_Init : 
 *
 * \param *pstUART_cfg : A pointer to a struct that holds the desired UART configuration
 * \return Error : Indication to the function execution .
 *
 **/     
extern uint8_t UART_Init(stUART_cfg *pstUART_cfg)
{
	/*
	*	The variable that will hold the indication of the execution
	*/
	uint8_t u8Execution_indication  = UART_E_OK ;
		
	/* Checking if the passed pointer is not a null one */
	if(0 != pstUART_cfg)
	{
		if ( UART_INTERRUPT == pstUART_cfg->u8Interrupt_mode || UART_POLLING == pstUART_cfg->u8Interrupt_mode)
		{
			if ( pstUART_cfg ->u32Baud_rate >= gu32Minimum_baudrate && pstUART_cfg ->u32Baud_rate <= gu32Maximum_baudrate)
			{
				/* CSZ0 & CSZ1 are set for using 8-bit mode , RSEL is set to use CSRC not BRRH (Duplicated registers)*/
				UART_CSRC |= (1 << UART_RSEL) ;
				UART_CSRC |= (1 << UART_CSZ0) ;
				UART_CSRC |= (1 << UART_CSZ1) ;
				
				/* Choosing the operation mode whether it's receiver , transmitter ,or transceiver*/
				if (TRANSMITTER == pstUART_cfg ->u8Desired_operation)
				{
					gu8Operation_mode = TRANSMITTER ;
					
					/* Enable the transmit complete interrupt */
					UART_CSRB |= (1<< UART_TXCIE) ;
					
					UART_CSRB |= (1 << UART_TXEN) ;
				} 
				else if(RECEIVER == pstUART_cfg ->u8Desired_operation)
				{
					gu8Operation_mode = RECEIVER ;
					
					/* Enable the receive complete interrupt */
					UART_CSRB |= (1<< UART_RXCIE) ;
					
					UART_CSRB |= (1 << UART_RXEN) ;
				}
				else if(pstUART_cfg ->u8Desired_operation == TRANSCEIVER)
				{
					gu8Operation_mode = TRANSCEIVER ;
					
					/* Enable the transmit complete interrupt */
					UART_CSRB |= (1<< UART_TXCIE) ;
					/* Enable the receive complete interrupt */
					UART_CSRB |= (1<< UART_RXCIE) ;
					UART_CSRB |= (1 << UART_TXEN) ;
					UART_CSRB |= (1 << UART_RXEN) ;
				}
				else
				{
					/* Handling of entering wrong operation mode */
					u8Execution_indication = UART_E_WRONG_OPERATION_MODE ;
				}
				/* Setting the BRR value to get the desired baud rate*/
				UART_BRRL = ((F_CPU / (16 * pstUART_cfg->u32Baud_rate)) - 1) ;
			} 
			else
			{
				/* Handling of passing not acceptable baud rate */
				u8Execution_indication = UART_E_WRONG_BAUD_RATE ;
			}
			
			/* Passing the used interrupt mode to the corresponding static variable */
			if (UART_INTERRUPT == pstUART_cfg->u8Interrupt_mode)
			{
				gu8Interrupt_mode = UART_INTERRUPT ;
				SREG |=(1<<7);
				
			} 
			else
			{
				gu8Interrupt_mode = UART_POLLING ;
			}
		} 
		else
		{
			/* Handling of entering wrong interrupt mode */
			u8Execution_indication = UART_E_WRONG_INTERRUPT_MODE ;
		}
	}
	else
	{
		/* Handling of passing a null pointer */
		u8Execution_indication = UART_E_NULL_POINTER ;
	}
	
	if (UART_E_OK == u8Execution_indication)
	{
		gu8Is_UART_initialized = UART_E_INITIALIZED ;
	} 
	else
	{
		/* Do nothing (Leave it uninitialized) */
	}
	
	return u8Execution_indication ; 
}

/** 
 * \brief UART_Deinit : Deinitialize the UART module
 *
 * \param void
 * \return Error : Indication to the function execution 
 *
 **/     


/** 
 * \brief  UART_Send_Byte : Sends a certain byte
 *
 * \param u8Data : The byte the user wants to send
 * \return Error : Indication to the function execution 
 *
 **/  
extern uint8_t UART_Send_Byte(uint8_t u8Data)
{
	/*
	*	The variable that will hold the indication of the execution
	*/
	uint8_t u8Execution_indication  = UART_E_OK ;
	
	if (UART_E_INITIALIZED)
	{		
		if (UART_INTERRUPT == gu8Interrupt_mode )
		{
			/* Passing the data to be sent in a static variable so the ISR can access it */
			gu8Sent_data = u8Data ;
			
			/* Setting DRIE bit to enable the data register empty interrupt */
			UART_CSRB |= (1<< UART_DRIE) ;
		}
		else if (UART_POLLING == gu8Interrupt_mode )
		{
			UART_DR = u8Data ;
			while ( !(UART_CSRA & (1 << UART_DRE)) );
		}
		else
		{
			/* Handling of using wrong operation mode */
			u8Execution_indication = UART_E_WRONG_OPERATION_MODE ;
		}
	} 
	else
	{
		/* Handling of using the UART module before initializing it */
		u8Execution_indication = UART_E_NOT_INITIALIZED_YET ;
	}
	
	return u8Execution_indication ;
}

/** 
 * \brief  UART_Receive_Byte : Receives a certain byte
 *
 * \param pu8Received_data : The sent byte will be stored in the passed pointer 
 * \return Error : Indication to the function execution 
 *
 **/  
extern uint8_t UART_Receive_Byte(uint8_t *pu8Received_data)
{
	/*
	*	The variable that will hold the indication of the execution
	*/
	uint8_t u8Execution_indication  = UART_E_OK ;
	
	if (UART_E_INITIALIZED)
	{		
		if (UART_INTERRUPT == gu8Interrupt_mode )
		{
			*pu8Received_data = UART_DR ;
		}
		else if (UART_POLLING == gu8Interrupt_mode )
		{
			while ( !(UART_CSRA & (1 << UART_RXC)));
			*pu8Received_data = UART_DR ;
		}
		else
		{
			/* Handling of using wrong operation mode */
			u8Execution_indication = UART_E_WRONG_OPERATION_MODE ;
		}
	} 
	else
	{
		/* Handling of using the UART module before initializing it */
		u8Execution_indication = UART_E_NOT_INITIALIZED_YET ;
	}
	
	return u8Execution_indication ;
}


ISR(USART_UDRE_vect)
{
	UART_DR = gu8Sent_data ;
	/* Resetting DRIE bit to disable the data register empty interrupt */

	UART_CSRB &= ~(1<< UART_DRIE) ;
}

