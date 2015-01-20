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
#include "control.h"


volatile struct state Current_state;


/************************************************************/
/*				Main begins									*/
/************************************************************/
int main(void)
{
    
    /// Blink LED to show that program is successfully running
     DDRA = 0xF0;
     PORTA = 0xF0;
     _delay_ms(500);
     PORTA = 0x00;
     _delay_ms(500);
     PORTA = 0xF0;
     _delay_ms(500);
     PORTA = 0x00;
     _delay_ms(500);
    
    
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
     sprintf(array2,"\rCurrent MagnetoMeter and Torquer state is =\t");
     transmit_string_UART0(array1);
     transmit_string_UART0(array2);
    
    /// Configure the Magnetometer
     init_UART_MM();
    
    /// Configure the Torquer
     configure_torquer();
    
    
    /// Define 3 strings for storing Magnetometer field values
     char sx[20];
     char sy[20];
     char sz[20];
    
    while (1)
        {
           /// Receive Magnetic field everytime
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
           
           /// when atleast one of the values is non-zero, execute the control Law
           if (Bx !=0x00 || By != 0x00 || Bz != 0x00)
           {
               /// Apply control Law
               control();
               
               /// Transmit Magnetic field Data to terminal
               sprintf(sx,"%d",Bx);
               transmit_UART0('X');
               transmit_string_UART0(sx);
               transmit_UART0(' ');
               
               sprintf(sy,"%d",By);
               transmit_UART0('Y');
               transmit_string_UART0(sy);
               transmit_UART0(' ');
               
               sprintf(sz,"%d",Bz);
               transmit_UART0('Z');
               transmit_string_UART0(sz);
               transmit_UART0(' ');
               transmit_UART0('\r');
               
               
               
           
               
               
               /// Transmit Torquer Current Data to the Terminal
               sprintf(sx,"%d",Current_state.pwm.x);
               transmit_UART0('X');
               transmit_string_UART0(sx);
               transmit_UART0(' ');
               
               sprintf(sy,"%d",Current_state.pwm.y);
               transmit_UART0('Y');
               transmit_string_UART0(sy);
               transmit_UART0(' ');
               
               sprintf(sz,"%d",Current_state.pwm.z);
               transmit_UART0('Z');
               transmit_string_UART0(sz);
               transmit_UART0(' ');
               transmit_UART0('\r');
               
               
               
               
           }
           
           
           
       }
    return 0;  
}



