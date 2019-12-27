/*
 * AVR.h
 *
 *  Created on: Nov 2, 2019
 *      Author: H
 */

#ifndef SPRINT2_AVR_H_
#define SPRINT2_AVR_H_

#define PORTA *((volatile uint8_t*) 0x3B)
#define PORTB *((volatile uint8_t*) 0x38)
#define PORTC *((volatile uint8_t*) 0x35)
#define PORTD *((volatile uint8_t*) 0x32)

#define PINA *((volatile uint8_t*) 0x39)
#define PINB *((volatile uint8_t*) 0x36)
#define PINC *((volatile uint8_t*) 0x33)
#define PIND *((volatile uint8_t*) 0x30)

#define DDRA *((volatile uint8_t*) 0x3A)
#define DDRB *((volatile uint8_t*) 0x37)
#define DDRC *((volatile uint8_t*) 0x34)
#define DDRD *((volatile uint8_t*) 0x31)

#define TCCR2 *((volatile uint8_t*) 0X45)
#define TCNT2 *((volatile uint8_t*) 0X44)
#define OCR2 *((volatile uint8_t*) 0X43)
#define TIMSK *((volatile uint8_t*) 0X59)
#define TIFR *((volatile uint8_t*) 0X58)

#define TCCR1A *((volatile uint8_t*) 0X4F)
#define TCCR1B *((volatile uint8_t*) 0X4E)
//#define TCNT1H *((volatile uint8*) 0X4D)
//#define TCNT1 *((volatile uint8*) 0X4C)
#define TCNT1 *((volatile uint16_t*) 0X4C)
#define OCR1A *((volatile uint16_t*) 0X4A)
//#define OCR1AH *((volatile uint8*) 0X4B)
//#define OCR1AL *((volatile uint16*) 0X4A)
#define OCR1BH *((volatile uint8_t*) 0X49)
#define OCR1BL *((volatile uint8_t*) 0X48)



#define SREG *((volatile uint8_t*) 0X5F)
#define GICR *((volatile uint8_t*) 0X5B)
#define GIFR *((volatile uint8_t*) 0X5A)
#define MCUCR *((volatile uint8_t*) 0X55)
#define MCUCSR *((volatile uint8_t*) 0X54)


#define TCCR0  *((volatile uint8_t*)0x53)
#define TCNT0  *((volatile uint8_t*)0x52)
#define OCR0   *((volatile uint8_t*)0x5C)


#define SPDR *((volatile uint8_t*)0X2F)

#define SPSR *((volatile uint8_t*)0X2E)
#define SPSR_SPIF 7
#define SPSR_WCOL 6
#define SPSR_SPI2X 0

#define SPCR *((volatile uint8_t*)0X2D)
#define SPCR_SPR0 0
#define SPCR_SPR1 1
#define SPCR_CPHA 2
#define SPCR_CPOL 3
#define SPCR_MSTR 4
#define SPCR_DORD 5
#define SPCR_SPE 6
#define SPCR_SPIE 7





#define TCCR0_WGM00 6
#define TCCR0_WGM01 3
#define TCCR0_FOC0  7
#define TCCR0_COM01 5
#define TCCR0_COM00 4

#define TCCR0_CS00 0
#define TCCR0_CS01 1
#define TCCR0_CS02 2

#define TIMSK_TOIE0 0
#define TIMSK_OCIE0 1

#define TIFR_TOV0   0
#define TIFR_OCF0   1


/*
*	Definition of UART registers
*/
#define UART_CSRC ( *((volatile unsigned char *) (0x40) ) )  /* Multiple use with USART_BRRH >> URSEL is set to choose which one will override */
#define UART_BRRH ( *((volatile unsigned char *) (0x40) ) )  /* Multiple use with USART_CSRC >> URSEL is set to choose which one will override */
#define UART_BRRL ( *((volatile unsigned char *) (0x29) ) )
#define UART_DR   ( *((volatile unsigned char *) (0x2c) ) )
#define UART_CSRA ( *((volatile unsigned char *) (0x2b) ) )
#define UART_CSRB ( *((volatile unsigned char *) (0x2a) ) )

#define UCSRA_MPCM  0U
#define UCSRA_U2X   1U
#define UCSRA_PE    2U
#define UCSRA_DOR   3U
#define UCSRA_FE    4U
#define UCSRA_UDRE  5U
#define UCSRA_TXC   6U
#define UCSRA_RXC   7U


#define UCSRB_RXCIE  7
#define UCSRB_TXCIE  6
#define UCSRB_UDRIE  5
#define UCSRB_RXEN   4
#define UCSRB_TXEN   3
#define UCSRB_UCSZ2  2
#define UCSRB_RXB8   1
#define UCSRB_TXB8   0

#define UCSRC_URSEL  7
#define UCSRC_UMSEL  6
#define UCSRC_UPM1   5
#define UCSRC_UPM0   4
#define UCSRC_USBS   3
#define UCSRC_UCSZ1  2
#define UCSRC_UCSZ0  1
#define UCSRC_UCPOL  0


#endif /* SPRINT2_AVR_H_ */
