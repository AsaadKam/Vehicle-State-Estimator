/*
 * AVR_MC1.h
 *
 *  Created on: Dec 26, 2019
 *      Author: H
 */

#ifndef APP_MC1_AVR_MC1_H_
#define APP_MC1_AVR_MC1_H_
#include "MCAL/SPI/SPI.h"
#include "MCAL/UART/UART.h"
#include "MCAL/DIO/DIO.h"
#include"avr/interrupt.h"

/*******************Typedef_Struct**************/

typedef uint8_t ERROR_TYPE_t;
/*
 * Definition: A structure to initialize SPI & UART preferals needed
 * Input: (UART Structure, SPI Structure, SWitch_one_channel, Switch_Two_Channale)
 * Output :Error Checking
 */
typedef struct
{
	stUART_cfg * str_UART_sys_one;
	str_Configuration_SPI_t * str_SPI_sys_one;
	uint8_t sw_one_channel;
	uint8_t sw_two_channel;
}str_system_One_t;

/***********************MACROS*******************************/
#define NORMAL_UART_SPEED 9600ul

#define System_One_Ok 0u
#define UART_TRANSMITION_NOK 1u
#define UART_BAUDERATE_NOK 2u
#define UART_INTERRUPT_MODE_NOK 3u
#define SPI_MODE_NOK 4u
#define SPI_SPEED_NOK 5u
#define SPI_PRESCALER_NOK 6u
#define SPI_BIT_ORDER_NOK 7u
#define DIO_CHANNELS_NOK 8u
/***************Function_Prortotypes****************************/

ERROR_TYPE_t System_one_Init(str_system_One_t * str_system_confg);

void System_Dispatch(void);

#endif /* APP_MC1_AVR_MC1_H_ */
