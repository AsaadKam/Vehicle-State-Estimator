/*
 * DIO.h
 *
 * Created: 02/11/2019 00:35:59
 *  Author: Native_programmer
 */ 


#ifndef DIO_H_
#define DIO_H_

#include "BitManipulation.h"
#include "Data_Types.h"
#include "DIO_PostBConfig.h"
/* _________________DIO PINS_______________________*/
#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7
#define B0 8
#define B1 9
#define B2 10
#define B3 11
#define B4 12
#define B5 13
#define B6 14
#define B7 15
#define C0 16
#define C1 17
#define C2 18
#define C3 19
#define C4 20
#define C5 21
#define C6 22
#define C7 23
#define D0 24
#define D1 25
#define D2 26
#define D3 27
#define D4 28
#define D5 29
#define D6 30
#define D7 31

/*_____________________________AVR_PORTS________________________________*/
#define PA 0
#define PB 1
#define PC 2
#define PD 3


/***************************************************************/
/******************GENERAL_PARPOUS_INPUT_OUTPUT*****************/
/***************************************************************/

//typedef
typedef enum {NOTHING,LOW_LEVEL ,LOGICAL_CHANGE,FALLING ,RISING} ExternalInterruptTypes;
typedef enum {Disable , Enable} EnableDisable;
//defines
#define INPUT  0
#define OUTPUT 1

#define LOW  0
#define HIGH   1

#define LOWER_NIBBLE  0
#define HIGHER_NIBBLE 1

#define SHIFTED_LEFT 0
#define NOT_SHIFTED  1

//Macro
#define  EnableGeneralInterrupt()    SET_BIT(CPU_FLAG_Register,INT_BIT)

//Functions
extern uint8_t DIO_Init_Nibble(uint8_t por,uint8_t nibble,uint8_t direction); // function used to initiate data direction of a given port.....  ex:- fun(LCD_PORT,INPUT)

extern uint8_t DIO_Init_Nibble(uint8_t por,uint8_t nibble,uint8_t direction); // function to initiate data direction of nibble in a given port.....  ex:- fun(SEVEN_SEGGMENT_PORT,LOWER_NIBBLE,OUTPUT)

extern uint8_t DIO_INIT_PORT(uint8_t Port_Name,uint8_t direction); // function used to initiate data direction a single pin in a given register.....ex:- fun(KEY_BAD_PORT,6,INPUT)

extern uint8_t DIO_Read_Port (uint8_t por, uint8_t * ReadData); // function returns port state.....  ex:- fun(LCD_PORT)

extern uint8_t DIO_Write_Port(uint8_t por,uint8_t Data); // function to write data to given port.....  ex:- fun(LCD_PORT,99)

extern uint8_t DIO_Read_Pin (uint8_t pin, uint8_t * ReadData); // function that returns the value of a pin in a register.....  ex:- fun(LCD_PORT,LCD_RS_PIN)

extern uint8_t DIO_Write_Pin(uint8_t pin,uint8_t BitValue); // function used to.....  ex:- fun(LCD_PORT,LCD_ENABLE_PIN,1)

extern uint8_t DIO_toggle_Port(uint8_t por); // this function is used to toggle defined port.....  ex:- fun(LED_LINE_PORT)

extern uint8_t DIO_toggle_Pin(uint8_t pin); // this function is used to toggle pin in a defined port .....  ex:- fun(LED_PORT,LED_PIN)

extern uint8_t DIO_Read_Nibble(uint8_t por,uint8_t nibble,uint8_t shifted_left_or_not,uint8_t * ReadData); // this function is used to read a nibble ....ex:- fun(LCD_DATA_PORT,HIGHER_NIBBLE,NOT_SHIFTED)

extern uint8_t DIO_Write_Nibble(uint8_t por ,uint8_t lower_or_higher_nibble,uint8_t data); // this function is used to write on nibble ... ex:- fun(LCD_DATA_PORT,LOWER_NIBBLE,'M')

#define DIO_ERROR_OK   0
#define DIO_ERROR_NOK  1


#endif /* DIO_H_ */