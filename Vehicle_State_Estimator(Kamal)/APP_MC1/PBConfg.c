/*
 * PBConfg.c
 *
 *  Created on: Dec 26, 2019
 *      Author: H
 */
#include "PBConfg.h"

stUART_cfg str_UART_sys_one={UART_INTERRUPT,RECEIVER,9600ul};

str_Configuration_SPI_t str_SPI_sys_one={SPI_MODE_MASTER, SPI_PRESCALER_SPEED_4, SPI_INTERRUPT_MODE,SPI_SPEED_NORMAL_MODE,DATA_ORDER_LSB_FIRST};

str_system_One_t str_config={&str_UART_sys_one,&str_SPI_sys_one,10,27};


