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
#include "mag.h"
#include "peripherals.h"
#include <string.h>


void transmit_float(float f)
{
    unsigned char *ptr;
    int i=0;
    ptr = (unsigned char*)&f;
    for(i=0;i<sizeof(float);transmit_UART0(*(ptr+i)),i++);
    
}

void OutputFloat(char *address)
{
    int i=0;
    for(i=0;i<4;i++)
    transmit_UART0(*(address+i));
    
}

volatile uint8_t tot_overflow;

void timer_init(void)
{
    // set up timer with prescaler = 256
    TCCR1B |= (1 << CS11);
    
    // initialize counter
    TCNT1 = 0;
    
    // enable overflow interrupt
    TIMSK |= (1 << TOIE1);
    
    // enable global interrupts
    sei();
    
    // initialize overflow counter variable
    tot_overflow = 0;
}

ISR(TIMER1_OVF_vect)
{
    // keep a track of number of overflows
    tot_overflow++;
    
}

/************************************************************/
/*				Main begins									*/
/************************************************************/
int main(void)
{
    
    /// Define Variables to store Magnetic field
    char array[40];
    char array1[40];
    char array2[40];

    int16_t Bx;
    int16_t By;
    int16_t Bz;
    
    
    /// Blink LED to show that program is successfully running
     DDRA = 0xFF;
     PORTA = 0xF0;
     _delay_ms(1000);
     PORTA = 0x00;
     _delay_ms(1000);
     PORTA = 0xF0;
     _delay_ms(1000);
     PORTA = 0x00;
     _delay_ms(1000);
    
    
    /// Inittialise UART0 for Transmission to terminal
     init_UART0();
    
    
    /// Transmit "Hello" String
//     transmit_UART0('\r');
//     transmit_UART0('\r');
//     transmit_UART0('H');
//     transmit_UART0('e');
//     transmit_UART0('l');
//     transmit_UART0('l');
//     transmit_UART0('o');

    
     sprintf(array1,"\tThis is PRATHAM's OBC-Master code...");
     sprintf(array2,"\rThis code is being used for testing AVR-timer with UART\t");
//     transmit_string_UART0(array1);
//     transmit_string_UART0(array2);
    
    /// Initialise UART1 for Magnetometer Reception of Data and Transmission of Poll
     init_UART_MM();
   
    
    
    /// Define 3 strings for storing Magnetometer field values
     char sx[20];
     char sy[20];
     char sz[20];
    
    /// Initialise the Timer
    timer_init();
    //char s[10];
    
    double b = 65.45;
    int j=0;
    char s[8];
    
    char *ptr;
    
    int64_t g = 65000;
    ptr = (char*)&b;
//    sprintf(s,"%d",g);
    /// Start while loop
	   while (j<1)
       {
            if(tot_overflow >= 10)         // find out what the value of x will be for a delay of 2 seconds
            {
//               transmit_float(b);
//                transmit_string_UART0(s);
//                transmit_UART0('\r');
                OutputFloat(ptr);
                PORTA ^= 0xFF;
                tot_overflow = 0;
                TCNT1 = 0;
                j++;
            }
           
           
       }
    return 0;  
}



