/*
 * main.c
 *
 *  Created on: Dec 26, 2019
 *      Author: H
 */
#include "AVR_MC1.h"
#include "PBConfg.h"
int main (void)
{
	System_one_Init(&str_config);
	DIO_INIT_Pin(PA0,OUTPUT);
	while(1)
	{
		System_Dispatch();
	}
}

