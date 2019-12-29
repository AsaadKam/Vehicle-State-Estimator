/*
 * Config.h
 *
 * Created: 02/11/2019 00:36:52
 *  Author: Native_programmer
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_

#include "Atmega32Registers.h"

#define PinsNeeded  32
/* _________________DIO PINS_____________________?__*/
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

#define P0          0
#define P1          1
#define P2          2
#define P3          3
#define P4          4
#define P5          5
#define P6          6
#define P7          7
#define P8          8
#define P9          9
#define P10         10//INT2
#define P11         11
#define P12         12
#define P13         13
#define P14         14
#define P15         15
#define P16         16
#define P17         17
#define P18         18
#define P19         19
#define P20         20
#define P21         21
#define P22         22
#define P23         23
#define P24         24
#define P25         25
#define P26         26//INT0
#define P27         27//INT1
#define P28         28
#define P29         29
#define P30         30
#define P31         31


#define No_of_pins  No_of_ports*PortSize

#define CPU_FLAG_Register                     SREG
#define INT_BIT                               I

#define ReadRegisterA                         PINA
#define WriteRegisterA                        PORTA
#define DirectionRegisterA                    DDRA
 
#define ReadRegisterB                         PINB
#define WriteRegisterB                        PORTB
#define DirectionRegisterB                    DDRB

#define ReadRegisterC                         PINC
#define WriteRegisterC                        PORTC
#define DirectionRegisterC                    DDRC

#define ReadRegisterD                         PIND
#define WriteRegisterD                        PORTD
#define DirectionRegisterD                    DDRD

#define ExternalInterruptRegister             GICR

#define ExternalInterrupt_2_ENABLE            INT2
#define ExternalInterrupt_0_ENABLE            INT0
#define ExternalInterrupt_1_ENABLE            INT1

#define ExternalInterruptSenseCntrlRegister_1 MCUCR

#define ExternalInterrupt_1_LOW_LEVEL         0b00000000
#define ExternalInterrupt_1_LOGICAL_CHANGE    0b00000100
#define ExternalInterrupt_1_RISING            0b00001000
#define ExternalInterrupt_1_FALLING           0b00001100
#define ExternalInterrupt_0_LOW_LEVEL         0b00000000
#define ExternalInterrupt_0_LOGICAL_CHANGE    0b00000001
#define ExternalInterrupt_0_RISING            0b00000010
#define ExternalInterrupt_0_FALLING           0b00000011

#define ExternalInterruptSenseCntrlRegister_2 MCUCSR

#define ExternalInterrupt_2_FALLING           0b00000000
#define ExternalInterrupt_2_RISING            0b01000000

/*
#define ReadRegisterE       PINE
#define WriteRegisterE      PORTE
#define DirectionRegisterE  DDRE

#define ReadRegisterF       PINF
#define WriteRegisterF      PORTF
#define DirectionRegisterF  DDRF
*/



#endif /* CONFIG_H_ */