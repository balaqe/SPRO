#include <Arduino.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include "usart.h"




struct Car_config_t
{

    char Input_carSpeed;
    int Input_sec;

};



/***Golbal varible*
 * 
 *all the varibles that needed in many functions are made here
 * 
*/
    struct  Car_config_t Car_Build[8];
    char    ReadBuffer[8];
    int     counter = 5;
    int     car_running = 0;
    int     curent_inteval = 0;
    int     total_time;
    int     timer = 0;
    int     sender = 0;
    int     interval;
    int     j =  0;
    int     i = 0;
    int     avg;








///////*************** Main ***************///////
int main() 
{    





// initialization 

    ADMUX   = ADMUX  | 0x40;           // sets A0 as
    ADCSRA  = ADCSRA | 0xE7;
    ADCSRB  = ADCSRB & (0xF8);

    
    TCCR1A  = 0;
    TCCR1B  = 0;
    TCCR1B |= (1<<CS12);        // sets prescaler 256
    TIMSK1 |= (1<<TOIE1);       // Enable timer overflow
    TCNT1   = 3036;             // sets the timer to exicute every sec : formula  16.000.000(Hz CPU) / (256(prescaler) * 1 (sec)) - 65535(16 bit) = 3036



    void    CarRun(void);
    void    Car_config(void);
    void    CarStartup(void);
    void    Button_scaner(int a);
    void    battery_info_sender(void);
    float   Battery_volt(void);
    int     time(void);
    int     Button_ID(int page, int ID, int event);

    uart_init();                    // open the communication to the microcontroller
	io_redirect();                  // redirect input and output to the communication
    




///////*************** Presetup ***************///////
    printf("page 0%c%c%c",255,255,255);         // every time when resets it send the use to main page
    _delay_ms(1000);                               // secure so analog has time to get its value




///////*************** Main Loop ***************///////
    while(Battery_volt() > 6) 
    {



        battery_info_sender();          // sends the battery info to the screen
        Button_scaner(7);               // scans for information from the screen
        Battery_volt();                 // checks the status with the battery
        Car_config();                   // configurates the car
        CarStartup();                   //Calls a fuction carstartup


    }

    printf("page 5%c%c%c",255,255,255);     // battery Warning

    return 0;
}







/* 
    * *Car Configuration*
    * 
    * this has comunication with the display and takes the needed
    * infomation for the display when the user has commited it
    *  
*/
void Car_config(void){
    



    void Button_scaner(int a);
    int Button_ID(int page, int ID, int event);




    if(Button_ID(0x04, 0x06, 0x01)){          // sets the number of intervals there are
        Button_scaner(4);
        interval = (int)ReadBuffer[0];
        
    }





    if (Button_ID(0x03, 0x04, 0x01)){         // checks if button *save* is pressed
        
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
            printf("n1.val=%d%c%c%c",interval,255,255,255);         // sends *Input_carSpeed* to the info colom
            printf("x0.val=%d%c%c%c",avg,255,255,255);              // sends *Input_carSpeed* to the info colom
            j=0;

        }
        else
        {
            j++;  
        }
        
    }

}







/* 
    * *Car start up*
    * 
    * this function makes the car ready for driving
    * first it checks if everthing is allright with the battery
    * and and everthing is up and running at it should!
    * Then it start a count down when the count down is finnished
    * it calls the Car run function that runs the car!
    * 
*/
void CarStartup(void)
{

    void    CarRun(void);
    void    battery_info_sender(void);
    float   Battery_volt(void);
    int     Button_ID(int page, int ID, int event);

    
    
    battery_info_sender();



    if(Button_ID(0x01, 0x02, 0x00))           //  Selected program 1 for the car to run on
    {

    counter = 5;                    // sets counter to 5 sec

    for ( i = 0; i < 6; i++)     // this does the count down on the screen
    {
        
        printf("n3.val=%d%c%c%c",counter,255,255,255);          // sends the vaible to the screen so it can show the count down
        counter--;                                              // decreses the counter by 1
        printf("x0.val=%d%c%c%c",avg ,255,255,255);             // sends the input for sec to the screen 
        printf("n0.val=%d%c%c%c",total_time ,255,255,255);      // sends the input for sec to the screen 
        if(Battery_volt() < 6.8){
            return;
        }
        _delay_ms(1000);                                        // delay of 1 sec

    }
    CarRun();                       // Calls the fuction CarRun that Runs the car        
   
    }
}




/*
    * *Run comand*
    * 
    * this function is the main running fuction!
    * when its called it will do a count down then start
    * first it will get the number of times it shall run from
    * the config function and polt that in the forloop and in that
    * forloop the engin is, that's where everything will happen, 
    * there the motor speed and everything will happen!
    * 
    * when finnished it will send the user to the main page again
    * 
*/
void CarRun(void)
{
    void battery_info_sender(void);

        battery_info_sender();
        TIFR1 = 0x01;                     // restarts the flagpoint for the timer to start


    for (int i = 0; i < interval  ; i++)
    {

        while(timer < Car_Build[i].Input_sec)         // for loop just temporery until we get the motor function working      
        {
            


            if (TIFR1&0x01)         // checks if there is a flag been
            {
                TIFR1 = 0x01;       // resets the flag
                timer++;            // increases the timer by 1
                
                /**** data for the display regarding the time ****/
                printf("z0.val=%d%c%c%c",timer*6,255,255,255);        // sends the sec valu to the clock, it has to be *6 because one reveloution is 360
                printf("n0.val=%d%c%c%c",timer,255,255,255);          // sends the sec valu to the interger n0 on the display


                /**** data for the display regarding the speed ****/
                printf("x0.val=%d%c%c%c",Car_Build[i].Input_carSpeed,255,255,255);   // sends the Carspeed to the dispay interger x0
                printf("z1.val=%d%c%c%c",Car_Build[i].Input_carSpeed,255,255,255);   // sends the Carspeed to the dispay interger z1
                
                
            }
        
        
        
        }

        timer = 0;

    }


    car_running = 0;
    total_time = 0;
    curent_inteval = 0;
    avg = 0;

    _delay_ms(2000);        // delay 2 sec
    printf("page 1%c%c%c",255,255,255);             // after car is done running sends the user to page 1

}


/*
    *
    * smaller functions
    *
    * when going futhere down you'll see
    *
    * all the smaller functions!
    *
    * the once to shorten the code so its easy to look at
    *
*/



/* 
 *    * Battery Voltage input from analog *    
 */
float Battery_volt(void){

    float adclow = ADCL;
    return (((adclow + ( (ADCH & 0x03) << 8 ))*5/1023)*5/3);
}




/*
 * * sends battery procentage to the nextion display *
 */
void battery_info_sender(void){

    float battery_Prosent = ((Battery_volt()-6)/2.333333)*100;      // calculate for the procentage
    printf("j0.val=%.0f%c%c%c",battery_Prosent,255,255,255);        // sends the procenst to the screen
    printf("n2.val=%.0f%c%c%c",battery_Prosent,255,255,255);        // sends the procenst to the screen

}



/* 
    * *this is only used for if functions, it only return a 1 or 0 statement* 
    *
*/
int Button_ID(int page, int ID, int event){

    if(ReadBuffer[0] == 0x65 &&  ReadBuffer[1] == page && ReadBuffer[2] == ID && ReadBuffer[3] == event){          // identifies what button is pressed
        return 1;
    }
    return 0;
}


/*
    * *************** Button scaner ***************
    *
    * type in what amount of characters you want to be scanded
    * Demo if you want 3 char scaned then type  Button_scaner(3)
    * then you will get at array of char in readBuffer
*/
void Button_scaner(int amount)
{

    for(int i = 0; i<amount;i++)                 // loop that has to go throw the Serial to check if there is a putton press
    {
        scanf("%c", &ReadBuffer[i]);        // Scanse the Serial port for info
    }


}