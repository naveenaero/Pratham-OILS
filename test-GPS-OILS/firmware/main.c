/****************************************************************************/
/* 					PRATHAM - IITB's Student Satellite                      */
/*                                                                          */
/* Microcontroller:                                                         */
/*          Atmel AVRmega128L                                               */
/* Written for the AVRStudio5 compiler                                      */
/*                                                                          */
/* Author:  Hussain Manasawala, MTech in Electronic Systems, IITB           */
/*                                                                          */
/* Contact: husainmanasa@ee.iitb.ac.in                                      */
/*                                                                          */
/****************************************************************************/
/*
 * hMaster_GPStest.c
 *
 * Created: 07-04-2012 04:56:21
 *  Author: Hussain
 */

#include "common.h"
#include "uart.h"
#include "gps.h"
#include "peripherals.h"

/**
 * @defgroup global_state Global State parameters
 */
//@{
volatile int8_t GPS_done = -1;
uint8_t Mode;
uint64_t Time;
volatile struct state Current_state;
uint8_t i;
//@}

/************************************************************/
/*				Main begins									*/
/************************************************************/
int main(void)
{
	char array[40];
	char array1[40];
	char array2[40];
	
	DDRA = 0xF0;
	PORTA = 0xF0;
	_delay_ms(1000);
	PORTA = 0x00;
	_delay_ms(1000);
	PORTA = 0xF0;
	_delay_ms(1000);
	PORTA = 0x00;
	_delay_ms(1000);
	
	init_UART0();
  
	transmit_UART0('\r');
	transmit_UART0('\r');
	transmit_UART0('H');
	transmit_UART0('e');
	transmit_UART0('l');
	transmit_UART0('l');
	transmit_UART0('o');
	
	sprintf(array,"\t..This is IITB's Student Satellite...\r");
	transmit_string_UART0(array);

	sprintf(array1,"\tThis is HUSSAIN's OBC-Master code...");
	sprintf(array2,"\rCurrent GPS state is =\t");
		
/************************************************************/
	init_UART_GPS();
	
	///* Switch on Global interrupts
	sei();
  
    //uint8_t GPS_data[159];
    uint8_t temp;
/************************************************************/
	while(1)
    {
        
        while (!(UCSR0A & _BV(RXC0)));
        temp =  UDR0;
        transmit_UART0('A');

        
        
//        for (i=0;i<159;i++)
//        {
//            GPS_data[i] = receive_GPS();
//            
//        }
//        
//        for(i=0;i<4;i++)
//        {
//            temp = temp >> 8;
//            temp &= 0x00FFFFFF;
//            temp |= ((uint32_t) GPS_data[i])<<24;
//            
//        }
//        sprintf(array,"x = %lu \n",temp);
//        transmit_string_UART0(array);
        
        
    //process_GPS();
    //	read_GPS();
    //while(UCSR0B & _BV(RXCIE0));
    
//	send_preflight((char *)&Current_state.gps, sizeof(struct GPS_reading));
//	sprintf(array,"x = %lu \n",Current_state.gps.x);
//    transmit_string_UART0(array);
//        
//	sprintf(array,"y = %lu \n",Current_state.gps.y);
//    transmit_string_UART0(array);
//        
//	sprintf(array,"z = %lu \n",Current_state.gps.z);
//    transmit_string_UART0(array);

//	sprintf(array,"x = %x\n",Current_state.gps.x);transmit_string_UART0(array);
//	sprintf(array,"y = %x\n",Current_state.gps.y);transmit_string_UART0(array);
//	sprintf(array,"z = %x\n",Current_state.gps.z);transmit_string_UART0(array);	
	
//    sprintf(array,"%lu ",Current_state.gps.v_x);
//    transmit_string_UART0(array);
//        
//	sprintf(array,"%lu ",Current_state.gps.v_y);
//    transmit_string_UART0(array);
//        
//	sprintf(array,"%lu ",Current_state.gps.v_z);
//    transmit_string_UART0(array);
	
/************************************************************/
	}
	return 0;
}
/************************************************************/
/*				Main ends									*/
/************************************************************/
