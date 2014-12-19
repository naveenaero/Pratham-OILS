#include "common.h"
#include "timer.h"
/**
 * @file timer.c
 * @brief Implement the 2-second Time frame for the main loop using a timer
 */

///@todo verify the timer code

void timer_reset_two_sec(void){
  ///@todo Confirm the configuration of the Timer
  
  ///* Clear Timer Compare Flag
  sbi(TIFR, OCF1A);
  
  ///* Stop Timer
  TCCR1B = 0x00;
  
  ///* Reset Count
  TCNT1H = 0;
  TCNT1L = 0;
  
  ///* Output compare match value 8 MHz clock
  OCR1A = TIMER_TWO_SEC;    /// Naveen: this is a compare value, if Timer counter (TCNT1) matches with this an interrupt will be called, this value is set such that it is equal to 2 seconds
  
  ///* Start Timer
  TCCR1A = 0x00;
  TCCR1B = 0x05;  /// Naveen : for prescalar 1024
  TCCR1C = 0x00;
    
}

void watch_dog(int time)
{
  wdt_disable();
  wdt_enable(time);
}

void timer_wait_reset(void)
{
  ///* Return when timer>2 seconds.
  while(!(TIFR & _BV(OCF1A)));
  ///* Restart timer
  timer_reset_two_sec();  
}
