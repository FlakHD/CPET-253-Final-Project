/*
*Name: Kenzie Moore
* Program: Electrical Engineering Technology
* Year: 3rd year
* Class: Microcontroller Systems
* Section: CPET 253
* Exercise: ICA 16
* Date : 3/16/2022
*/
#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"
#include "..\inc\Motor.h"


/**
 * main.c
 */



void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    Clock_Init48MHz(); // makes bus clock 48 MHz
    PinInit();
    TimerInit();


while (1){
    while (!(P1IN & 0x2)){ // while buton is pressed
        Servo_Motor(5999); // trigger pwm at 10% duty cycle

        } // while buton
    TA3CTL &= ~0x0030;  //stop the timer/pwm

    }//while 1
return;
}

