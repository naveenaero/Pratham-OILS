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
 * hMaster_Torqtest.c
 *
 * Created: 07-04-2012 21:00:48
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
    if (tot_overflow >= 100)  // NOTE: '>=' is used
    {
        i++;
        tot_overflow = 0;     // reset overflow counter
        TCNT1 = 0;
        if(i==3)
            i=1;
        

    }
    
}


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
    
    transmit_UART0('H');
    transmit_UART0('e');
    transmit_UART0('l');
    transmit_UART0('l');
    transmit_UART0('o');
    
    sprintf(array,"\t..This is IITB's Student Satellite...\r");
    transmit_string_UART0(array);
    
    sprintf(array1,"\tThis is HUSSAIN's OBC-Master code...");
    sprintf(array2,"\rGenerating Torquer Current for =\t");
    
    /************************************************************/
    configure_torquer();
    
 
    float A[4] = {1,0.75,0.25,0.25};
    float B[4] = {0.25,0.5,0.75,1};
    float C[4] = {0.5,0.5,0.5,0.5};
    timer_init();
    
    /************************************************************/
    while(1)
        {
            
            transmit_UART0('H');
            transmit_UART0('\r');


        ///* * Magnetometer and Torquer test
        
        
        
        
        ///* * Reading with ALL torquer on at once, in one direction
        transmit_string_UART0(array2);
        transmit_UART0('+');
        transmit_UART0('X');
        transmit_UART0(' ');
        transmit_UART0('Y');
        transmit_UART0(' ');
        transmit_UART0('Z');
        transmit_UART0('\r');
        Current_state.pwm.x_dir = 0;
            Current_state.pwm.x = 32768*2*A[i];
        Current_state.pwm.y_dir = 0;
            Current_state.pwm.y = 32768;//*2*B[i];
        Current_state.pwm.z_dir = 0;
            Current_state.pwm.z = 32768;//*2*C[i];
        set_PWM ();
            
            
        ///* * Reading with one torquer on at once, in one direction
//        transmit_string_UART0(array2);
//        transmit_UART0('+');
//        transmit_UART0('X');
//        transmit_UART0('\r');
//        Current_state.pwm.x_dir = 0;
//        Current_state.pwm.x = 32768/2;
//        Current_state.pwm.y_dir = 0;
//        Current_state.pwm.y = 0;
//        Current_state.pwm.z_dir = 0;
//        Current_state.pwm.z = 0;
//        set_PWM ();
//
//            _delay_ms(5000);
//       PORTA = 0xA0;
//        _delay_ms(10000);
//        PORTA = 0x50;
//        _delay_ms(10000);
//        
//        transmit_string_UART0(array2);
//        transmit_UART0('+');
//        transmit_UART0('Y');
//        transmit_UART0('\r');
//        Current_state.pwm.x_dir = 0;
//        Current_state.pwm.x = 0;
//        Current_state.pwm.y_dir = 0;
//        Current_state.pwm.y = 32768;
//        Current_state.pwm.z_dir = 0;
//        Current_state.pwm.z = 0;
//        set_PWM ();
//        
//        PORTA = 0xA0;
//        _delay_ms(10000);
//        PORTA = 0x50;
//        _delay_ms(10000);
//        
//        transmit_string_UART0(array2);
//        transmit_UART0('+');
//        transmit_UART0('Z');
//        transmit_UART0('\r');
//        Current_state.pwm.x_dir = 0;
//        Current_state.pwm.x = 0;
//        Current_state.pwm.y_dir = 0;
//        Current_state.pwm.y = 0;
//        Current_state.pwm.z_dir = 0;
//        Current_state.pwm.z = 32768;
//        set_PWM ();
//        
//        PORTA = 0xA0;
//        _delay_ms(10000);
//        PORTA = 0x50;
//        _delay_ms(10000);
//        
//        ///* * Reading with ALL torquer on at once, in other direction
//        transmit_string_UART0(array2);
//        transmit_UART0('-');
//        transmit_UART0('X');
//        transmit_UART0(' ');
//        transmit_UART0('Y');
//        transmit_UART0(' ');
//        transmit_UART0('Z');
//        transmit_UART0('\r');
//        Current_state.pwm.x_dir = 1;
//        Current_state.pwm.x = 32768;
//        Current_state.pwm.y_dir = 1;
//        Current_state.pwm.y = 32768;
//        Current_state.pwm.z_dir = 1;
//        Current_state.pwm.z = 32768;
//        set_PWM ();
//        
//        PORTA = 0xA0;
//        _delay_ms(10000);
//        PORTA = 0x50;
//        _delay_ms(10000);
//        
//        ///* * Reading with one torquer on at once, in other direction
//        transmit_string_UART0(array2);
//        transmit_UART0('-');
//        transmit_UART0('X');
//        transmit_UART0('\r');
//        Current_state.pwm.x_dir = 1;
//        Current_state.pwm.x = 32768;
//        Current_state.pwm.y_dir = 0;
//        Current_state.pwm.y = 0;
//        Current_state.pwm.z_dir = 0;
//        Current_state.pwm.z = 0;
//        set_PWM ();
//        
//        PORTA = 0xA0;
//        _delay_ms(10000);
//        PORTA = 0x50;
//        _delay_ms(10000);
//        
//        transmit_string_UART0(array2);
//        transmit_UART0('-');
//        transmit_UART0('Y');
//        transmit_UART0('\r');
//        Current_state.pwm.x_dir = 0;
//        Current_state.pwm.x = 0;
//        Current_state.pwm.y_dir = 1;
//        Current_state.pwm.y = 32768;
//        Current_state.pwm.z_dir = 0;
//        Current_state.pwm.z = 0;
//        set_PWM ();
//        
//        PORTA = 0xA0;
//        _delay_ms(10000);
//        PORTA = 0x50;
//        _delay_ms(10000);
//        
//        transmit_string_UART0(array2);
//        transmit_UART0('-');
//        transmit_UART0('Z');
//        transmit_UART0('\r');
//        Current_state.pwm.x_dir = 0;
//        Current_state.pwm.x = 0;
//        Current_state.pwm.y_dir = 0;
//        Current_state.pwm.y = 0;
//        Current_state.pwm.z_dir = 1;
//        Current_state.pwm.z = 32768;  
//        set_PWM ();
//        
//        PORTA = 0xA0;
//        _delay_ms(10000);
//        PORTA = 0x50;
//        _delay_ms(10000);
//        
//        ///* * Set Torquer values to zero
//        reset_PWM();
//        
//        PORTA = 0xA0;
//        _delay_ms(500);
//        PORTA = 0x50;
//        _delay_ms(500);
        
        /************************************************************/
    }
    return 0;
}
/************************************************************/
/*				Main ends									*/
/************************************************************/
