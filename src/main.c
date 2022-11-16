#define F_CPU 16000000UL

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

int main(void)
{

  uart_init();   // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication
  DDRD |= 0x60;
  int count = 0;
  OCR0A = 220;
  //int count2 = 0;
  OCR0A = 0;
  TCCR0A |= 0xA3;
  TCCR0B |= 0x05;

  while (1)
  {

    // for (int i = 0; i < count; i++)
    // {
    //     OCR0A = OCR0A + 1;
    //     count2 = count2 + 1;
    //     if (count2 > OCR0A)
    //     {
    //       PORTD |= (1 << PD6);
    //       count2 =0;
    //     }
    //     else {
    //       PORTD &= ~(1 << PD6);
    //     }
    //     _delay_ms(15);
    // }
    
   while (count > 255)
   {
     count = count + 1;
     if (count > OCR0A)
     {
       PORTD |= (1 << PD6);
       count = 0;
     }
     else {
       PORTD &= ~(1 << PD6);
     }
      
   }
    
  }
  


  return 0;
}