#define F_CPU 16000000UL
#define circumference 20 // circumference of the wheel in cm
#define slits 8 // number of slits on the encoder wheel
#define omega_max (89*0.001*slits*4) // 89*10^-3*8*4 (time at max speed in seconds * number of slits * gear ratio)

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

int time_smoother[8];
int distributor = 0; // used for specifying the time smoother array elements
int time_sum = 0; //sum of intervals used to calculate a floating average time
int mode = 0; //driving mode. (0: stop, 1: go)
int path_distance = 100; // cm
int traveled_distance = 0; // cm
long elapsed_time = 0; // ms

float smoothed_time; // floating average arcoss 8 measurements of opco times
float duration = 20; // sec
float target_speed; // rotational speed in 1/sec or rev/sec
float measured_speed; // 1/sec or rev/sec

void distance(); // function for updating the traveled distance
void time(); // function for getting the optocoupler reading



int main(void)
{

  uart_init();   // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication
  DDRD |= 0x60;
  OCR0A = 255; // pulse width - change this to controle speed
  TCCR0A |= 0xA3;
  TCCR0B |= 0x05;
  TCCR1A = 0x00;
  DDRB &= ~0x01;
  PORTB |= 0x01;
  TCCR1B = (1 << ICNC1) | (1 << ICES1) | (1 << CS12) | (1 << CS10);

  target_speed = (path_distance/circumference)/duration; // in 1/sec or rev/sec

  if(1)
  { // set the driving mode based on user input (just hard code it to 1 for now)
    mode = 1;
  }

  while(mode == 1)
  {
    for(int i = 0; i < 8; i++)
    {
      time();
      if(target_speed < omega_max)
      {
        OCR0A = target_speed*88.54; // scaling factor to map 2.88 to 255 and everything inbetween
      }
      else
      {
        OCR0A = 255;
      }

      distance();
      
    }
    
    smoothed_time = time_sum/8;

    while (1)
    {
      time();

      smoothed_time = time_sum/8;
      measured_speed = 1/smoothed_time*slits; // 1/sec or rev/sec

      target_speed = (path_distance-traveled_distance)/(duration-elapsed_time/1000); // remainding distance / remainding time (elapsed time is converted to seconds)

      if(target_speed < omega_max && target_speed > 0)
      {
        OCR0A += (target_speed-measured_speed)*10; // add the difference in target and measured speed to the PWM value scaled by 10 (since we're expecting a difference with an order of 10^-2)
      }
      else if(target_speed > omega_max)
      {
        OCR0A = 255;
      }
      else
      {
        OCR0A = 0;
      }

      distance();

    }
  }
  
  return 0;
}

void time()
{
  unsigned int time;
  TIFR1 = (1 << ICF1);  // resets the timer interupt flag register so it no longer is on rising edge
  TCNT1 = 0;            // resets the timer
  time = ICR1;          // copies the interupt capture register into a local variable
  time_smoother[distributor] = time;
  time_sum += time_smoother[distributor]; // add time interval to time_sum for later use in the floating average
  elapsed_time += time; // update value of elapsed time since the first interrupt
  printf("\n%d", time); // prints out the time (needs to be changed to lcd)

  distributor++;
  if(distributor > 7){
    distributor = 0;
  }
}

void distance()
{
  traveled_distance += circumference/slits;
}
