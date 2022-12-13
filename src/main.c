#define F_CPU 16000000UL
#define circumference 20 // cm
#define slices 8 // number of slices (holes and solid parts) on the encoder wheel
#define min_opto_time 356 // 89*4 (from measurements, using the 4/1 gear ratio)
#define PWM_scaling_factor 255/(circumference/(min_opto_time*0.000064*slices)) // calculate scaling factor for converting speed (cm/s) to a PWM value
#define snappiness 3 // factor setting how aggressively/smoofly the speed is updated

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "usart.h"

unsigned int opto();

int path_distance = 100; // cm
int duration = 25; // sec

long loops = 0;

float current_distance = 0; // cm
float elapsed_time = 0; // sec
float target_speed; // cm/sec (not necessary but makes the code easier to understand)
float measured_speed; // cm/sec
float prev_speed; // previous PWM (OCR0A) value for smoothing
float average_speed;
float speed_sum = 0;

int main(void){

  uart_init();   // open the communication to the microcontroller
  io_redirect(); // redirect input and output to the communication

  DDRD |= 0x60;
  TCCR0A |= 0xA3;
  TCCR0B |= 0x05;
  TCCR1A = 0x00;
  DDRB &= ~0x01;
  PORTB |= 0x01;
  TCCR1B = (1 << ICNC1) | (1 << ICES1) | (1 << CS12) | (1 << CS10);

  OCR0A = 255; // set speed to max to start with        OCR0A is the PWM variable (value 0-255)
  target_speed = ((float)path_distance)/duration;
  
  for(int i=0; i<slices; i++){ // take readings while spinning the motor
    measured_speed = ((float)slices/circumference)/(opto()*0.000064); // get measurements on the car's speed
    // intuitively it should be circumference / slices but it doesn't work that way... but it works inverted

    speed_sum += measured_speed;
    loops++;
  }
  prev_speed = measured_speed;

// go into the main while loop 

  while(current_distance <= path_distance){
    // opto();

    measured_speed = 0.05*((float)slices/circumference)/(opto()*0.000064) + 0.95*prev_speed; // update measured speed and smooth the change
    speed_sum += measured_speed;
    average_speed = speed_sum/loops; // calculate average speed

    current_distance = average_speed*elapsed_time;

    printf("\nMeasured speed: %.2f", measured_speed);

    // if(loops > slices){
    target_speed = (path_distance-current_distance)/(duration-elapsed_time); // calculate target speed based on remainding distance and time
    if(OCR0A + (target_speed-measured_speed)*snappiness < 255 && OCR0A + (target_speed-measured_speed)*snappiness > 0){
      OCR0A += (target_speed-measured_speed)*snappiness;
    }
    
      // OCR0A = 0.95*prev_speed + 0.05*target_speed*PWM_scaling_factor;
    // }
    printf("     Target speed: %.2f", target_speed);
    printf("     OCR0A: %d", OCR0A);

    prev_speed = target_speed;

    loops++;
  }
  OCR0A = 0;
  printf("Done");
  return 0;
}
unsigned int opto(){
  unsigned int opto_time; // 64 us

  if (TIFR1 & (1 << ICF1)){
    TIFR1 = (1 << ICF1);  // resets the timer interupt flag register so it no longer is on rising edge
    TCNT1 = 0;            // resets the timer

    opto_time = ICR1;     // copies the interupt capture register into a local variable
    elapsed_time += opto_time*0.000064;
    // printf("     Opto time: %d", opto_time); // prints out the time (needs to be changed to lcd)

    // current_distance += ((float)circumference)/slices; // update current distance by adding circumference / number of slices

    printf("      Elapsed time: %.2f", elapsed_time);
    
    printf("      Current distance: %.2f", current_distance);


    return opto_time;
  }
  
}
