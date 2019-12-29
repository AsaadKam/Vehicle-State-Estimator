/*
 * AVR_MC1.c
 *
 *  Created on: Dec 26, 2019
 *      Author: H
 */


#include "AVR_MC1.h"
/**********************MACROS*****************************/
#define SYSTEM_IDLE 0u
#define SYSTEM_NORMAL_SPEED 1u
#define SYSTEM_UPDATED_SPEED 2u

/************GLOBAL_Variables****************************/
static volatile uint32_t gu8_Init_Speed= NULL;
static uint8_t gu8_Dispatch_status = SYSTEM_IDLE;
//static uint32_t gu8_updated_speed=NULL;
static uint8_t gu8_sw1_flag=NULL;
static uint8_t gu8_sw2_flag=NULL;
static uint8_t gu8_UART_Flag=NULL;
ERROR_TYPE_t System_one_Init(str_system_One_t * str_system_confg)
{
	uint8_t au8_error;
	if(str_system_confg->str_UART_sys_one->u8Desired_operation==(TRANSMITTER||RECEIVER||TRANSCEIVER))
	{
		au8_error= System_One_Ok;
	}
	else
	{
		au8_error=UART_TRANSMITION_NOK;
	}
	if(str_system_confg->str_UART_sys_one->u32Baud_rate==NORMAL_UART_SPEED)
	{
		au8_error= System_One_Ok;
	}
	else
	{
		au8_error=UART_BAUDERATE_NOK;
	}
	if(str_system_confg->str_UART_sys_one->u8Interrupt_mode==(UART_POLLING||UART_INTERRUPT))
	{
		au8_error= System_One_Ok;
	}
	else{

	}
	if(str_system_confg->str_SPI_sys_one->au8_Interrupt_Mode==(SPI_INTERRUPT_MODE||SPI_POLLING_MODE))
	{
		au8_error= System_One_Ok;
	}
	else
	{
		au8_error=UART_INTERRUPT_MODE_NOK;
	}
	if(str_system_confg->str_SPI_sys_one->au8_SPI_Mode==(SPI_MODE_MASTER||SPI_MODE_SLAVE))
	{
		au8_error= System_One_Ok;
	}
	else
	{
		au8_error=SPI_MODE_NOK;
	}
	if(str_system_confg->str_SPI_sys_one->au8_Speed_Mode==(SPI_SPEED_NORMAL_MODE||SPI_SPEED_DOUBLE_MODE))
	{
		au8_error= System_One_Ok;
	}
	else{
		au8_error=SPI_SPEED_NOK;
	}
	if(str_system_confg->str_SPI_sys_one->au8_prescaler==(SPI_PRESCALER_SPEED_4||SPI_PRESCALER_SPEED_16||SPI_PRESCALER_SPEED_64||SPI_PRESCALER_SPEED_128))
	{
		au8_error= System_One_Ok;
	}
	else
	{
		au8_error=SPI_PRESCALER_NOK;
	}
	if(str_system_confg->str_SPI_sys_one->au8_SPI_Data_Order==(DATA_ORDER_MSB_FIRST||DATA_ORDER_MSB_FIRST))
	{
		au8_error= System_One_Ok;
	}
	else
	{
		au8_error=SPI_BIT_ORDER_NOK;
	}
	if(((str_system_confg->sw_one_channel&&str_system_confg->sw_two_channel)>=0)&&((str_system_confg->sw_one_channel&&str_system_confg->sw_two_channel)<=31))
	{
		au8_error= System_One_Ok;
	}
	else
	{
		au8_error=DIO_CHANNELS_NOK;
	}
	if(au8_error==System_One_Ok)
	{
		UART_Init(str_system_confg->str_UART_sys_one);
		SPI_Init(str_system_confg->str_SPI_sys_one);
		DIO_INIT_Pin(str_system_confg->sw_one_channel,INPUT);
		DIO_INIT_Pin(str_system_confg->sw_two_channel,INPUT);
		SREG|=(1<<7); /*enable global interrupt*/
		GICR|=(1<<5); /*enable external interr 2*/
		MCUCSR|=(1<<6); /*make it work with rising edge*/
		GICR|=(1<<7);  /*enable external interr 1*/
		MCUCR|=(1<<2); /*make it work with rising edge*/
		MCUCR|=(1<<3);
	}
	return au8_error;
}

void System_Dispatch(void)
{
	switch(gu8_Dispatch_status)
	{
	case SYSTEM_IDLE:
		break;
	case SYSTEM_NORMAL_SPEED:
		SPI_SendByte(gu8_Init_Speed);
		gu8_Dispatch_status=SYSTEM_UPDATED_SPEED;
		break;
	case SYSTEM_UPDATED_SPEED:
		if(gu8_sw1_flag==1)
		{
			gu8_Init_Speed+=gu8_sw1_flag;
			gu8_sw1_flag=0;
			gu8_Dispatch_status=SYSTEM_NORMAL_SPEED;
		}
		if(gu8_sw2_flag==1)
		{
			gu8_Init_Speed-=gu8_sw1_flag;
			gu8_sw2_flag=0;
			gu8_Dispatch_status=SYSTEM_NORMAL_SPEED;
		}
		break;
	}
}

ISR(USART_RXC_vect)
{
	UART_Receive_Byte(&gu8_Init_Speed);
	gu8_UART_Flag++;
	gu8_Dispatch_status=SYSTEM_NORMAL_SPEED;
}

ISR(INT2_vect)
{
	if(gu8_UART_Flag>0)
	{
		gu8_sw1_flag++;
		gu8_Dispatch_status=SYSTEM_UPDATED_SPEED;
	}
}
ISR(INT1_vect)
{
	if(gu8_UART_Flag>0)
	{
		gu8_sw2_flag++;
		gu8_Dispatch_status=SYSTEM_UPDATED_SPEED;
	}
}
