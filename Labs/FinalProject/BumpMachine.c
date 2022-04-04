#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"
#include "..\inc\BumpInt.h"
#include "..\inc\Motor.h"

extern volatile bool        wasInterrupt;
extern volatile uint16_t        counts;
extern volatile uint8_t        direction;
void BumpTask(void){

 enum motor_states{off, forward, right, left, backward} state, prevState;
         state = off;                   //start state
         prevState = !off;               //used to know when the state has changed
         uint16_t stateTimer;       //used to stay in a state
         bool isNewState;           //true when the state has switched

         while(1){
                         isNewState = (state != prevState);
                         prevState = state;  //save state for next time
                 switch (state) {
                     case off:
                          state = forward;

                          break;

                          if (isNewState){
                              stateTimer = 0;
                          }

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
                            Clock_Delay1ms(100);
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
                           Clock_Delay1ms(100);
                           state = forward;


                           break;

                       case backward:
                            if (isNewState){
                                  stateTimer = 0;
                              }

                             Motor_Backward(14999,14200);
                             Clock_Delay1ms(100);



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
                 Clock_Delay1ms(20);

                } //while(1)


}
