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
 * hMaster_MMtest.c
 *
 * Created: 31-03-2012 15:04:56
 *  Author: Hussain
 */


#include "common.h"
#include "uart.h"



/************************************************************/
/*				Main begins									*/
/************************************************************/
int main(void)
{
    
    
    /// Inittialise UART0 for Transmission to terminal
    init_UART0();
    /// Initialise UART1 for Magnetometer Reception of Data and Transmission of Poll
    init_UART1();

    char s[20];
    int8_t B[7];
    int8_t poll[5];
    int i;
    
    /// *00P\r : Actual Poll
    int8_t poll_actual[5] = {42,48,48,80,13};
    int k;
    
    /// Start while loop
	   while (1)
       {
//           DDRA = 0xF0;
//           PORTA = 0xF0;

           k=0;

           for(i=0;i<7;i++)
           {
               B[i] = receive_UART1();
           }
           
           for(i=0;i<5;i++)
           {
               poll[i] = receive_UART0();
           }
           
           for(i=0;i<5;i++)
           {
               if(poll[i] == poll_actual[i])
                   k++;
           }
           
           if(k==5)
           {
               
               sprintf(s,"%d",B[i]);
               transmit_string_UART0(s);
           }
       }
    
    return 0;  
}



