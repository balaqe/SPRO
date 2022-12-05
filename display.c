#include <Arduino.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"

struct Car_config_t
{

    char Input_carSpeed;
    char Input_sec;

};



    char ReadBuffer[8];
    int counter = 5;
    int interval;
    int j=0;
    int avg;
    int total_time;

    struct Car_config_t Car_Build[8];











///////*************** Main ***************///////
int main() 
{    

    void Car_config(void);
    void CarStartup(void);
    void CarRun(void);
    void Button_scaner(int a);


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
    
    void Button_scaner(int a);

    if(ReadBuffer[0] == 0x65 &&  ReadBuffer[1] == 0x04 && ReadBuffer[2] == 0x06 && ReadBuffer[3] == 0x01){          // sets the number of intervals there are
        Button_scaner(4);
        interval = (int)ReadBuffer[0];
        
    }

    if (ReadBuffer[0] == 0x65 &&  ReadBuffer[1] == 0x03 && ReadBuffer[2] == 0x04 && ReadBuffer[3] == 0x01){         // checks if button *save* is pressed
        Button_scaner(8);


        Car_Build[j].Input_sec = ReadBuffer[0];               
        Car_Build[j].Input_carSpeed = ReadBuffer[4]; 
        
        
        if(j+1==interval)
        {
            printf("page 1%c%c%c",255,255,255);       // sends the user to main page because the interval settings is finnished
            for (int i = 0; i < j+1; i++)
            {
                avg += Car_Build[i].Input_carSpeed;         // colectes all the speed inputs and adds them together
                total_time += Car_Build[i].Input_sec;       // colectes all the time inputs and adds them together 
            }
            j   =   j + 1;
            avg = avg/j;                        // calulates the avrage speed
            printf("n0.val=%d%c%c%c",total_time,255,255,255);       // sends *Input_carSpeed* to the info colom
            printf("n1.val=%d%c%c%c",interval,255,255,255);       // sends *Input_carSpeed* to the info colom
            printf("x0.val=%d%c%c%c",avg,255,255,255);       // sends *Input_carSpeed* to the info colom
            j=0;

        }
        else
        {

            j++;  
        }
        
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

    void CarRun(void);


    if(ReadBuffer[0] == 0x65 &&  ReadBuffer[1] == 0x01 && ReadBuffer[2] == 0x02 && ReadBuffer[3] == 0x00)           //  Selected program 1 for the car to run on
    {

    counter = 5;                    // sets counter to 5 sec

    for (int i = 0; i < 6; i++)     // this does the count down on the screen
    {
        
        printf("n2.val=%d%c%c%c",counter,255,255,255);    //sends the vaible to the screen so it can show the count down
        counter--;                   // decreses the counter by 1
        printf("x0.val=%d%c%c%c",avg ,255,255,255);       // sends the input for sec to the screen 
        printf("n0.val=%d%c%c%c",total_time ,255,255,255);       // sends the input for sec to the screen 
        _delay_ms(1000);            // delay of 1 sec

    }

    CarRun();                       // Calls the fuction CarRun that Runs the car        
   
    }
}




///////*************** Run comand ***************///////
void CarRun()
{

    // while (timer < Input_sec){
    for (int i = 0; i < interval  ; i++)
    {
        for (int h = 1; h < Car_Build[i].Input_sec + 1 ; h++)         // for loop just temporery until we get the motor function working      
        {

            printf("z0.val=%d%c%c%c",h*6,255,255,255);        // sends the sec valu to the clock, it has to be *6 because one reveloution is 360
            printf("n0.val=%d%c%c%c",h,255,255,255);          // sends the sec valu to the interger n0 on the display
            printf("x0.val=%d%c%c%c",Car_Build[i].Input_carSpeed,255,255,255);   // sends the Carspeed to the dispay interger x0
            printf("z1.val=%d%c%c%c",Car_Build[i].Input_carSpeed,255,255,255);   // sends the Carspeed to the dispay interger z1
        
        
            _delay_ms(1000);                // delay of 1 sec
        
        }
    }

    total_time = 0;
    avg = 0;
    _delay_ms(2000);        // delay 2 sec
    printf("page 1%c%c%c",255,255,255);             // after car is done running sends the user to page 1

}
