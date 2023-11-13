/*
 *Name: Kenzie Moore
 * Program: Electrical Engineering Technology
 * Year: 3rd year
 * Class: Microcontroller Systems
 * Section: CPET 253
 * Exercise: Lab 5 Prelab
 * Date : 2/12/2022
 */
// BumpInt.c
// Runs on MSP432, interrupt version
// Provide low-level functions that interface bump switches on the robot.
// Daniel Valvano and Jonathan Valvano
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

// Negative logic bump sensors
#define bump5 0x80 // P4.7 Bump5, left side of robot
#define bump4 0x40 // P4.6 Bump4
#define bump3 0x20 // P4.5 Bump3
#define bump2 0x08 // P4.3 Bump2
#define bump1 0x04 // P4.2 Bump1
#define bump0 0x01 // P4.0 Bump0, right side of robot

#include <stdint.h>

#include <stdbool.h>

#include "msp.h"

#include "..\inc\Clock.h"

#include "..\inc\CortexM.h"

// Initialize Bump sensors

// pins 7,6,5,3,2,0
// Interrupt on falling edge (on touch)
volatile bool wasInterrupt = false;
volatile uint16_t counts = 0;
volatile uint8_t direction = 2;
volatile uint8_t Interupt = 0;

void PinInit(void) {
   P4DIR &= ~0xED; // Make six Port 4 pins inputs
   P4REN |= 0xED; // Activate interface pullup
   P4OUT |= 0xED; // put 0 in pin positions to turn on pull up resistor
   P4SEL0 &= ~0xED; //use P4.7,6,5,3,2,0 as GPIO
   P4SEL1 &= ~0xED;
   return;
}

void BumpInt_Init(void) {
   P4IE |= 0xED; //enables interrupt on pins 7,6,5,3,2,0
   P4IES |= 0xED; //sets falling edge edge
   NVIC -> ISER[1] = 0x40; //enables port 4 interrupts

   P4IFG &= ~0xED; //clears flag if button was already pushed
   EnableInterrupts(); //global interrupt enable
   return;

}

// triggered on touch, falling edge
void PORT4_IRQHandler(void) {
   static uint16_t status = 0;
   status = P4IV; //read current state of switches
   if (status == 0x02) {
      wasInterrupt = true; //tells main about interrupt
      Interupt = 1;
      direction = 0;
      counts += 3;
      P4IFG &= 0x00;
      return;
   }
   if (status == 0x06) {
      wasInterrupt = true; //tells main about interrupt
      Interupt = 1;
      direction = 0;
      counts += 2;
      P4IFG &= 0x00;
      return;
   }
   if (status == 0x08) {
      wasInterrupt = true; //tells main about interrupt
      Interupt = 1;
      direction = 0;
      counts += 1;
      P4IFG &= 0x00;
      return;
   }
   if (status == 0x0C) {
      wasInterrupt = true; //tells main about interrupt
      Interupt = 1;
      direction = 1;
      counts -= 1;
      P4IFG &= 0x00;
      return;
   }
   if (status == 0x0E) {
      wasInterrupt = true; //tells main about interrupt
      Interupt = 1;
      direction = 1;
      counts -= 2;
      P4IFG &= 0x00;
      return;
   }
   if (status == 0x10) {
      wasInterrupt = true; //tells main about interrupt
      Interupt = 1;
      direction = 1;
      counts -= 3;
      P4IFG &= 0x00;
      return;
   }
   return;
}
