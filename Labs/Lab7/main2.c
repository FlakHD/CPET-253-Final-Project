/*
*Name: Kenzie Moore
* Program: Electrical Engineering Technology
* Year: 3rd year
* Class: Microcontroller Systems
* Section: CPET 253
* Exercise: Lab 7
* Date : 3/20/2022
*/
#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"
#include "..\inc\Motor.h"
#include"..\inc\Reflectance.h"

uint8_t Data;
int32_t position;

void TimedPause(uint32_t time){
  Clock_Delay1ms(time);          // run for a while and stop
  Motor_Stop();

}




void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	Clock_Init48MHz(); // makes bus clock 48 MHz
	Motor_Init();
	TimerInit();
	Reflectance_Init();
	while(1){


	   } //while(1)

	} //main()




