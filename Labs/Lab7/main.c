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
/*
void reflectance_funct(void){

        Data = Reflectance_Read(500);
        position = Reflectance_Position(Data);

        if(position==0){
             Motor_Forward(4000,4000);
         }

         else if(position>=238){
                  Motor_Stop();
                  TimedPause(100);
                  Motor_Forward(2000,7000);
                  TimedPause(100);
                      }

              else if(position<=-238){
                  Motor_Stop();
                  TimedPause(100);
                  Motor_Forward(7000,2000);
                  TimedPause(100);
              }
              else if((position>0)&&(position<238)){
                         Motor_Forward((3000-8*position),3700);
                     }

              else if((position <0)&&(position>-238)){
                         Motor_Forward(3700,(3000+8*position));
                     }


        if(Data == 0xFF){
                                 Motor_Forward(4000,4000);
                                          TimedPause(200);
                                                  Motor_Left(5000,5000);

                                                  TimedPause(350);

        }


        if((Data == 0xF8)||(Data == 0xF0)||(Data == 0xE0)){
           Data = Reflectance_Read(500);
                        TimedPause(10);
             if((Data == 0xF8)||(Data == 0xF0)||(Data == 0xE0)){
                 Data = Reflectance_Read(500);
                             TimedPause(10);
                if((Data == 0xF8)||(Data == 0xF0)||(Data == 0xE0)){
              Motor_Forward(4000,3900);
               TimedPause(200);
               Data = Reflectance_Read(500);
               if(Data == 0x00){
                 Motor_Right(5700,5700);

                 TimedPause(350);
                       }
               else {Motor_Forward(5000,5000);

                     }
                       }

                else;}
                   else;}
        else if(Data == 0x00){
                    Motor_Left(3000,4000);TimedPause(300);

                }


        if((Data == 0x1F)||(Data == 0x0F)||(Data == 0x07)){
            Data = Reflectance_Read(500);
        }
            TimedPause(5);
            if((Data == 0x1F)||(Data == 0x0F)||(Data == 0x07)){
               TimedPause(5);
                Data = Reflectance_Read(500);
             if((Data == 0x1F)||(Data == 0x0F)||(Data == 0x07)){
               Motor_Forward(4000,4000);
               TimedPause(150);
               Motor_Left(5000,5000);

               TimedPause(350);
                       }
                else;}
                else;}


*/
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
	Clock_Init48MHz(); // makes bus clock 48 MHz
	Motor_Init();
	TimerInit();
	Reflectance_Init();
	while(1){
	    Data = Reflectance_Read(2000);
	            position = Reflectance_Position(Data);

	            if(position==0){
	                 Motor_Forward(4000,4000);
	             }

	             else if(position>=238){
	                      Motor_Stop();
	                      TimedPause(100);
	                      Motor_Forward(2000,7000);
	                      TimedPause(100);
	                          }

	                  else if(position<=-238){
	                      Motor_Stop();
	                      TimedPause(100);
	                      Motor_Forward(7000,2000);
	                      TimedPause(100);
	                  }
	                  else if((position>0)&&(position<238)){
	                             Motor_Forward((3000-8*position),3700);
	                         }

	                  else if((position <0)&&(position>-238)){
	                             Motor_Forward(3700,(3000+8*position));
	                         }



	   } //while(1)

	} //main()




