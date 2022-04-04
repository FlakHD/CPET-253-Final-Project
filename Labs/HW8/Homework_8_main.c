#include "msp.h"
#include "../inc/Clock.h"
#include "../inc/SSD1306.h"
#include <stdint.h>
#include "../inc/Clock.h"
#include "../inc/CortexM.h"
#include "msp.h"


void acd_Init(){
  ADC14->CTL0 = 0x00000010; //power on and disable ADC (ENC??) config
    //sample hold pulse mode
    //
    //16 sample clocks
  ADC14->CTL0 = 0x04D80200;
  ADC14->CTL1 = 0x00000030; // 14 bit resolution; 16 clock conversion time
  ADC14->MCTL[5] = 0; // A0 input, single ended, AVCC -> AVSS ????, no window
  ADC14->CTL1 = 0x00050000; // start conversion for mem reg 5
  ADC14->CTL0 = 0x00000002; // enable ADC to complete conversion
}

void main(void){
  WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;	
  //clock set for 3MHz
  adc_Init();
  while(1){
    ADC14->CTL0 |= 0x01; // start conversion
    while(!((ADC14->IFGR0) & 0x00000020)); //wait until conversion is completed
    inputData = ADC14->MEM[5];
    calcVoltage = (((float)inputData) * (3.3/(pow(2,14))));
    tempCelcius = calcVoltage/0.0019;
    printf("Value is: %d \n", inputData);
    printf("Converted value is : %f Volts \n", calcVoltage);
    printf("Temperature value is :" tempCelcius "Volts \n");
    Clock_Delay1ms(500);
    }
}
