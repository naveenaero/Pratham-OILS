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



int i=1;
volatile uint8_t tot_overflow;

void timer_init()
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



// TIMER1 overflow interrupt service routine
// called whenever TCNT1 overflows
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
    char array3[40];
    

    int16_t Bx;
    int16_t By;
    int16_t Bz;
    
    
    /// Blink LED to show that program is successfully running
     DDRA = 0xF0;
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
//    init_UART1();
//    
    
    /// Transmit "Hello" String
     transmit_UART0('\r');
     transmit_UART0('\r');
     transmit_UART0('H');
     transmit_UART0('e');
     transmit_UART0('l');
     transmit_UART0('l');
     transmit_UART0('o');
    
     sprintf(array,"\t..This is IITB's Student Satellite...\r");
     transmit_string_UART0(array);
    
     sprintf(array1,"\tThis is PRATHAM's OBC-Master code...");
     sprintf(array2,"\rCurrent MagnetoMeter state is =\t");
     sprintf(array3,"\rGenerating Torquer Current for =\t");
    
     transmit_string_UART0(array1);
//     transmit_string_UART0(array2);
//     transmit_string_UART0(array3);
    
    
    
    
    
    /// Initialise UART1 for Magnetometer Reception of Data and Transmission of Poll
     init_UART_MM();
    /// Configure the torquer to output the required current values
     configure_torquer();
    
    
    /// Define 3 strings for storing Magnetometer field values
     char sx[2];
     char sy[2];
     char sz[2];
    
//    transmit_string_UART0(array3);
//    transmit_UART0(' ');
//    transmit_UART0('X');
//    transmit_UART0(' ');
//    transmit_UART0('Y');
//    transmit_UART0(' ');
//    transmit_UART0('Z');
//    transmit_UART0('\r');
//    Current_state.pwm.x_dir = 0;
//    Current_state.pwm.x = 32768;
//    Current_state.pwm.y_dir = 0;
//    Current_state.pwm.y = 32768;
//    Current_state.pwm.z_dir = 0;
//    Current_state.pwm.z = 32768;
//    set_PWM();

    float A[4] = {1,0.75,0.50,0.25};
    timer_init();
    /// Start while loop
	   while (1)
       {
          if(tot_overflow >= 100)
          {
              i=i+1;
              if(i==4)
              {
                  i=1;
              }
              TCNT1 = 0;
              tot_overflow = 0;
              send_MM_cmd("*00P\r");
//
          }

           /// Receive and store Bx,By,Bz
           Bx=(int16_t)receive_MM();
           Bx=(Bx<<8);
           Bx &= 0xFF00;
           Bx|=(int16_t)receive_MM();
           
           By=(int16_t)receive_MM();
           By=(By<<8);
           By &= 0xFF00;
           By|=(int16_t)receive_MM();
           
           Bz=(int16_t)receive_MM();
           Bz=(Bz<<8);
           Bz &= 0xFF00;
           Bz|=(int16_t)receive_MM();
           
           /// Receive carriage return and ignore
           receive_MM();
           
           /// Transmit Carriage return
           transmit_UART0('\r');
           
//           transmit_string_UART0(array3);
//           transmit_UART0(' ');
//           transmit_UART0('X');
//           transmit_UART0(' ');
//           transmit_UART0('Y');
//           transmit_UART0(' ');
//           transmit_UART0('Z');
//           transmit_UART0('\r');
//           Current_state.pwm.x_dir = 0;
//           Current_state.pwm.x = 32768;
//           Current_state.pwm.y_dir = 0;
//           Current_state.pwm.y = 32768;
//           Current_state.pwm.z_dir = 0;
//           Current_state.pwm.z = 32768;
//           set_PWM();
//           
           if (Bx != 0x00 || By != 0x00 || Bz != 0x00)
           {
               
           /// Copy Bx,By,Bz into Strings for transmiting
//           transmit_string_UART0(array2);
           sprintf(sz,"%d",Bz);
//           sprintf(sy,"%d",By);
//           sprintf(sz,"%d",Bz);
           
           transmit_UART0('z');
           transmit_string_UART0(sz);
           transmit_UART0(' ');
           
           sprintf(sz,"%d",By);
           transmit_UART0('y');
           transmit_string_UART0(sz);
           transmit_UART0(' ');
       
           sprintf(sz,"%d",Bx);
           transmit_UART0('x');
           transmit_string_UART0(sz);
           transmit_UART0(' ');
           transmit_UART0('\r');
           
       
            
//           transmit_string_UART0(array3);
//           transmit_UART0(' ');
//           transmit_UART0('X');
//           transmit_UART0(' ');
//           transmit_UART0('Y');
//           transmit_UART0(' ');
//           transmit_UART0('Z');
//           transmit_UART0('\r');
           Current_state.pwm.x_dir = 0;
           Current_state.pwm.x = 32768*2*A[i];
           Current_state.pwm.y_dir = 0;
           Current_state.pwm.y = 327688*2*A[i];
           Current_state.pwm.z_dir = 0;
           Current_state.pwm.z = 32768*2*A[i];
           set_PWM();
               
              
//
            }
      
}
    return 0;
}



