/*
 *Name: Kenzie Moore
 * Program: Electrical Engineering Technology
 * Year: 3rd year
 * Class: Microcontroller Systems
 * Section: CPET 253
 * Exercise: Lab 7
 * Date : 3/20/2022
 */
// Reflectance.c
// Provide functions to take measurements using the kit's built-in
// QTRX reflectance sensor array.  Pololu part number 3672. This works by outputting to the
// sensor, waiting, then reading the digital value of each of the
// eight phototransistors.  The more reflective the target surface is,
// the faster the voltage decays.
// Daniel and Jonathan Valvano
// July 11, 2019

/* This example accompanies the book
   "Embedded Systems: Introduction to Robotics,
   Jonathan W. Valvano, ISBN: 9781074544300, copyright (c) 2019
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/

Simplified BSD License (FreeBSD License)
Copyright (c) 2019, Jonathan Valvano, All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

The views and conclusions contained in the software and documentation are
those of the authors and should not be interpreted as representing official
policies, either expressed or implied, of the FreeBSD Project.
*/

// reflectance even LED illuminate connected to P5.3
// reflectance odd LED illuminate connected to P9.2
// reflectance sensor 1 connected to P7.0 (robot's right, robot off road to left)
// reflectance sensor 2 connected to P7.1
// reflectance sensor 3 connected to P7.2
// reflectance sensor 4 connected to P7.3 center
// reflectance sensor 5 connected to P7.4 center
// reflectance sensor 6 connected to P7.5
// reflectance sensor 7 connected to P7.6
// reflectance sensor 8 connected to P7.7 (robot's left, robot off road to right)

#include <stdint.h>
#include "msp432.h"
#include "..\inc\Clock.h"
#include "..\inc\Motor.h"

// ------------Reflectance_Init------------
// Initialize the GPIO pins associated with the QTR-8RC
// reflectance sensor.  Infrared illumination LEDs are
// initially off.
// Input: none
// Output: none
void Reflectance_Init(void) {
  P5DIR |= 0x8; // MAKE 5.3 OUTPUT
  P5SEL0 &= ~0x08;
  P5SEL1 &= ~0x08;


  P9DIR |= 0x4; // MAKE 9.2 OUTPUT
  P9SEL0 &= ~0x04;
  P9SEL1 &= ~0x04;


  P7DIR |= 0xFF;
  P7SEL0 = 0x00;
  P7SEL1 = 0x00;

}

// ------------Reflectance_Read------------
// Read the eight sensors
// Turn on the 8 IR LEDs
// Pulse the 8 sensors high for 10 us
// Make the sensor pins input
// wait t us
// Read sensors
// Turn off the 8 IR LEDs
// Input: time to wait in usec
// Output: sensor readings
// Assumes: Reflectance_Init() has been called
uint8_t Reflectance_Read(uint32_t time) {
  uint8_t result;

  P7DIR |= 0xFF;
  //1. set P5.3 and P9.2 as high, which means turn on the LED (no need to set as GPIO since we assume we already initialized)
  P5OUT |= 0x08;
  P9OUT |= 0x04;
  //2. Set ALL 8 Pins of P7 (P7.7 - P7.0) as outputs (DIR and OUT)
  P7DIR |= 0xFF;
  P7OUT |= 0xFF;
  Clock_Delay1ms(100);
  P7DIR &= ~0xFF;
  Clock_Delay1us(time);
  result = P7IN;
  P5OUT &= ~0x08;
  P9OUT &= ~0x04;
  //3. Use function Clock_Delay1us(time) to delay 10us, now we lighted up the LED and made a 10us pulse
  //4. Then make the sensor pins (P7.7 - P7.0) as inputs



  // write this as part of Lab 7
  return result;
}

// Perform sensor integration
// Input: data is 8-bit result from line sensor
// Output: position in 0.1mm relative to center of line
const int32_t Weight[8] = {
  334,
  238,
  142,
  48,
  -48,
  -142,
  -238,
  -334
};
const int32_t Mask[8] = {
  0x01,
  0x02,
  0x04,
  0x08,
  0x10,
  0x20,
  0x40,
  0x80
};
// Perform sensor integration
// Input: data is 8-bit result from line sensor
// Output: position in 0.1mm relative to center of line
int32_t Reflectance_Position(uint8_t data) {
    int32_t Result, e=0, f=0;
    int16_t i;
    int16_t b[8];
   for(i=0; i < 8; i++)
   {    b[i] = data % 2;
         data = data / 2;
   }
   for(i=0; i < 8; i++)
   { e = e + (b[i] * Weight[i]);
       f = f + b[i];

   }
   Result = e / f;







   // write this as part of Lab 6
 return Result; // replace this line
}
<<<<<<< Updated upstream

=======
<<<<<<< HEAD
=======

>>>>>>> 33d9a8a9ce833a08a000f6ddc1f6f11ec6d2130d
>>>>>>> Stashed changes


