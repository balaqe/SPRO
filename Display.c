#include <Arduino.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

#define F_CPU 16000000UL
#define BAUD 9600

char ReadBuffer[100];
int carSpeed = 0;
int secounds = 0; 
int counter = 5;
int Interval = 0;








int main(void) 
{    

    
    uart_init(); // open the communication to the microcontroller
	io_redirect(); // redirect input and output to the communication

    printf("page 0%c%c%c",255,255,255);     //going to homepage on the screen so it know where it starts

    while(1) {

 
        CarStartup();                       //Calls a fuction carstartup

    }
        
    return 0;
}








int CarStartup()
{

    for(int i = 0; i<8;i++)                 // loop that has to go throw the Serial to check if there is a putton press
    {
        scanf("%c", &ReadBuffer[i]);        // Scanse the Serial port for info
            
        if(ReadBuffer[i-1] == 0x65 && ReadBuffer[i] == 0x01)           //  Selected program 1 for the car to run on
        {

            counter = 5;                    // sets counter to 5 sec

            for (int i = 0; i < 6; i++)     // this does the count down on the screen
            {
                    
                printf("page2.n2.val=%d%c%c%c",counter,255,255,255);    //sends the vaible to the screen so it can show the count down
                counter--;                  // decreses the counter by 1
                _delay_ms(1000);            // makes a delay of 1 sec

            }

            CarRun();                       // Calls the fuction CarRun that Runs the car
            continue;

        }
        
    }

}

int CarRun()
{
    carSpeed = 60;                  // sets the Carspeed for the car
    secounds = 30;                  // sets the sec for the interval
    Interval = 1;

    printf("page2.n1.val=%d%c%c%c",Interval,255,255,255);      // sets the Interval number to 1
    
    for (int i = 1; secounds+1 > i ; i++)               
    {

        printf("page2.z0.val=%d%c%c%c",i*6,255,255,255);        // sends the sec valu to the clock, it has to be *6 because one reveloution is 360
        printf("page2.n0.val=%d%c%c%c",i,255,255,255);          // sends the sec valu to the interger n0 on the display
        printf("page2.x0.val=%d%c%c%c",carSpeed,255,255,255);   // sends the Carspeed to the dispay interger x0
        printf("page2.z1.val=%d%c%c%c",carSpeed,255,255,255);   // sends the Carspeed to the dispay interger z1
        
        
        _delay_ms(1000);                // delay of 1 sec
        
    }
    carSpeed = 80;                  // sets the Carspeed for the car
    secounds = 20;                  // sets the sec for the interval
    Interval = 2;

    printf("page2.n1.val=%d%c%c%c",Interval,255,255,255);              // sets the interval nr too 2


    for (int i = 1; secounds+1 > i ; i++)
    {

        printf("page2.z0.val=%d%c%c%c",i*6,255,255,255);        // sends the sec valu to the clock, it has to be *6 because one reveloution is 360
        printf("page2.n0.val=%d%c%c%c",i,255,255,255);          // sends the sec valu to the interger n0 on the display
        printf("page2.x0.val=%d%c%c%c",carSpeed,255,255,255);   // sends the Carspeed to the dispay interger x0
        printf("page2.z1.val=%d%c%c%c",carSpeed,255,255,255);   // sends the Carspeed to the dispay interger z1
        
        
        _delay_ms(1000);// delay of 1 sec
        
    }

    _delay_ms(3000);                        // Makes a delay of 3 sec before finiching the program
    CarReset();
    printf("page 1%c%c%c",255,255,255);

    
}


int CarReset()
{

    carSpeed = 0;
    secounds = 0;
    Interval = 0;
    counter = 5;

}

int CrashTest()
{

    for(int i = 0; i<7; i++) 
    {
        scanf("%c", &ReadBuffer[i]);
        if(ReadBuffer[i] == 0x1A)//some error occurred - retrieve the 0xFF commands and start over
        {
            scanf("%c", &ReadBuffer[i]);
            scanf("%c", &ReadBuffer[i]);
            scanf("%c", &ReadBuffer[i]);
            continue;
        }
    }

}