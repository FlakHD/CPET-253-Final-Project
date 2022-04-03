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
#include "../inc/AP.h"
#include "../inc/UART0.h"
#include "../inc/LaunchPad.h"
#include "../inc/SSD1306.h"
#include "../inc/BumpInt.h"

#define TRIGGER 0x04
#define ECHO 0x08

#define microsecondsToClockCycles(a)((a) * 1.5) //assume 12Mhz clock divided by 8
#define clockCyclesToMicroseconds(a)((a) / 1.5) // 1.5 clock cycles = 1us
/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
}
