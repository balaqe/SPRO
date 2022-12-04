#include <Arduino.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"


#define F_CPU 16000000UL
#define BAUD 9600

char ReadBuffer[8];
char Input_carSpeed;
char Input_sec; 
int counter = 5;



///////*************** Main ***************///////
int main(void) 
{    

    
    uart_init(); // open the communication to the microcontroller
	io_redirect(); // redirect input and output to the communication
    printf("page 0%c%c%c",255,255,255);

    
    
    while(1) {
        
        

        Button_scaner(7);
        Car_config();                   // configurates the car
        CarStartup();                   //Calls a fuction carstartup

    }
    return 0;
}














///////*************** config saver ***************///////
void Car_config(){
    
    if (ReadBuffer[0] == 0x65 &&  ReadBuffer[1] == 0x03 && ReadBuffer[2] == 0x05 && ReadBuffer[3] == 0x01){         // checks if button *save* is pressed
        Button_scaner(8);

        Input_sec = ReadBuffer[0];
        Input_carSpeed = ReadBuffer[4];

    }
    if (ReadBuffer[0] == 0x65 &&  ReadBuffer[1] == 0x03 && ReadBuffer[2] == 0x04 && ReadBuffer[3] == 0x00){         // checks if button *Back* is pressed

        printf("x0.val=%d%c%c%c",Input_carSpeed,255,255,255);       // sends *Input_carSpeed* to the info colom
        printf("n0.val=%d%c%c%c",Input_sec,255,255,255);       // sends *Input_sec* to the info colom
    }    
    

}


















///////*************** Button scaner ***************///////
void Button_scaner(int a){          


    for(int i = 0; i<a;i++)                 // loop that has to go throw the Serial to check if there is a putton press
    {
        scanf("%c", &ReadBuffer[i]);        // Scanse the Serial port for info
    }


}










///////*************** Car startup ***************///////
void CarStartup()
{

    if(ReadBuffer[0] == 0x65 &&  ReadBuffer[1] == 0x01 && ReadBuffer[2] == 0x02 && ReadBuffer[3] == 0x00)           //  Selected program 1 for the car to run on
    {

    counter = 5;                    // sets counter to 5 sec

    for (int i = 0; i < 6; i++)     // this does the count down on the screen
    {
        printf("n2.val=%d%c%c%c",counter,255,255,255);    //sends the vaible to the screen so it can show the count down
        counter--;                   // decreses the counter by 1
        printf("n0.val=%d%c%c%c",Input_sec,255,255,255);       // sends the input for sec to the screen 
        _delay_ms(1000);            // delay of 1 sec

    }

    CarRun();                       // Calls the fuction CarRun that Runs the car        
   
    }
}










///////*************** Run comand ***************///////
void CarRun()
{

    // while (timer < Input_sec){
    for (int i = 1; Input_sec + 1 > i ; i++)         // for loop just temporery until we get the motor function working      
    {

        printf("z0.val=%d%c%c%c",i*6,255,255,255);        // sends the sec valu to the clock, it has to be *6 because one reveloution is 360
        printf("n0.val=%d%c%c%c",i,255,255,255);          // sends the sec valu to the interger n0 on the display
        printf("x0.val=%d%c%c%c",Input_carSpeed,255,255,255);   // sends the Carspeed to the dispay interger x0
        printf("z1.val=%d%c%c%c",Input_carSpeed,255,255,255);   // sends the Carspeed to the dispay interger z1
        
        
        _delay_ms(1000);                // delay of 1 sec
        
    }
    
    _delay_ms(2000);        // delay 2 sec
    printf("page 1%c%c%c",255,255,255);             // after car is done running sends the user to page 1

}
