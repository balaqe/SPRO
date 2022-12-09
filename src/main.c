#define F_CPU 16000000UL

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

void time()
{
  unsigned int time;
  TIFR1 = (1 << ICF1);  // resets the timer interupt flag register so it no longer is on rising edge
  TCNT1 = 0;            // resets the timer
  time = ICR1;          // copies the interupt capture register into a local variable
  printf("\n%d", time); // prints out the time (needs to be changed to lcd)
}

void motor()
{
  int count = 0;
  while (count < 255)
  { // 255 is the time period - for the pwm calculator

    if (TIFR1 & (1 << ICF1))
    {         // checks if there is a rising edge AKA the optocoupler has been interrupted, if so it gets the time
      time(); // function for optocoupler
    }

    count = count + 1;
    if (count > OCR0A)
    {
      PORTD |= (1 << PD6); // sets a logic 1 to our pwm pin
      count = 0;
    }
    else
    {
      PORTD &= ~(1 << PD6); // sets a logic 0 to our pwm pin
    }
  }
}

int main(void)
{

  uart_init();   // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication
  DDRD |= 0x60;
  OCR0A = 255; // pulse width - change this to controle speed
  // for testing
  // int count = 220;
  // OCR0A = 220;
  // int count2 = 0;
  // OCR0A = 100;
  TCCR0A |= 0xA3;
  TCCR0B |= 0x05;
  TCCR1A = 0x00;
  DDRB &= ~0x01;
  PORTB |= 0x01;
  TCCR1B = (1 << ICNC1) | (1 << ICES1) | (1 << CS12) | (1 << CS10);

  while (1)
  {

    // for testing
    //  for (int i = 0; i < count; i++)
    //  {
    //      OCR0A = OCR0A + 1;
    //      count2 = count2 + 1;
    //      if (count2 > OCR0A)
    //      {
    //        PORTD |= (1 << PD6);
    //        count2 =0;
    //      }
    //      else {
    //        PORTD &= ~(1 << PD6);
    //      }
    //      _delay_ms(15);
    //  }

    motor(); // calls the motor function, made so its sohuld be easier to combine lcd code
  }
  return 0;
}