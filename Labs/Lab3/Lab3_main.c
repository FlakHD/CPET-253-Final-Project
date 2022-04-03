/****************************************************************************************
         CPET253 Lab3 - State Machine Review and Motor Drivers

*Name: Kenzie Moore
* Program: Electrical Engineering Technology
* Year: 2nd year
* Class: Microcontroller Systems
* Section: CPET 253
* Exercise: Lab 2 Prelab
* Date : 1/16/2021
*

 This program uses a state machine to control the TI-RSLK robot to drive
 in a square

 It uses several function provided by TI as well as the Cortex SysTick
 peripheral
  •inc/Clock.c and inc/Cortex.c must be included
  •the function Clock_Init48MHz() makes the system clock 48MHz (period = 20.83 ns)
  •To use SysTick to create a delay, do the following
      -Set SysTick -> LOAD = the delay you want to create. Remember to multiply
       by 48 for each 1us of delay you want
      -Set SysTick -> VAL = 0 to start the count at 0
      -Set SysTick -> CTRL = 0x00000005 to enable the clock
      -Wait for 0x00010000 to be true to indicate time is up
      -See MSP432 datasheet for more information on SysTick

 To control the motors on the TI-RSLK robot, there are three outputs that need
 to be driven.
    :Pin    :Description            :Notes
    :=======:=======================:=========================
    : P5.5  : Right motor direction : 0=forwards, 1=backwards
    : P3.6  : Right motor sleep     : 0=sleep, 1=awake
    : P2.6  : Right motor PWM       : 0=stop, PWM signal = go
    : P5.4  : Left motor direction  : 0=forwards, 1=backwards
    : P3.7  : Left motor sleep      : 0=sleep, 1= awake
    : P2.7  : Left motor PWM        : 0=stop, PWM signal = go

 Functions in this code:
     -MotorInit(void) - enable the motor pins as outputs, put the motors to sleep
     -MotorForward(void) - set both motors to forward, use SysTick to create PWM
      with 10 ms period and 25% duty cycle
     -MotorBackward(void) - set both motors to backward, use SysTick to create PWM
      with 10 ms period and 25% duty cycle
     -MotorTurnRight(void) - set left motor to forward and right motor to sleep,
      use SysTick to create PWM with 10 ms period and 25% duty cycle
     -MotorTurnLeft(void) - set right motor to forward and left motor to sleep,
      use SysTick to create PWM with 10 ms period and 25% duty cycle
     -MotorStop(void) - nice but not required, used to stop the motors at end of square

The state machine has 4 states; forward, right, left, backward
use FSM to make a square: Forward, right turn 90 degrees, forward, right turn 90, ....
or backward, left turn 90 degrees, backward, left turn 90 degrees ......
*******************************************************************************************/

#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "..\inc\Clock.c"
#include "..\inc\CortexM.c"


//Constants for motor pins
#define RIGHT_MOT_DIR      0x20     //p5.5
#define RIGHT_MOT_SLEEP    0x40     //p3.6
#define RIGHT_MOT_PWM      0x40     //p2.6
#define LEFT_MOT_DIR       0x10     //p5.4
#define LEFT_MOT_SLEEP     0x80     //p3.7
#define LEFT_MOT_PWM       0x80     //p2.7

void MotorInit (void)
//This function sets the motor pins as outputs and puts the motors to sleep
{
    //set direction pins as outputs
    P5DIR |= RIGHT_MOT_DIR | LEFT_MOT_DIR;
    //set PWM pins as outputs
    P3DIR |= RIGHT_MOT_PWM | LEFT_MOT_PWM;
    //set sleep pins as outputs
    P2DIR |= RIGHT_MOT_SLEEP | LEFT_MOT_SLEEP;
    //put motors to sleep
    P3OUT &= ~RIGHT_MOT_SLEEP & ~LEFT_MOT_SLEEP;

    return;
}
void MotorStop (void)
//This function stops the motors by putting 0 on PWM pins and then puts
//motors to sleep
{
    P2OUT &= ~RIGHT_MOT_PWM & ~LEFT_MOT_PWM;       //stop motors
    P3OUT &= ~RIGHT_MOT_SLEEP & ~LEFT_MOT_SLEEP;   //put motors to sleep
    return;
}
void MotorForward (void)
//This function is used to drive both motors in the forward direction.
//It uses SysTick to create a PWM wave with a period of 10ms and 25% duty cycle
//The PWM signal is high for 2.5 ms and low for 7.5 ms
//Each time this function is called, one cycle of the PWM is output on the PWM pin
{
     P3OUT |= RIGHT_MOT_SLEEP | LEFT_MOT_SLEEP;  //wake up motors
     P5OUT &= ~RIGHT_MOT_DIR & ~LEFT_MOT_DIR;  //motors forward
     P2OUT |= RIGHT_MOT_PWM | LEFT_MOT_PWM;  //drive pins high for PWM
     // wait high time
     // since the clock is 48Mhz, every 48 counts is 1 us
     // high time of 2500 us is 25% duty cycle
         SysTick -> LOAD = 48 * 2500;  //2500us = 2.5ms
         SysTick -> VAL = 0;           //clear the count to 0
         SysTick -> CTRL = 0x00000005; //enable the timer
         while (!(SysTick -> CTRL & 0x00010000)); //wait for flag that time is up
     P2OUT &= ~RIGHT_MOT_PWM & ~LEFT_MOT_PWM;  //drive pins low for PWM
         // now low time
         SysTick -> LOAD = 48 * 7500;  //7.5ms
         SysTick -> VAL = 0;
         SysTick -> CTRL = 0x00000005;
         while (!(SysTick -> CTRL & 0x00010000));
     return;
}
void MotorBackward (void)
//This function is used to drive both motors in the backward direction.
//It uses SysTick to create a PWM wave with a period of 10ms and 25% duty cycle
//The PWM signal is high for 2.5 ms and low for 7.5 ms
//Each time this function is called, one cycle of the PWM is output on the PWM pin
{
P3OUT |= RIGHT_MOT_SLEEP | LEFT_MOT_SLEEP;  //wake up motors
P5OUT |= RIGHT_MOT_DIR | LEFT_MOT_DIR;  //motors forward
P2OUT |= RIGHT_MOT_PWM | LEFT_MOT_PWM;  //drive pins high for PWM
// wait high time
// since the clock is 48Mhz, every 48 counts is 1 us
// high time of 2500 us is 25% duty cycle
    SysTick -> LOAD = 48 * 2500;  //2500us = 2.5ms
    SysTick -> VAL = 0;           //clear the count to 0
    SysTick -> CTRL = 0x00000005; //enable the timer
    while (!(SysTick -> CTRL & 0x00010000)); //wait for flag that time is up
P2OUT &= ~RIGHT_MOT_PWM & ~LEFT_MOT_PWM;  //drive pins low for PWM
    // now low time
    SysTick -> LOAD = 48 * 7500;  //7.5ms
    SysTick -> VAL = 0;
    SysTick -> CTRL = 0x00000005;
    while (!(SysTick -> CTRL & 0x00010000));
return;
}

void MotorTurnRight (void)
//This function is used to drive left motor forward and right motor to sleep.
//It uses SysTick to create a PWM wave with a period of 10ms and 25% duty cycle
//The PWM signal is high for 2.5 ms and low for 7.5 ms
//Each time this function is called, one cycle of the PWM is output on the PWM pin
{
P3OUT |= RIGHT_MOT_SLEEP | LEFT_MOT_SLEEP;  //wake up motors
P5OUT |= RIGHT_MOT_DIR | ~LEFT_MOT_DIR;  //motors forward
P2OUT |= RIGHT_MOT_PWM | LEFT_MOT_PWM;  //drive pins high for PWM
// wait high time
// since the clock is 48Mhz, every 48 counts is 1 us
// high time of 2500 us is 25% duty cycle
    SysTick -> LOAD = 48 * 2500;  //2500us = 2.5ms
    SysTick -> VAL = 0;           //clear the count to 0
    SysTick -> CTRL = 0x00000005; //enable the timer
    while (!(SysTick -> CTRL & 0x00010000)); //wait for flag that time is up
P2OUT &= ~RIGHT_MOT_PWM & ~LEFT_MOT_PWM;  //drive pins low for PWM
    // now low time
    SysTick -> LOAD = 48 * 7500;  //7.5ms
    SysTick -> VAL = 0;
    SysTick -> CTRL = 0x00000005;
    while (!(SysTick -> CTRL & 0x00010000));
return;
}

void MotorTurnLeft (void)
//This function is used to drive right motor forward and left motor to sleep.
//It uses SysTick to create a PWM wave with a period of 10ms and 25% duty cycle
//The PWM signal is high for 2.5 ms and low for 7.5 ms
//Each time this function is called, one cycle of the PWM is output on the PWM pin
{
P3OUT |= RIGHT_MOT_SLEEP | LEFT_MOT_SLEEP;  //wake up motors
P5OUT |= ~RIGHT_MOT_DIR | LEFT_MOT_DIR;  //motors forward
P2OUT |= RIGHT_MOT_PWM | LEFT_MOT_PWM;  //drive pins high for PWM
// wait high time
// since the clock is 48Mhz, every 48 counts is 1 us
// high time of 2500 us is 25% duty cycle
    SysTick -> LOAD = 48 * 2500;  //2500us = 2.5ms
    SysTick -> VAL = 0;           //clear the count to 0
    SysTick -> CTRL = 0x00000005; //enable the timer
    while (!(SysTick -> CTRL & 0x00010000)); //wait for flag that time is up
P2OUT &= ~RIGHT_MOT_PWM & ~LEFT_MOT_PWM;  //drive pins low for PWM
    // now low time
    SysTick -> LOAD = 48 * 7500;  //7.5ms
    SysTick -> VAL = 0;
    SysTick -> CTRL = 0x00000005;
    while (!(SysTick -> CTRL & 0x00010000));
return;
}

void main(void)
{


       WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
       Clock_Init48MHz();  // makes bus clock 48 MHz
       MotorInit();

       //These are the four states of the state machine
       enum motor_states{off, forward, right, left, backward} state, prevState;

       state = off;                   //start state
       prevState = !off;               //used to know when the state has changed
       uint16_t stateTimer;       //used to stay in a state
       bool isNewState;           //true when the state has switched

       //through this while loop, every time one of the motor functions is called
       //it takes 10ms. Assume that the delay in each state is 10ms
       //time spent in any direction = stateTimer * 10ms
       while(1)
       {
           isNewState = (state != prevState);
           prevState = state;  //save state for next time
          

          switch (state) {
          //each case below should have entry housekeeping, state business and exit housekeeping
          //remember to reset the stateTimer each time you enter a new state
          //you must assign a new state when stateTer reaches the correct value
          case off:
             state = forward;


             break;

          case forward:
             if (isNewState){
                 stateTimer = 0;
             }
             MotorForward();

             stateTimer++;
           if(stateTimer >= 110) {

               state = right;
              }
             break;
          case right:
              if (isNewState){
                       stateTimer = 0;
                           }
              MotorTurnRight();

              stateTimer++;

              if(stateTimer >=45) {

                  state = forward;
                 }
              break;

          case left:
              if (isNewState){
                  stateTimer = 0;
              }
              MotorTurnLeft();
              stateTimer++;

              if(stateTimer >= 42) {

                  state = backward;
                 }
              break;

          case backward:
              if (isNewState){
                  stateTimer = 0;
              }

              MotorBackward();
              stateTimer++;

              if(stateTimer >= 180) {

                  state = left;
                 }
              break;
          default: state = off;
          } //switch
       } //while(1)
   } //main()
