/*
 * UART.h
 *
* Created: 12/19/2019 10:40:43 AM
*  Author: H
 */ 


#ifndef UART_H_
#define UART_H_

#include <avr/interrupt.h>
#include "Infra_Structure/STD_TYPES.h"
#include "Infra_Structure/MACROS.h"
#include "MCAL/UART/UART_Ret_Val.h"
#include "Infra_Structure/AVR.h"


/*
*	Definition of UART register's bits
*/

/*CSRA*/
#define UART_MPCM  0
#define UART_U2X   1
#define UART_PE    2
#define UART_DOR   3
#define UART_FE    4
#define UART_DRE   5
#define UART_TXC   6
#define UART_RXC   7

/*CSRB*/
#define UART_TXB8  0
#define UART_RXB8  1
#define UART_UCSZ2 2
#define UART_TXEN  3
#define UART_RXEN  4
#define UART_DRIE  5
#define UART_TXCIE 6
#define UART_RXCIE 7

/*CSRC*/
#define UART_CPOL  0
#define UART_CSZ0  1
#define UART_CSZ1  2
#define UART_SBS   3
#define UART_PM0   4
#define UART_PM1   5
#define UART_MSEL  6
#define UART_RSEL  7

/*
*	Used macros 
*/
#define F_CPU 8000000UL
#define UART_POLLING   0u
#define UART_INTERRUPT 1u

#define TRANSMITTER    0u
#define RECEIVER       1u
#define TRANSCEIVER    2u

typedef struct 
{
	uint8_t u8Interrupt_mode ;
	uint8_t u8Desired_operation ;
	uint32_t u32Baud_rate ;	
}stUART_cfg;

/** 
 * \brief UART_Init : 
 *
 * \param *pstUART_cfg : A pointer to a struct that holds the desired UART configuration
 * \return Error : Indication to the function execution 
 *
 **/     
extern uint8_t UART_Init(stUART_cfg *pstUART_cfg);

/** 
 * \brief  UART_Send_Byte : Sends a certain byte
 *
 * \param u8Data : The byte the user wants to send
 * \return Error : Indication to the function execution 
 *
 **/  
extern uint8_t UART_Send_Byte(uint8_t u8Data);

/** 
 * \brief  UART_Receive_Byte : Receives a certain byte
 *
 * \param pu8Received_data : The sent byte will be stored in the passed pointer 
 * \return Error : Indication to the function execution 
 *
 **/  
extern uint8_t UART_Receive_Byte(uint8_t *pu8Received_data);
   
#endif /* UART_H_ */
