/*
 * SPI.h
 *
 *  Created on: Dec 20, 2019
 *      Author: H
 */

#ifndef PROJECT_SPI_H_
#define PROJECT_SPI_H_

/*- INCLUDES -----------------------------------------------*/
#include "Atmega32Registers.h"
#include "BitManipulation.h"
#include "Data_Types.h"
#include "DIO.h"


/*----TYPEDEF------------------------------------*/
typedef uint8_t Error_Type_t ;
/*- STRUCTS -------------------------------------*/
typedef struct {
	uint8_t au8_SPI_Mode;
	/*MASTER OR SLAVE MODE*/
	uint8_t au8_prescaler;
	/*Prescaler Is only needed when configuration as a master*/
	uint8_t au8_Interrupt_Mode;
	/*Configure the Mode of Interrupt (POLLING, INTERRUPT)*/
	uint8_t au8_Speed_Mode;
	/*Configure the speed of SPI(Normal, Double)*/
	uint8_t au8_SPI_Data_Order;
	/*DATA ORDER IS MOST SEGNIFICANT OR MOST SEGINIFICANT*/
}str_Configuration_SPI_t;


/*----MACROS-------------------------------------------*/

     /*PRESCALER_MACROS*/
#define SPI_PRESCALER_SPEED_4 (0u)
#define SPI_PRESCALER_SPEED_16 (1u)
#define SPI_PRESCALER_SPEED_64 (2u)
#define SPI_PRESCALER_SPEED_128 (3u)

      /*SPI_MODE_MACROS*/
#define SPI_MODE_MASTER (0u)
#define SPI_MODE_SLAVE (1u)

       /*SPI_MODE_INTERRUPT*/
#define SPI_INTERRUPT_MODE (0u)
#define SPI_POLLING_MODE (1U)

       /*SPI_MODE_SPEED*/
#define SPI_SPEED_NORMAL_MODE (0u)
#define SPI_SPEED_DOUBLE_MODE (1u)

           /*SPI_MODE_POLARITY*/
#define SPI_CLK_POLARITY (0U)

             /*SPI_MODE_PHASE*/
#define SPI_CLK_PHASE (0u)

#define DATA_ORDER_MSB_FIRST (0u)
#define DATA_ORDER_LSB_FIRST (1u)


#define ERROR_INIT_MASTER_OK (0U)
#define ERROR_INIT_MASTER_MODE_NOT_DEFINED (1U)
#define ERROR_INIT_PRESCALER_NOT_DEFINED (2u)
#define ERROR_INIT_DATA_ORDER_NOT_DEFINED (3U)
#define ERROR_INIT_INTEERUPT_MODE_NOT_DEFINED (4U)
#define ERROR_INIT_SPEED_MODE_NOT_DEFINED (5u)
#define ERROR_INIT_SLAVE_OK (06)

#define ERROR_SEND_NOK (1u)
#define ERROR_SEND_OK (0u)

#define ERROR_RECIEVE_OK (0u)
#define ERROR_RECIEVE_NOK (1u)
/*- FUNCTION DECLARATIONS ----------------------------------*/

Error_Type_t SPI_Init (str_Configuration_SPI_t * str_Confg);

Error_Type_t SPI_SendByte (const uint8_t au8_Data);

Error_Type_t SPI_RecieveByte (uint8_t * au8_PtrData);

Error_Type_t SPI_Trans_SetCallBack(PntrToFunc_t Copy_pfunc_callbackfunc_ISR);
#endif /* PROJECT_SPI_H_ */
