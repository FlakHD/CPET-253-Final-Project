/*
*Name: Kenzie Moore, Yin Hinton, Jonathon Sumner
* Class: Microcontroller Systems
* Section: CPET 253
* Exercise: HW7
* Date : 3/27/2022
*/
#include "msp.h"
#include <stdint.h>
#include <stdbool.h>
#include "..\inc\Clock.h"
#include "..\inc\CortexM.h"

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

void Servo_Motor(uint16_t Duty){
    // Run TimerA3 in PWM mode with provided duty cycle
    // Set motor controls for forward

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
/*
void Servo_MotorInit(void){
    // Run TimerA3 in PWM mode with provided duty cycle
      // Set motor controls for forward

      // turn on PWM and set duty cycle
      // fixed period of 20ms
    //First initialize TimerA2 for PWM
   P6DIR |= 0x40; // MAKE 6.6 OUTPUT
   P6SEL1 &= ~0x40; //select alternate IO mode
   P6SEL0 |= 0x40;//select alternate IO mode

   TA2CTL &= ~0x0030;  //stop the timer
   TA2CTL |= 0x0200;  TA2CTL &= ~0x0100;//choose SMCLK for the clock source
   TA2CCTL3 |= 0x00E0; //Outmode 7: reset/set

   TA2CTL |= 0x0080; TA2CTL &= ~0x0040;//choose clock divider of 4
   //start pwm
   TA2CCR0 = 59999;
   TA2CCR3 = 5999; //20% duty cycle
   TA2CTL |= 0x0010; //SET TIMER FOR UP MODE - this starts it



  return;
}
void Servo_Motor(uint16_t Duty){
    // Run TimerA3 in PWM mode with provided duty cycle
    // Set motor controls for forward

    // turn on PWM and set duty cycle
    TA2CTL |= 0x0010; // Control bits 5 and 4 are mode control 00 to stop, 01 for up counting
                    // bits 7 and 6 are clock divider 01 = /2
                    // bits 9 and 8 choose clock 10 = SMCLK

          // Counter, start at zero once turned on
    TA2CCR3 = Duty; // Capture/Compare 2 COMPARE MODE : holds value for comparison to timer TA3R

    // - START PWM

    P6OUT |= 0x40; //PWML on P6.6 (EN)



  return;
}
*/
const double steps[25] = {4499, 3999,3499,2999, 2499, 1999, 1499, 1999, 2499, 2999, 3499, 3999, 4499, 4999, 5499, 5999, 6499, 6999, 7499, 6999, 6499, 5999, 5499, 4999, 4499 };
const double right[7] = {4499, 3999,3499,2999, 2499, 1999, 1499};
const double left[7] = {4499, 4999, 5499, 5999, 6499, 6999, 7499};
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
    Clock_Init48MHz(); // makes bus clock 48 MHz

//Servo_MotorInit();
ServoInit();
Servo_Motor(4499);
int16_t i;


        for(i=0; i < 7; i++)
           {
            Clock_Delay1ms(100);
            Servo_Motor(left[i]);
            Clock_Delay1ms(100);
           }

<<<<<<< Updated upstream
<<<<<<< Updated upstream
//while (1){
       // Servo_Motor(7499); // 1ms pulse trigger pwm at 5% duty cycle
        //Clock_Delay1ms(20);
        //Servo_Motor(5999); // 2ms pulsetrigger pwm at 10% duty cycle
        //Clock_Delay1ms(20);
        //Servo_Motor(4499); // 1.5ms pulsetrigger pwm at 7.5% duty cycle

        //} // while button
//}
=======

>>>>>>> Stashed changes

=======
>>>>>>> Stashed changes
}

