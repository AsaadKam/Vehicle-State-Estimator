/*
 * SPI_Driver.c
 *
 *  Created on: Dec 20, 2019
 *      Author: H
 */

#include "SPI.h"
/*___________________LOCAL_MACROS_____________________*/
#define NORMAL_PRESCALER_MASK 0XF3
#define DOUBLE_PRESCALER_MASK 0XF1

static uint8_t gu8_Interrupt_Mode =0;
static uint8_t gu8_Interrupt_Flag=1;

Error_Type_t SPI_Init (str_Configuration_SPI_t * str_Confg)
{
	Error_Type_t au8_error= 0;
	DIO_INIT_Pin(B4,OUTPUT);
	DIO_INIT_Pin(B5, OUTPUT);
	DIO_INIT_Pin(B6, INPUT);
	DIO_INIT_Pin(B7, OUTPUT);
	if(str_Confg->au8_SPI_Mode==SPI_MODE_MASTER) /*Check If structure Configuration is passing Master_Mood*/
	{
		SET_BIT(SPCR,SPCR_MSTR);
		if(str_Confg->au8_prescaler==((SPI_PRESCALER_SPEED_4)||(SPI_PRESCALER_SPEED_16)||(SPI_PRESCALER_SPEED_64) || (SPI_PRESCALER_SPEED_128)))
		{
			SPCR &= NORMAL_PRESCALER_MASK;
			SPCR |= str_Confg->au8_prescaler;
			CLEAR_BIT(SPCR,SPCR_CPHA);
			CLEAR_BIT(SPCR,SPCR_CPOL);
			au8_error=ERROR_INIT_MASTER_OK;
		}
		else
		{
			au8_error=ERROR_INIT_PRESCALER_NOT_DEFINED;
		}
		if(str_Confg->au8_SPI_Data_Order==(DATA_ORDER_MSB_FIRST))
		{
			CLEAR_BIT(SPCR,SPCR_DORD);
			au8_error=ERROR_INIT_MASTER_OK;
		}
		else if(str_Confg->au8_SPI_Data_Order==(DATA_ORDER_LSB_FIRST))
		{
			SET_BIT(SPCR,SPCR_DORD);
			au8_error=ERROR_INIT_MASTER_OK;
		}
		else
		{
			au8_error=ERROR_INIT_DATA_ORDER_NOT_DEFINED;
		}
		if(str_Confg->au8_Speed_Mode==SPI_SPEED_DOUBLE_MODE)
		{
			SET_BIT(SPSR,SPSR_SPI2X);
			au8_error=ERROR_INIT_MASTER_OK;
		}
		else if(str_Confg->au8_Speed_Mode==SPI_SPEED_NORMAL_MODE){
			au8_error=ERROR_INIT_MASTER_OK;
		}
		else{
			au8_error=ERROR_INIT_SPEED_MODE_NOT_DEFINED;
		}
		if(str_Confg->au8_Interrupt_Mode==SPI_INTERRUPT_MODE)
		{
		   	SET_BIT(SREG,7);
		    SET_BIT(SPCR,SPCR_SPIE);
			gu8_Interrupt_Mode= SPI_INTERRUPT_MODE;
			au8_error=ERROR_INIT_MASTER_OK;
		}
		else if(str_Confg->au8_Interrupt_Mode==SPI_POLLING_MODE)
		{
			gu8_Interrupt_Mode= SPI_POLLING_MODE;
			au8_error=ERROR_INIT_MASTER_OK;
		}
		else
		{
			au8_error=ERROR_INIT_INTEERUPT_MODE_NOT_DEFINED;
		}
		SET_BIT(SPCR,SPCR_SPE);
		return au8_error;
	}
	else if(str_Confg->au8_SPI_Mode==SPI_MODE_SLAVE)
	{
		DIO_INIT_Pin(B4, INPUT);
		DIO_INIT_Pin(B5, INPUT);
		DIO_INIT_Pin(B6, OUTPUT);
		DIO_INIT_Pin(B7, INPUT);
		CLEAR_BIT(SPCR,SPCR_MSTR); /*ACTIVATE SLAVE MOODE*/
		if(str_Confg->au8_SPI_Data_Order==DATA_ORDER_MSB_FIRST)
		{
			CLEAR_BIT(SPCR,SPCR_DORD);
			SET_BIT(SPCR,SPCR_SPE); /*ENABLE SPI*/
			if(str_Confg->au8_Interrupt_Mode==SPI_INTERRUPT_MODE)
			{
				SET_BIT(SREG,7);
		        SET_BIT(SPCR,SPCR_SPIE);
				gu8_Interrupt_Mode=SPI_INTERRUPT_MODE;
				au8_error=ERROR_INIT_SLAVE_OK;
			}
			else if(str_Confg->au8_Interrupt_Mode==SPI_POLLING_MODE)
			{
				gu8_Interrupt_Mode=SPI_POLLING_MODE;
				au8_error=ERROR_INIT_SLAVE_OK;
			}
			else
			{
				au8_error=ERROR_INIT_INTEERUPT_MODE_NOT_DEFINED;
			}
		}
		else if(str_Confg->au8_SPI_Data_Order==DATA_ORDER_LSB_FIRST)
		{
			SET_BIT(SPCR,SPCR_DORD);
			SET_BIT(SPCR,SPCR_SPE); /*ENABLE SPI*/
			if(str_Confg->au8_Interrupt_Mode==SPI_INTERRUPT_MODE)
			{
				SET_BIT(SREG,7);
	        	SET_BIT(SPCR,SPCR_SPIE);
				gu8_Interrupt_Mode=SPI_INTERRUPT_MODE;
				au8_error=ERROR_INIT_SLAVE_OK;
			}
			else if(str_Confg->au8_Interrupt_Mode==SPI_POLLING_MODE)
			{
				gu8_Interrupt_Mode=SPI_POLLING_MODE;
				au8_error=ERROR_INIT_SLAVE_OK;
			}
			else
			{
				au8_error=ERROR_INIT_INTEERUPT_MODE_NOT_DEFINED;
			}
		}
		else
		{
			au8_error=ERROR_INIT_DATA_ORDER_NOT_DEFINED;
		}

	}
	else
	{
		au8_error=ERROR_INIT_MASTER_MODE_NOT_DEFINED;
	}
	return au8_error;
}

Error_Type_t SPI_SendByte (const uint8_t au8_Data)
{
	uint8_t au8_error=0;
	if(gu8_Interrupt_Mode==SPI_INTERRUPT_MODE)
	{
		SET_BIT(SREG,7);
		SET_BIT(SPCR,SPCR_SPIE);
		if(gu8_Interrupt_Flag==1)
		{
			SPDR=au8_Data;
			gu8_Interrupt_Flag=0;
		}
		au8_error=ERROR_SEND_OK;
	}
	else if(gu8_Interrupt_Mode==SPI_POLLING_MODE)
	{
		SPDR=au8_Data;
		while(BIT_IS_CLEAR(SPSR,SPSR_SPIF)){}
		au8_error=ERROR_SEND_OK;
	}
	else{
		au8_error=ERROR_SEND_NOK;
	}
	return au8_error;
}

Error_Type_t SPI_RecieveByte (uint8_t * au8_PtrData)
{
	uint8_t au8_error=0;
	if(gu8_Interrupt_Mode==SPI_INTERRUPT_MODE)
	{
/* 		SET_BIT(SREG,7);
		SET_BIT(SPCR,SPCR_SPIE);
		if(gu8_Interrupt_Flag==1)
		{ */
			*au8_PtrData=SPDR;
/* 			gu8_Interrupt_Flag=0; */
/* 		} */
		au8_error=ERROR_RECIEVE_OK;
	}
	if(gu8_Interrupt_Mode==SPI_POLLING_MODE)
	{
		*au8_PtrData=SPDR;
		while(BIT_IS_CLEAR(SPSR,SPSR_SPIF)){}
		au8_error=ERROR_RECIEVE_OK;
	}
	else
	{
		au8_error=ERROR_RECIEVE_NOK;
	}

	return au8_error;
}

ISR(SPI_STC_vect)
{
/* 	gu8_Interrupt_Flag=1; */
/* 	CLEAR_BIT(SPCR,SPCR_SPIE); */
	DIO_Write_pin(PA0,HIGH);
}


