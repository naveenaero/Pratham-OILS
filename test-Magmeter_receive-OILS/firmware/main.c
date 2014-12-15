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
#include "timer-oils.h"

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
//     DDRA = 0xF0;
//     PORTA = 0xF0;
//     _delay_ms(1000);
//     PORTA = 0x00;
//     _delay_ms(1000);
//     PORTA = 0xF0;
//     _delay_ms(1000);
//     PORTA = 0x00;
//     _delay_ms(1000);
    
    
    /// Inittialise UART0 for Transmission to terminal
     init_UART0();
    
    
    /// Transmit "Hello" String
     transmit_UART0('\r');
     transmit_UART0('\r');
     transmit_UART0('H');
     transmit_UART0('e');
     transmit_UART0('l');
     transmit_UART0('l');
     transmit_UART0('o');

    
     sprintf(array1,"\tThis is PRATHAM's OBC-Master code...");
     sprintf(array2,"\rCurrent MagnetoMeter state is =\t");
     transmit_string_UART0(array1);
     transmit_string_UART0(array2);
    
    /// Initialise UART1 for Magnetometer Reception of Data and Transmission of Poll
     init_UART_MM();
    /// Configure the torquer to output the required current values
//     configure_torquer();
    
    
    
    /// Define 3 strings for storing Magnetometer field values
     char sx[20];
     char sy[20];
     char sz[20];
    
    /// Initialise the Timer
    timer_init();
    
    /// Start while loop
	   while (1)
       {
            if(tot_overflow >= 15)         // find out what the value of x will be for a delay of 2 seconds
            {
               send_MM_cmd("*00P\r");

               /// Receive and store Bx,By,Bz
               Bx=(int16_t)receive_MM();
               Bx=(Bx<<8);
               Bx &= 0xFF00;
               Bx|=(int16_t)receive_MM();
               
               By=(int16_t)receive_MM();
               By=(By<<8); By &= 0xFF00;
               By|=(int16_t)receive_MM();
               
               Bz=(int16_t)receive_MM();
               Bz=(Bz<<8);
               Bz &= 0xFF00;
               Bz|=(int16_t)receive_MM();
               
               /// Receive carriage return and ignore
               receive_MM();
               
               /// Transmit Carriage return
               transmit_UART0('\r');
               
               if (Bx != 0x00 || By != 0x00 || Bz != 0x00)
               {
     
                   /// Copy Bx,By,Bz into Strings for transmiting
                   sprintf(sx,"%d",Bx);
                   sprintf(sy,"%d",By);
                   sprintf(sz,"%d",Bz);
                   
                   transmit_UART0('x');
                   transmit_string_UART0(sx);
                   transmit_UART0(' ');
                   
                   transmit_UART0('y');
                   transmit_string_UART0(sy);
                   transmit_UART0(' ');
                   
                   transmit_UART0('z');
                   transmit_string_UART0(sz);
                   transmit_UART0('\r');
                   
                   transmit_string_UART0("Hello this is Naveen from IITB");
                   transmit_UART0('\r');
               
               }
                
                tot_overflow = 0;
                TCNT1 = 0;
            }
           
           
       }
    return 0;  
}



