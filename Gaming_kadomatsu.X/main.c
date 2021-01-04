/*
 * File:   main.c
 * Author: narua
 *
 * Created on 2021/01/01, 15:16
 */


#include <avr/io.h>
#include <avr/interrupt.h> 


#define Blue_decrease_status 2
#define Red_decrease_status 1
#define Yellow_decrease_status 0

volatile  unsigned int blue_LED    =   0xff;/*0*/
volatile unsigned int red_LED     =   0x00;/*1*/
volatile unsigned int yellow_LED  =   0xff;/*2*/

volatile int led_status  =   Yellow_decrease_status;




int main(void) {
    /* Replace with your application code */
    cli();  //innterrupt stop
        
        //TCNT2 = 0xFF;
    DDRB    =   0b00000111;/*o—Í‚ÉÝ’è*/
    PORTB   =   0xff;  

    TCCR0A  =   0b00000000 ;//
    TCCR0B  =   0b00000001 ;//prescalser 
    
    TIMSK  =   0b00000010;// innterrupt admmsion

    static unsigned int led_cycle           =   0;
    static unsigned int led_brink_status    =   0;
    
    sei();    // innterrupt start

/***************************/
/*main                  */
/***************************/
    
    while (1) {
        
        led_brink_status = (  (led_cycle > blue_LED))   *4 +
                            ( (led_cycle > red_LED ))   *2 +
                            ( (led_cycle > yellow_LED))  ;
        led_cycle++;
        PORTB=led_brink_status;
        if(led_cycle>250){
            led_cycle=0;
        }
        
        led_brink_status=0;
    }
}
ISR(TIMER0_OVF_vect) //timer0??????????????
{
    

    switch(led_status){
        case  Yellow_decrease_status:
          yellow_LED--;  
          red_LED++;
          if(yellow_LED == 0){
              led_status=Red_decrease_status;
          }
        break;
        case Red_decrease_status:
          red_LED--;  
          blue_LED++;
          if(red_LED == 0){
              led_status=Blue_decrease_status;
          }
        break;
        case Blue_decrease_status :
          blue_LED--;  
          yellow_LED++;
          if(blue_LED == 0){
              led_status=Yellow_decrease_status;
          }
        break;      
        default:
            blue_LED    =   0xff;/*0*/
            red_LED     =   0x00;/*1*/
            yellow_LED  =   0x00;/*2*/
            led_status=Blue_decrease_status;
            break;
    }
    //   TIFR0=0x01;
         
    return ;
}