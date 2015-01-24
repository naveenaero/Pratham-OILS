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
//#include "timer.h"

uint64_t Time;
volatile struct state Current_state;

//int16_t Bx,By,Bz;
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
//    char array[40];
//    char array1[40];
//    char array2[40];

   
    
    
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
     transmit_UART0('H');
     transmit_UART0('e');
     transmit_UART0('l');
     transmit_UART0('l');
     transmit_UART0('o');

    
//     sprintf(array1,"\tThis is PRATHAM's OBC-Master code...");
//     sprintf(array2,"\rCurrent MagnetoMeter and Torquer state is =\t");
//     transmit_string_UART0(array1);
//     transmit_string_UART0(array2);
    
    /// Configure the Magnetometer
     init_UART_MM();
    
    /// Configure the Torquer
     configure_torquer();
    
    
    /// Define 3 strings for storing Magnetometer field values
     char sx[20];
     char sy[20];
     char sz[20];
    
    
    Time = 0;
//    timer_reset_teo_sec();
    
       while (1)
       {
           /// check for 2 seconds
//           if(tot_overflow >= 30)
//           {
//               Time += 2;
//               reset_PWM();
//               send_MM_cmd("*00P\r");
//               poll_MM();
//               TCNT1 = 0;
//               tot_overflow = 0;
//           }
           
           /// Read Magmeter every time Simulink sends something whether 0 or non-zero
//           read_MM();
           
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
               ////////////////////////////////////////////
               
               sprintf(sx,"%d",vg[0]);
               transmit_UART0('A');
               transmit_string_UART0(sx);
               transmit_UART0(' ');
               
               sprintf(sx,"%d",vg[1]);
               transmit_UART0('B');
               transmit_string_UART0(sx);
               transmit_UART0(' ');
               
               sprintf(sx,"%d",vg[2]);
               transmit_UART0('C');
               transmit_string_UART0(sx);
               transmit_UART0(' ');
               transmit_UART0('\r');
               
               ////////////////////////////
               
//               sprintf(sx,"%d",norm_vect);
//               transmit_UART0('D');
//               transmit_string_UART0(sx);
//               transmit_UART0(' ');
//               transmit_UART0('\r');
               
               ///////////////////////////
               
//               sprintf(sx,"%d",Bdot[0]);
//               transmit_UART0('P');
//               transmit_string_UART0(sx);
//               transmit_UART0(' ');
//               transmit_UART0('\r');
//
//               sprintf(sy,"%d",Bdot[1]);
//               transmit_UART0('Q');
//               transmit_string_UART0(sy);
//               transmit_UART0(' ');
//               transmit_UART0('\r');
//
//               sprintf(sz,"%d",Bdot[2]);
//               transmit_UART0('R');
//               transmit_string_UART0(sz);
//               transmit_UART0(' ');
//               transmit_UART0('\r');
//               
//               //////////////////////////
//               
//               sprintf(sx,"%d",temp[0]);
//               transmit_UART0('L');
//               transmit_string_UART0(sx);
//               transmit_UART0(' ');
//               transmit_UART0('\r');
//               
//               sprintf(sy,"%d",temp[1]);
//               transmit_UART0('M');
//               transmit_string_UART0(sy);
//               transmit_UART0(' ');
//               transmit_UART0('\r');
//               
//               sprintf(sz,"%d",temp[2]);
//               transmit_UART0('N');
//               transmit_string_UART0(sz);
//               transmit_UART0(' ');
//               transmit_UART0('\r');
//               
//               sprintf(sz,"%d",temp);
//               transmit_UART0('M');
//               transmit_string_UART0(sz);
//               transmit_UART0(' ');
//               transmit_UART0('\r');
               
               
               
//                              Current_state.pwm.x_dir = 0;
//               Current_state.pwm.y_dir = 0;
//               Current_state.pwm.z_dir = 0;
               
               
               /// Transmit Torquer Current Data to the Terminal
//               sprintf(sx,"%d",Current_state.pwm.x);
//               transmit_UART0('X');
//               transmit_string_UART0(sx);
//               transmit_UART0(' ');
//               
//               sprintf(sy,"%d",Current_state.pwm.y);
//               transmit_UART0('Y');
//               transmit_string_UART0(sy);
//               transmit_UART0(' ');
//               
//               sprintf(sz,"%d",Current_state.pwm.z);
//               transmit_UART0('Z');
//               transmit_string_UART0(sz);
//               transmit_UART0(' ');
//               transmit_UART0('\r');
               
               
//               
               
               
               
           }
           
           
           
       }
    return 0;  
}



