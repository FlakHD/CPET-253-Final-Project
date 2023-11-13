#include "msp.h"

#include <stdint.h>

#include <stdbool.h>

#include "..\inc\Clock.h"

#include "..\inc\CortexM.h"

#include "..\inc\BumpInt.h"

#include "..\inc\Motor.h"

#include "../inc/SSD1306.h"

extern volatile bool wasInterrupt;
extern volatile uint8_t direction;
extern volatile uint8_t Interupt;
void BumpTask(void) {

   enum motor_states {
      off,
      BLE,
      forward,
      right,
      left,
      backward
   }
   state, prevState;
   state = off; //start state
   prevState = !off; //used to know when the state has changed
   uint16_t stateTimer; //used to stay in a state
   bool isNewState; //true when the state has switched

   while (1) {
      isNewState = (state != prevState);
      prevState = state; //save state for next time
      switch (state) {
      case off:
         state = BLE;

         break;
      case BLE:
         if (wasInterrupt) {
            SSD1306_SetCursor(0, 0);
            SSD1306_Clear();
            SSD1306_OutString("OUCH!!!");

            state = backward;
            wasInterrupt = false;
         }
         Interupt = 0;

         break;

      case forward:
         Motor_Forward(14999, 14200);
         Clock_Delay1ms(3000);

         state = BLE;

         break;

      case right:
         if (isNewState) {
            stateTimer = 0;
         }
         SSD1306_Clear();
         Motor_Right(14999, 14200);
         Clock_Delay1ms(100);
         state = forward;

         break;

      case left:
         if (isNewState) {
            stateTimer = 0;
         }
         SSD1306_Clear();
         Motor_Left(14999, 14200);
         // stateTimer++;
         Clock_Delay1ms(100);
         state = forward;

         break;

      case backward:

         Motor_Backward(14999, 14200);
         Clock_Delay1ms(100);

         if (direction == 0) {
            state = left;
            wasInterrupt = false;

         }
         if (direction == 1) {
            state = right;
            wasInterrupt = false;
         }
default: state = BLE;
         break;
      } //switch
      Clock_Delay1ms(20);

   } //while(1)

}
