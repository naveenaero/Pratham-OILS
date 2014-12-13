/* Name: main.c
 * Author: <insert your name here>
 * Copyright: <insert your copyright message here>
 * License: <insert your license reference here>
 My name is Anthoney Gonsalves
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


volatile uint8_t tot_overflow;

void timer0_init()
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



// TIMER0 overflow interrupt service routine
// called whenever TCNT0 overflows
ISR(TIMER1_OVF_vect)
{
    // keep a track of number of overflows
    tot_overflow++;
    if (tot_overflow >= 15)  // NOTE: '>=' is used
        {
            PORTA ^= 0xFF ;        // toggles the led
            tot_overflow = 0;     // reset overflow counter
        }

}



int main(void)
{
    timer0_init();
    while(1)
    {
        
        
    }
    
    return 0;   /* never reached */
}
