//------------See AP.c for details of hardware connections to CC2650--------------------
//------------See LaunchPad.c for details of switches and LEDs--------------------------

#include <stdint.h>

#include "../inc/Clock.h"

#include "../inc/CortexM.h"

#include "../inc/AP.h"

#include "../inc/UART0.h"

#include "../inc/UART1.h"

#include "../inc/SSD1306.h"

#include "../inc/motor.h"

#include <stdbool.h>

//#include "../inc/BumpMachine.h"

#include "../inc/BumpInt.h"

#include "../inc/LaunchPad.h"

#include "msp.h"

#define CR 0x0D

uint8_t BT_ByteData; // 8-bit user data from the phone
extern volatile uint16_t counts;
extern volatile bool wasInterrupt;
extern volatile uint8_t direction;
//extern volatile uint8_t Interupt;

// ********OutValue**********
// Debugging dump of a data value to virtual serial port to PC
// data shown as 1 to 8 hexadecimal characters
// Inputs:  response (number returned by last AP call)
// Outputs: none
void ValueOut(char * label, uint32_t value) {
   UART0_OutString(label);
   UART0_OutUHex(value);
}

void MoveRobot(uint8_t command) {
   // this function calls the appropriate functions to stop, move forward, move backward, turn right,
   // or turn left according to the command received from the BLE

   if (command == 1) {
      Motor_Forward(21999, 21200);
      SSD1306_SetCursor(0, 0);
      SSD1306_Clear();
      SSD1306_OutString("FTL DRIVE ENGAGED ");
   }

   if (command == 2) {
      SSD1306_SetCursor(0, 0);
      SSD1306_Clear();
      SSD1306_OutString("Turning Right... ");
      Motor_Right(9999, 9999);
      Clock_Delay1ms(500);
     /* Motor_Stop();
      SSD1306_SetCursor(0, 0);
      SSD1306_Clear();
      SSD1306_OutString("DONE!"); */
      Motor_Forward(9999, 9200);
      SSD1306_SetCursor(0, 0);
      SSD1306_Clear();
      SSD1306_OutString("ZOOOOOM!!! ");
   }
   if (command == 3) {
      SSD1306_SetCursor(0, 0);
      SSD1306_Clear();
      SSD1306_OutString("Turning Left... ");
      Motor_Left(9999, 9999);
      Clock_Delay1ms(500);
     /* Motor_Stop();
      SSD1306_SetCursor(0, 0);
      SSD1306_Clear();
      SSD1306_OutString("DONE!");*/
      Motor_Forward(9999, 9200);
      SSD1306_SetCursor(0, 0);
      SSD1306_Clear();
      SSD1306_OutString("ZOOOOOM!!! ");
   }
   if (command == 4) {
      Motor_Backward(14999, 14200);
      SSD1306_SetCursor(0, 0);
      SSD1306_Clear();
      SSD1306_OutString("Backing up! ");
   }
   if (command == 5) {
      Motor_Stop();
      SSD1306_SetCursor(0, 0);
      SSD1306_Clear();
      SSD1306_OutString("Stopped... ");
   }

}

void WriteByteData(void) { // called on a SNP Characteristic Write Indication on characteristic ByteData
   MoveRobot(BT_ByteData); // send command to robot
   ValueOut("\n\rWrite BLE_ByteData=", BT_ByteData);
}

int main(void) {
   volatile int r;
   SSD1306_Init(SSD1306_SWITCHCAPVCC);
   DisableInterrupts();
   Clock_Init48MHz();
   UART0_Init();
   TimerInit();
   Motor_Init();
   PinInit();
   BumpInt_Init();
   SSD1306_Init(SSD1306_SWITCHCAPVCC);
   EnableInterrupts();
   UART0_OutString("\n\rApplication Processor - MSP432-CC2650\n\r");
   r = AP_Init();
   AP_GetStatus(); // optional
   AP_GetVersion(); // optional
   AP_AddService(0xFFF0);
   //------------------------
   BT_ByteData = 0; // write parameter from the phone will be used to control direction
   AP_AddCharacteristic(0xFFF1, 1, & BT_ByteData, 0x02, 0x08, "DirectionData", 0, & WriteByteData);

   //------------------------

   AP_RegisterService();
   AP_StartAdvertisementJacki();
   AP_GetStatus(); // optional

   ////////////////////////////////////////////////
   ////////////////////////////////////////////////
   Clock_Init48MHz(); // set system clock to 48 MHz
   SSD1306_Init(SSD1306_SWITCHCAPVCC);
   SSD1306_DrawString(0, 0, "--- RSLK 1.1 --------", WHITE);
   SSD1306_DrawString(0, 16, "123456789012345678901", WHITE);
   SSD1306_DrawString(0, 24, "abcdefghijklmnopqrstu", WHITE);
   SSD1306_DrawString(0, 32, "ABCDEFGHIJKLMNOPQRSTU", WHITE);
   SSD1306_DrawString(0, 40, "!@#$%^&*()_+-=", WHITE);
   SSD1306_DrawString(0, 48, "123456789012345678901", WHITE);
   SSD1306_DrawString(0, 56, "vwxyz,./<>?;'\"[]\\{}|", WHITE);
   SSD1306_DisplayBuffer();
   Clock_Delay1ms(500);
   SSD1306_ClearBuffer();
   SSD1306_DrawFastHLine(0, 5, 100, WHITE);
   SSD1306_DrawFastVLine(5, 10, 25, WHITE);
   SSD1306_DisplayBuffer();
   Clock_Delay1ms(500);
   SSD1306_ClearBuffer();

   SSD1306_Clear();
   /////////////////////////////////////////////////////////////
   /////////////////////////////////////////////////////////////
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
      AP_BackgroundProcess(); // handle incoming SNP frames

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

         break;

      case forward:
          Motor_Forward(18999, 18200);
          SSD1306_SetCursor(0, 0);
          SSD1306_Clear();
          SSD1306_OutString("ZOOOOOM!!! ");

         state = BLE;

         break;

      case right:
         if (isNewState) {
            stateTimer = 0;
         }
         SSD1306_Clear();
         Motor_Right(14999, 14200);
         Clock_Delay1ms(750);
         state = forward;

         break;

      case left:
         if (isNewState) {
            stateTimer = 0;
         }
         SSD1306_Clear();
         Motor_Left(14999, 14200);
         // stateTimer++;
         Clock_Delay1ms(750);

         state = forward;

         break;

      case backward:

         Motor_Backward(14999, 14200);
         Clock_Delay1ms(3000);

         if (direction == 0) {
            state = left;
            wasInterrupt = false;

         }
         if (direction == 1) {
            state = right;
            wasInterrupt = false;
         }

         break;
      default:
         state = BLE;
      } //switch
      Clock_Delay1ms(20);

   } //while(1)

}
