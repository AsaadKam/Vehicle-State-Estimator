/*
 * APP.C
 *
 * Created: 17/12/2019 18:33:53\
 *  Author: Asaad
 */ 
/*- INCLUDES ----------------------------------------------*/
#include"DIO.h"
#include"Timer.h"
#include"Timer_PostBConfig.h"
#include"UART.h"
#include"UART_PostBConfig.h"
#include"SPI.h"
#include"SPI_PostBConfig.h"
#include"String_Manipulation.h"

/*- LOCAL MACROS ------------------------------------------*/
#define INT0_PIN        									 26
#define EXTERNAL_INTERRUPT_0_FUNC_ISR                        __vector_1
#define LOW_LEVEL        									 0
#define LOGICAL_CHANGE   									 1
#define FAILING         									 2
#define RISING       					      			     3
#define ENABLE_EXTERNAL_INTERRUPT_0(EVENT)                   MCUCR|=EVENT;\
SET_BIT(GICR,INT0)
#define ENTER_CHARACHTER 									'\r'
#define SPACE_CHARACHTER 									'  '
#define Nothing_passed   									 0
/*- LOCAL Dataypes ----------------------------------------*/

/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
void EXTERNAL_INTERRUPT_0_FUNC_ISR (void)__attribute((signal,used));
static void TIMR0_CTC_CALLBACK_FUNC_ISR(void);
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/**Time in milisecond as the interrupt is activated or every 1 milisecond***/
static volatile float64_t f64_Miliseconds=0U;
/***Declartion for Timer in seconds to print it with each switch press***/
static volatile float64_t f64_Time_In_Seconds=0U;
static volatile float64_t f64_Distances_In_M=0U;
static volatile float64_t f64_Velocity_In_M_PER_S=2U;
static volatile char_t sgc_updated_speed=0u;
/*- GLOBAL EXTERN VARIABLES -------------------------------*/

/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/

/* .........     ...................
   .       .     .                 .
   . HIGH  . LOW .      HIGH       .
----       -------                 -------------*/

/**This function transfer int data of Uart character by character***/
static void UART_TRANSFER_INT_DATA(uint64_t u64_Copy_INT_DATA)
{
	/***Varaible declaration for string will ocuppy the characters of Distance,Velocity and time**/
	char *pu8_INT_STRING=TransformIntToString(u64_Copy_INT_DATA);

	while(*pu8_INT_STRING)
	{
		UART_SendByte(*pu8_INT_STRING);
		pu8_INT_STRING++;
	}
}
/**This function print all measuring Data(Speed,Distance,time)**/
static void UART_PRINT_MEASURING_DATA(void)
{
	/**Firstly print the velocity value**/
	UART_SendByte('V');	
	UART_SendByte('=');	
	UART_TRANSFER_INT_DATA(f64_Velocity_In_M_PER_S);
	UART_SendByte(SPACE_CHARACHTER);
	UART_SendByte(',');	
	
	/**Secondly print the distance value**/
	UART_SendByte('D');
	UART_SendByte('=');	
	UART_TRANSFER_INT_DATA(f64_Distances_In_M);
	UART_SendByte(SPACE_CHARACHTER);
	UART_SendByte(',');	
	
	/**Secondly print the time value**/
	UART_SendByte('T');
	UART_SendByte('=');	
	UART_TRANSFER_INT_DATA(f64_Time_In_Seconds);
	UART_SendByte('.');

	UART_SendByte(ENTER_CHARACHTER);
}
	
/***The function that will execute in CTC Timer ISR***/
static void TIMR0_CTC_CALLBACK_FUNC_ISR(void)
{
    /***Incrementing miliseconds***/
	f64_Miliseconds++;
	/**Here we made the calculation of the distance in m**/
	f64_Distances_In_M+=(f64_Velocity_In_M_PER_S/1000.0);
	/**Measurment of time in seconds***/
	f64_Time_In_Seconds=f64_Miliseconds/1000.0;	
}
/***The function that will execute in External Interrupt ISR****/

void EXTERNAL_INTERRUPT_0_FUNC_ISR(void)
{

	/***Uart print measurment of data***/
	UART_PRINT_MEASURING_DATA();
	/***Then it let the values of Distance and time equal zero**/
	f64_Distances_In_M=0U;
	/* Time_In_Seconds=0U; */
	f64_Miliseconds=0U; 
}

void SPI_SLAVE_FUNCTION_ISR(void)
{
	 DIO_INIT_Pin(3,1);
	 DIO_toggle_Pin(3);		
	/***Update the value of Velocity through SPI SLAVE***/
	
}


int main()
{
	/**Initailize SPI by Spi Configuration**/
	SPI_Init(&spi);
	/**Initalize setcall back for spi ISR**/
	 /*SPI_SETCALL_BACK(SPI_SLAVE_FUNCTION_ISR);*/
	/**Initalization fo Timer**/
    Timer_Init(&Measurement_ECU_TIMER_Init_Config);
	/***Start the timer to count until it reaches 1 mili to measure at each mili the velocity and the new distance***/
	Timer_Start(TIMER0,Nothing_passed,TIMR0_CTC_CALLBACK_FUNC_ISR);
	/**Initalize UART by configuration structure***/
	UART_Init(&Measurement_ECU_TX_UART_CONFIG);
	/**Initalize GPIO pin as input for INT0**/
    DIO_INIT_Pin(INT0_PIN,INPUT);
	/**Initailize INT0 as external interrupt falling edge**/
	ENABLE_EXTERNAL_INTERRUPT_0(FAILING);

 
	while(1)
	{
		SPI_RecieveByte(&sgc_updated_speed);
	    f64_Velocity_In_M_PER_S=sgc_updated_speed;
	}
}


