/*
 *Name: Kenzie Moore
 * Program: Electrical Engineering Technology
 * Year: 3rd year
 * Class: Microcontroller Systems
 * Section: CPET 253
 * Exercise: Lab 8
 * Date : 3/28/2022
 */

#include "msp.h"

#include <msp432.h>

#include <stdint.h>

#include <stdbool.h>

#include "../inc/Clock.h"

#include "../inc/CortexM.h"

#include "../inc/motor.h"

#define TRIGGER 0x04
#define ECHO 0x08

#define microsecondsToClockCycles(a)((a) * 1.5) //assume 12Mhz clock divided by 8
#define clockCyclesToMicroseconds(a)((a) / 1.5) // 1.5 clock cycles = 1us

void TimerInit(void);
void Servo(void);

uint32_t pulseIn(void);
void UltraSonicInit(void) {
  P6DIR |= 0x4; //make P6.2 an output for US trigger
  P6DIR &= ~0x8; //make P6.3 an input for US echo
  P6SEL1 &= ~0xC; //put both pins in normal mode
  P6SEL0 &= ~0xC;
  return;
}
void ServoInit(void)
{
       P8DIR |= 0x4; // MAKE 8.2 OUTPUT
       P8SEL1 &= ~0x4; //select alternate IO mode
       P8SEL0 |= 0x4;//select alternate IO mode

       TA3CTL &= ~0x0030;  //stop the timer
       TA3CTL |= 0x0200;  TA3CTL &= ~0x0100;//choose SMCLK for the clock source
       TA3CCTL2 |= 0x00E0; //Outmode 7: reset/set
       TA3CTL |= 0x0080; TA3CTL &= ~0x0040;//choose clock divider of 4
       //start pwm
       TA3CCR0 = 59999;
       TA3CCR2 = 5999; //20% duty cycle
       TA3CTL |= 0x0010; //SET TIMER FOR UP MODE - this starts it;     //make P8.2 an output : output from Timer3.2
       //set SEL1 and SEL0 for pwm function

      //call Servo() function to center servo
     //stop the timer
    return;
}
void Servo_Motor(uint16_t Duty) {

  // turn on PWM and set duty cycle
  TA3CTL |= 0x0010; // Control bits 5 and 4 are mode control 00 to stop, 01 for up counting
  // bits 7 and 6 are clock divider 01 = /2
  // bits 9 and 8 choose clock 10 = SMCLK

  // Counter, start at zero once turned on
  TA3CCR2 = Duty; // Capture/Compare 2 COMPARE MODE : holds value for comparison to timer TA3R

  // - START PWM

  P8OUT |= 0x4; //PWML on P8.2 (EN)

  return;
}
int32_t rightDist;
int32_t leftDist;

const double left[7] = {4499, 4999, 5499, 5999, 6499, 6999, 7499};
const double right[7] = {4499, 3999,3499,2999, 2499, 1999, 1499};
void servoLeft(void) {
  int16_t i;
  ServoInit();

  for (i = 0; i < 7; i++) {
    Clock_Delay1ms(100);
    Servo_Motor(left[i]);
    Clock_Delay1ms(100);
  }

  //set period for 20ms
  //set high time for the input angle
  //set timer for up mode
  return;

}
void servoRight(void) {
  int16_t i;
  ServoInit();
  for (i = 0; i <7; i++) {
    Clock_Delay1ms(100);
    Servo_Motor(right[i]);
    Clock_Delay1ms(100);
  }


  return;
}


uint32_t distanceInCm(void) {
  uint32_t distance;

  P6OUT |= 0x4; //drive trigger pin high
  Clock_Delay1us(10); //wait 10 us
  P6OUT &= ~0x4; //drive trigger pin low
  distance = ((pulseIn() * 0.034) / 2); //calculate distance using s=t * 0.034/2. t comes from pulseIn() function
  if (distance >= 400) {
    distance = 0; // if no echo (distance = 0), assume object is at farthest distance
  }

  return distance;//return the distance
}
uint32_t pulseIn(void) {
  uint16_t width = 0;
  uint16_t time = 0;
  uint16_t maxcount = 56999; //max count for 38 ms (timeout)
  TA2CTL |= 0x0020; //set timer for continuous mode

  TA2R = 0; //reset the count
  while (!(P6IN & 0x8)) {} //wait for the pulse to start (while Echo is low)
  if (TA2R > maxcount) {
    return 0;
  }
  //if count is greater than maxcount return 0

  TA2R = 0; //reset the count
  while ((P6IN & 0x8)) {} //wait for the pulse to finish (while Echo is high)
  if (TA2R > maxcount) {
    return 0;
  } //if count is greater than maxcount return 0

  width = TA2R; //read the count (width of the return pulse)
  TA2CTL &= ~0x0030; //stop the timer
  time = clockCyclesToMicroseconds(width); // convert the reading to microseconds.
  return time; //return the microsecond reading
}


void main(void) {

  uint16_t rightDistance, leftDistance, distance;

  WDT_A -> CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD; // stop watchdog timer
  Clock_Init48MHz(); // makes bus clock 48 MHz
  Motor_Init();
  TimerInit();
  UltraSonicInit();
  ServoInit();
  Servo_Motor(4499);
  //Servo_MotorInit();

  //These are the states of the state machine
  enum motor_states {
    FORWARD,
    BACKWARD,
    RIGHT,
    LEFT,
    SWEEP_RIGHT,
    SWEEP_LEFT
  }
  state, prevState;

  state = FORWARD; //start in FORWARD state
  prevState = !FORWARD; //used to know when the state has changed
  uint16_t stateTimer = 0; //used to stay in a state
  bool isNewState; //true when the state has switched

  while (1) {

    isNewState = (state != prevState);
    prevState = state;
    distance = distanceInCm(); //this needs to be moved to the states that use it


    switch (state) {

          case FORWARD:
             if (isNewState){
                 stateTimer = 0;
             }
             Motor_Forward(14999,14200);


             if(distance <= 30){
                 state = BACKWARD;
             }


             break;
          case RIGHT:
              if (isNewState){
                       stateTimer = 0;
                           }
              Motor_Right(14999,14999);

              stateTimer++;

              if(stateTimer >=50 ) {//8 or 9

                  state = FORWARD;
                 }
              break;

          case LEFT:
              if (isNewState){
                  stateTimer = 0;
              }
              Motor_Left(14999,14999);
              stateTimer++;

              if(stateTimer >= 50) {

                  state = FORWARD;
                 }
              break;

          case BACKWARD:
              if (isNewState){
                  stateTimer = 0;
              }

              Motor_Backward(14999,14200);
              stateTimer++;

              if(stateTimer >= 30) {

                  state = SWEEP_RIGHT;
                 }
              break;

          case SWEEP_RIGHT:
              if (isNewState){
                  stateTimer = 0;
              }
              Motor_Stop();
              servoRight();
              Clock_Delay1ms(50);
              rightDistance = distance;

              Servo_Motor(4499);



              state = SWEEP_LEFT;


              break;

          case SWEEP_LEFT:
              if (isNewState){
                  stateTimer = 0;
              }

              servoLeft();
              Clock_Delay1ms(50);
              leftDistance = distance;
              Servo_Motor(4499);

                  if(rightDistance < leftDistance){
                      state = RIGHT;
                  }
                  else if(rightDistance >= leftDistance){
                      state = LEFT;
                  }

              break;
              default: state = FORWARD;

    }
    Clock_Delay1ms(20);
  } //while
}
