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

#define MU_Sending_Velocity                                  1
#define MU_SEND_Velocity_CHARACTER                           2
#define MU_SEND_Velocity_EQUAL_CHARACTER                     3
#define MU_SEND_Velocity_DATA                                4
#define MU_SEND_Velocity_SPACE                               5
#define MU_SEND_Velocity_COMMA                               6
#define MU_Sending_Distance                                  7
#define MU_SEND_Distance_CHARACTER                           8
#define MU_SEND_Time_SPACE                                   9
#define MU_SEND_Distance_EQUAL_CHARACTER                     10
#define MU_SEND_Distance_DATA                                11
#define MU_SEND_Distance_SPACE                               12
#define MU_SEND_Distance_COMMA                               13
#define MU_Sending_Time                                      15
#define MU_SEND_Time_CHARACTER                               16
#define MU_SEND_Time_EQUAL_CHARACTER                         17
#define MU_SEND_Time_DATA                                    18
#define MU_SEND_Time_COMMA                                   19
#define MU_Sending_DOT                                       20
#define MU_Sending_Enter                                     21

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
static volatile uint64_t sgu64_updated_speed=2U;
static volatile uint8_t Mswitch_flag=0U;
static volatile uint8_t sgu8_SPI_Interrupt_Flag_Receive=0U;
static volatile uint8_t sgu8_MU_Switch_flag=0U;
static volatile uint8_t sgu8_UART_Interrupt_Flag_trans=0U;
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/

/* .........     ...................            .
   .       .     .                 .            .
   . HIGH  . LOW .      HIGH       .            .
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
	UART_TRANSFER_INT_DATA(sgu64_updated_speed);
	UART_SendByte(SPACE_CHARACHTER);
	UART_SendByte(',');	
	
	/**Secondly print the distance value**/
	UART_SendByte('D');
	UART_SendByte('=');	
	UART_TRANSFER_INT_DATA((uint64_t)f64_Distances_In_M );
	UART_SendByte(SPACE_CHARACHTER);
	UART_SendByte(',');	
	
	/**Thirdly print the time value**/
	UART_SendByte('T');
	UART_SendByte('=');	
	UART_TRANSFER_INT_DATA((uint64_t)f64_Time_In_Seconds);
	UART_SendByte('.');
 
	UART_SendByte(ENTER_CHARACHTER);
	
}				
/***The function that will execute in CTC Timer ISR***/
static void TIMR0_CTC_CALLBACK_FUNC_ISR(void)
{
    /***Incrementing miliseconds***/
	f64_Miliseconds++;
	/**Here we made the calculation of the distance in m**/
	f64_Distances_In_M+=(float64_t)(sgu64_updated_speed/1000.0);
	/**Measurment of time in seconds***/
	f64_Time_In_Seconds=f64_Miliseconds/1000.0;	
}
/***The function that will execute in External Interrupt ISR****/
void EXTERNAL_INTERRUPT_0_FUNC_ISR(void)
{
	/*UART_PRINT_MEASURING_DATA();*/
	/**Set MU switch flag**/
    sgu8_MU_Switch_flag=1U;

}
/***This is function is called back in ISR OF SPI Reception finished***/
void SPI_SLAVE_REC_FUNCTION_ISR(void)
{
/* 	DIO_INIT_Pin(3,1);
	DIO_toggle_Pin(3);	 */	
	/***Update the value of Velocity through SPI SLAVE***/
	SPI_RecieveByte(&sgu64_updated_speed);
}
/***This is function is called back in ISR OF UART Transmission finished***/
void UART_SLAVE_TRANS_FUNCTION_ISR(void)
{
	sgu8_UART_Interrupt_Flag_trans=1U;
/* 	DIO_INIT_Pin(0,1);
	DIO_toggle_Pin(0);  */	

}
int main()
{
	uint8_t *pu8_INT_STRING=NullPointer;
    uint8_t u8_MU_First_Byte_Trans_Flag=1U;
	/**Initalize MU states**/
    uint8_t u8_MU_General_State=MU_Sending_Velocity;
	uint8_t u8_MU_Sending_Velocity_State=MU_SEND_Velocity_CHARACTER;
	uint8_t u8_MU_Sending_Distance_State=MU_SEND_Distance_CHARACTER;
	uint8_t u8_MU_Sending_Time_State=MU_SEND_Time_CHARACTER;	
	/**Initailize SPI by SPI Configuration**/
	SPI_Init(&spi);
	/**Initalize setcall back for spi ISR**/
	SPI_Trans_SetCallBack(SPI_SLAVE_REC_FUNCTION_ISR);
	/**Initalization fo Timer**/
    Timer_Init(&Measurement_ECU_TIMER_Init_Config);
	/***Start the timer to count until it reaches 1 mili to measure at each mili the velocity and the new distance***/
	Timer_Start(TIMER0,Nothing_passed,TIMR0_CTC_CALLBACK_FUNC_ISR);
	/**Initalize UART by configuration structure***/
	UART_Init(&Measurement_ECU_TX_UART_CONFIG);
	/**Initalize setcall back for UART ISR**/
	UART_Trans_SetCallBack(UART_SLAVE_TRANS_FUNCTION_ISR);
	/**Initalize GPIO pin as input for INT0**/
    DIO_INIT_Pin(INT0_PIN,INPUT);
	/**Initailize INT0 as external interrupt falling edge**/
	ENABLE_EXTERNAL_INTERRUPT_0(FAILING);

 
	while(1)
	{

       /**Check if Measurment unit's switch flag is set**/
       if(1U==sgu8_MU_Switch_flag)
	   { 
			/**Print Velocity,Distance and Time**/
        	switch( u8_MU_General_State )
			{
				case MU_Sending_Velocity:
				{
					switch( u8_MU_Sending_Velocity_State )
					{
						case MU_SEND_Velocity_CHARACTER:
						{
							pu8_INT_STRING=TransformIntToString(sgu64_updated_speed);
                            if(u8_MU_First_Byte_Trans_Flag==1U)
							{
								UART_SendByte('V');		
                                u8_MU_First_Byte_Trans_Flag=0U;	
								u8_MU_Sending_Velocity_State=MU_SEND_Velocity_EQUAL_CHARACTER;									
							}
							else
							{
								if(sgu8_UART_Interrupt_Flag_trans==1U)
								{
									UART_SendByte('V');
									sgu8_UART_Interrupt_Flag_trans=0U;
									u8_MU_Sending_Velocity_State=MU_SEND_Velocity_EQUAL_CHARACTER;										
								}
								else
								{
								}								
							}


						}
						break;
						case MU_SEND_Velocity_EQUAL_CHARACTER:
						{
							
							if(sgu8_UART_Interrupt_Flag_trans==1U)
							{
								UART_SendByte('=');
								sgu8_UART_Interrupt_Flag_trans=0U;
								u8_MU_Sending_Velocity_State=MU_SEND_Velocity_DATA;										
							}									

						}
						break;								
						case MU_SEND_Velocity_DATA:
						{
     						if(*pu8_INT_STRING)
							{
								
								if(sgu8_UART_Interrupt_Flag_trans==1U)
								{
                                    UART_SendByte(*pu8_INT_STRING);									
									sgu8_UART_Interrupt_Flag_trans=0U;
									pu8_INT_STRING++;
								}																				
								
							}
							else 
							{ 								
								u8_MU_Sending_Velocity_State=MU_SEND_Velocity_SPACE;
 							} 
						}
						break;									
						case MU_SEND_Velocity_SPACE:
						{
							
							if(sgu8_UART_Interrupt_Flag_trans==1U)
							{	
                                UART_SendByte(SPACE_CHARACHTER);						
								u8_MU_Sending_Velocity_State=MU_SEND_Velocity_COMMA;								
								sgu8_UART_Interrupt_Flag_trans=0U;
							}										

							
						}
						break;								
						case MU_SEND_Velocity_COMMA:
						{
							
							if(sgu8_UART_Interrupt_Flag_trans==1U)
							{
								UART_SendByte(',');
								u8_MU_Sending_Velocity_State=MU_SEND_Velocity_CHARACTER;
								u8_MU_General_State=MU_Sending_Distance;																				
								sgu8_UART_Interrupt_Flag_trans=0U;										
							}																			
						}
						break;								
						default:
						{
							
						}
					}
				}
				break;					
				case MU_Sending_Distance:
				{
					switch( u8_MU_Sending_Distance_State )
					{
						case MU_SEND_Distance_CHARACTER:
						{
							pu8_INT_STRING=TransformIntToString((uint64_t)f64_Distances_In_M);	
							
							if(sgu8_UART_Interrupt_Flag_trans==1U)
							{
								UART_SendByte('D');
								sgu8_UART_Interrupt_Flag_trans=0U;
								u8_MU_Sending_Distance_State=MU_SEND_Distance_EQUAL_CHARACTER;										
							}		

						}
						break;						
						case MU_SEND_Distance_EQUAL_CHARACTER:
						{
							
							if(sgu8_UART_Interrupt_Flag_trans==1U)
							{
								UART_SendByte('=');
								u8_MU_Sending_Distance_State=MU_SEND_Distance_DATA;
								sgu8_UART_Interrupt_Flag_trans=0U;
							}	
							
						}
						break;							
						case MU_SEND_Distance_DATA:
						{																	
     						if(*pu8_INT_STRING)
							{
							    
								if(sgu8_UART_Interrupt_Flag_trans==1U)
								{
									UART_SendByte(*pu8_INT_STRING);	
									pu8_INT_STRING++;
									sgu8_UART_Interrupt_Flag_trans=0U;
								}																					
							}
							else 
							{ 
								u8_MU_Sending_Distance_State=MU_SEND_Distance_SPACE;
 							} 
							
						}
						break;								
						case MU_SEND_Distance_SPACE:
						{
							
							if(sgu8_UART_Interrupt_Flag_trans==1U)
							{
                                UART_SendByte(SPACE_CHARACHTER);								
								sgu8_UART_Interrupt_Flag_trans=0U;
								u8_MU_Sending_Distance_State=MU_SEND_Distance_COMMA;
							}																			
							
						}
						break;								
						case MU_SEND_Distance_COMMA:
						{
							
							if(sgu8_UART_Interrupt_Flag_trans==1U)
							{
								UART_SendByte(',');
								u8_MU_Sending_Distance_State=MU_SEND_Distance_CHARACTER;
								u8_MU_General_State=MU_Sending_Time;
								sgu8_UART_Interrupt_Flag_trans=0U;
							}										
							

						}
						break;								
						default:
						{
							
						}
					}
				}
				break;
				case MU_Sending_Time:
				{
						switch( u8_MU_Sending_Time_State )
						{
							case MU_SEND_Time_CHARACTER:
							{
								pu8_INT_STRING=TransformIntToString((uint64_t)f64_Time_In_Seconds);
								
								if(sgu8_UART_Interrupt_Flag_trans==1U)
								{
									UART_SendByte('T');	
									u8_MU_Sending_Time_State=MU_SEND_Time_EQUAL_CHARACTER;
									sgu8_UART_Interrupt_Flag_trans=0U;
								}																		
								
							}
							break;								
							case MU_SEND_Time_EQUAL_CHARACTER:
							{
									
								if(sgu8_UART_Interrupt_Flag_trans==1U)
								{
									UART_SendByte('=');
									u8_MU_Sending_Time_State=MU_SEND_Time_DATA;
									sgu8_UART_Interrupt_Flag_trans=0U;
								}	
								
							}
							break;								
							case MU_SEND_Time_DATA:
							{
								
  								if(*pu8_INT_STRING)
								{
									
									if(sgu8_UART_Interrupt_Flag_trans==1U)
									{
										UART_SendByte(*pu8_INT_STRING);	
										pu8_INT_STRING++;
										sgu8_UART_Interrupt_Flag_trans=0U;
									}	
									

								}
								else 
								{ 
									u8_MU_Sending_Time_State=MU_SEND_Time_SPACE;
        						} 
								
							}
							break;								
							case MU_SEND_Time_SPACE:
							{
								
								if(sgu8_UART_Interrupt_Flag_trans==1U)
								{
									UART_SendByte(SPACE_CHARACHTER);
									u8_MU_Sending_Time_State=MU_SEND_Time_CHARACTER;
									u8_MU_General_State=MU_Sending_DOT;										
									sgu8_UART_Interrupt_Flag_trans=0U;
								}																			

							}
							break;								
							default:
							{
								
							}
						}
				}
				break;
				case MU_Sending_DOT:
				{
					
					if(sgu8_UART_Interrupt_Flag_trans==1U)
					{
						UART_SendByte('.');
						u8_MU_General_State=MU_Sending_Enter;
						sgu8_UART_Interrupt_Flag_trans=0U;
					}												
					
				}
				break;
				case MU_Sending_Enter:
				{
					if(sgu8_UART_Interrupt_Flag_trans==1U)
					{
						UART_SendByte(ENTER_CHARACHTER);
						/**Reset Flag of switch**/
						sgu8_MU_Switch_flag=0U;		  
						/***let the values of Distance and time equal zero**/
						f64_Distances_In_M=0U;
						/* Time_In_Seconds=0U; */
						f64_Miliseconds=0U;
						u8_MU_General_State=MU_Sending_Velocity;
						u8_MU_Sending_Velocity_State=MU_SEND_Velocity_CHARACTER;
						u8_MU_Sending_Distance_State=MU_SEND_Distance_CHARACTER;
						u8_MU_Sending_Time_State=MU_SEND_Time_CHARACTER;
						sgu8_UART_Interrupt_Flag_trans=0U;							
						
					}													

				}
				break;					
				default:
				{
				}
			}
	   }
	   else
	   {
		   
	   } 

	}
}



			