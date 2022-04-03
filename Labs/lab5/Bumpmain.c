/*
*Name: Kenzie Moore
* Program: Electrical Engineering Technology
* Year: 3rd year
* Class: Microcontroller Systems
* Section: CPET 253
* Exercise: Lab 5
* Date : 2/22/2022
*/
#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"
#include "..\inc\BumpInt.h"
#include "..\inc\Motor.h"


/**
 * main.c
 */

extern volatile bool        wasInterrupt;
extern volatile uint16_t        counts;
extern volatile uint8_t        direction;

void main(void)
{
	 WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
	 DisableInterrupts();
	 Clock_Init48MHz();
	 PinInit();
	 BumpInt_Init();
	 Motor_Init();
	 TimerInit();




	     //declare enumerated states, declare starting state, declare previous state, declare state timer

	    //declare boolean to know if state has switched

	    enum motor_states{off, forward, right, left, backward} state, prevState;
	    state = off;                   //start state
	    prevState = !off;               //used to know when the state has changed
	    uint16_t stateTimer;       //used to stay in a state
	    bool isNewState;           //true when the state has switched



	           while(1)
	           {
	                    isNewState = (state != prevState);
	                    prevState = state;  //save state for next time
	            switch (state) {
	                case off:
	                     state = forward;


	                     break;

	                  case forward:
	                     if (isNewState){
	                         stateTimer = 0;
	                     }
	                     Motor_Forward(14999, 14200);

	                     //stateTimer++;
	                     if(wasInterrupt){

	                         state = backward;
	                         wasInterrupt = false;
                             }


	                     break;

	                  case right:
	                      if (isNewState){
	                               stateTimer = 0;
	                                   }
	                       Motor_Right(14999,14200);
	                       Delay();//100ms delay
	                       Delay();
	                       Delay();
	                       Delay();
	                       state = forward;

	                      //stateTimer++;

	                      //if(stateTimer >=5 ) {//8 or 9


	                         //}
	                      break;

	                  case left:
	                      if (isNewState){
	                          stateTimer = 0;
	                      }
	                      Motor_Left(14999,14200);
                         // stateTimer++;
	                      Delay();
	                      Delay();
	                      Delay();
	                      Delay();
	                      state = forward;

                         //if(stateTimer >=5) {//8 or 9


                          //}
                          break;

	                  case backward:
	                       if (isNewState){
	                             stateTimer = 0;
	                         }

	                        Motor_Backward(14999,14200);
	                        Delay();
	                        Delay();
	                        Delay();
	                        Delay();
	                        Delay();



	                        if(direction == 0){
	                            state = left;
	                            wasInterrupt = false;

	                        }
	                        if(direction == 1) {
	                            state = right;
	                            wasInterrupt = false;
	                         }

	                      break;

	                  default: state = off;
	              } //switch
	              P6DIR |= 0x01; // p6.0 as out
	              Delay();
	              P6OUT ^= 0x01; // toggles p6.0 every 100 ms

	           } //while(1)




}//main

