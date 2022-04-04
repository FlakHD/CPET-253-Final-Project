#include <string.h>

#include <stdbool.h>

#include <stdint.h> //Exact-width integer types

#include <ti\devices\msp432p4xx\driverlib\driverlib.h> //Driver library

#define DCO_FREQ 48e6 //unit: Hz; DCO nominal frequencies: 1.5, 3, 6, 12, 24, 48 MHz.
#define TIMER0_FREQ 2 //unit: Hz
#define SAMPLE_FREQ 1 //unit: Hz
#define RED_LED GPIO_PIN0
#define GREEN_LED GPIO_PIN1
#define BLUE_LED GPIO_PIN2
#define NUM_DISP_TEXT_LINE 4
#define MAX_STR_BUFFER_LEN 100
//Function prototypes
void initDevice(void);
void initGPIO(void);
void initTimer(void);
void initUART(void);
void initADC14(void);
void uart0_transmitStr(const char * str);
//Global variables
uint32_t clockMCLK, clockACLK;
uint8_t currentLED = RED_LED;
volatile uint32_t tempSensorCal30, tempSensorCal85, tempSensorCalDiff;
char strBuffer[MAX_STR_BUFFER_LEN];
const char * terminalDisplayText[NUM_DISP_TEXT_LINE] = {
   "\r\n",
   "UART and User Button Demo\r\n",
   "R: red, G: green, B:blue, S:timed sample, H: Help\r\n",
   "> "
};
//Timer_A continuous mode configuration
Timer_A_UpModeConfig upModeConfig = {
   TIMER_A_CLOCKSOURCE_ACLK, //ACLK clock source, 32kHz
   TIMER_A_CLOCKSOURCE_DIVIDER_1,
   0, //timerPeriod, will set in program
   TIMER_A_TAIE_INTERRUPT_DISABLE, //Disable timer ISR
   TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE, //Disable CCR0 interrupt
   TIMER_A_DO_CLEAR //Clear counter
};
//Timer_A Compare Configuration Parameter
Timer_A_CompareModeConfig compareConfig = {
   TIMER_A_CAPTURECOMPARE_REGISTER_1, //Use CCR1
   TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE, //Disable CCR interrupt
   TIMER_A_OUTPUTMODE_SET_RESET, //Toggle output
   //compareValue, will set in program
};
void main(void) {
   bool flagTimerAon = false;
   uint32_t i;
   uint8_t data;
   initDevice();
   initGPIO();
   initTimer();
   initUART();
   initADC14();
   Interrupt_enableMaster();
   Timer32_startTimer(TIMER32_0_BASE, false);
   //Initial display on terminal.
   for (i = 0; i < NUM_DISP_TEXT_LINE; i++) {
      uart0_transmitStr(terminalDisplayText[i]);
   }
   while (1) {
      if (UART_getInterruptStatus(EUSCI_A0_BASE,
            EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)) {
         data = UART_receiveData(EUSCI_A0_BASE);
         UART_clearInterruptFlag(EUSCI_A0_BASE,
            EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG);
         switch (data) {
         case 'R':
         case 'r':
            currentLED = RED_LED;
            uart0_transmitStr("Blink red LED.\r\n> ");
            break;
         case 'G':
         case 'g':
            currentLED = GREEN_LED;
            uart0_transmitStr("Blink green LED.\r\n> ");
            break;
         case 'B':
         case 'b':
            currentLED = BLUE_LED;
            uart0_transmitStr("Blink blue LED.\r\n> ");
            break;
         case 'H':
         case 'h':
            for (i = 0; i < NUM_DISP_TEXT_LINE; i++) {
               uart0_transmitStr(terminalDisplayText[i]);
            }
            break;
         case 'S':
         case 's':
            if (flagTimerAon) {
               flagTimerAon = false;
               Timer_A_stopTimer(TIMER_A0_BASE);
               //ADC14_toggleConversionTrigger();
            } else {
               flagTimerAon = true;
               Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
               ADC14_toggleConversionTrigger();
            }
            break;
         }
      } //end of if
   } //end of while
}
void initDevice(void) {
   WDT_A_holdTimer(); //Stop Watchdog timer
   //Change VCORE to 1 to support a frequency higher than 24MHz.
   //See data sheet for Flash wait-state requirement for a given frequency.
   PCM_setPowerState(PCM_AM_LDO_VCORE1);
   FlashCtl_setWaitState(FLASH_BANK0, 1);
   FlashCtl_setWaitState(FLASH_BANK1, 1);
   //Enable FPU for DCO Frequency calculation.
   FPU_enableModule();
   FPU_enableLazyStacking(); // Required to use FPU within ISR.
   //Only use DCO nominal frequencies: 1.5, 3, 6, 12, 24, 48MHz.
   CS_setDCOFrequency(DCO_FREQ);
   //Divider: 1, 2, 4, 8, 16, 32, 64, or 128.
   //SMCLK used by UART, ACLK used by Timer_A.
   CS_initClockSignal(CS_MCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1);
   CS_initClockSignal(CS_HSMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_8);
   CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_16);
   CS_initClockSignal(CS_ACLK, CS_REFOCLK_SELECT, CS_CLOCK_DIVIDER_1); //32kHz
   clockMCLK = CS_getMCLK();
   clockACLK = CS_getACLK();
}
void initGPIO(void) {
   //Configure P2.0, P2.1, P2.2 as output.
   //P2.0, P2.1, P2.2 are connected to a RGB tri-color LED on LaunchPad.
   GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
   // configured p4.6 as input.
   GPIO_setAsInputPin(GPIO_PORT_P4, GPIO_PIN6);
}
void initTimer(void) {
   Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
      TIMER32_PERIODIC_MODE);
   Timer32_setCount(TIMER32_0_BASE, clockMCLK / TIMER0_FREQ - 1);
   Timer32_enableInterrupt(TIMER32_0_BASE);
   Interrupt_enableInterrupt(INT_T32_INT1); //Enable Timer32_0 interrupt in the interrupt
   controller.
   //Configuring Timer_A to trigger ADC14
   upModeConfig.timerPeriod = clockACLK / SAMPLE_FREQ - 1;
   Timer_A_configureUpMode(TIMER_A0_BASE, & upModeConfig);
   compareConfig.compareValue = clockACLK / SAMPLE_FREQ - 2; //Set at 1 cycle before timer
   period.
   Timer_A_initCompare(TIMER_A0_BASE, & compareConfig);
}
void initUART(void) {
   //Configuration for 3MHz SMCLK, 9600 baud rate.
   //Calculated using the online calculator that TI provides at:
   //http://softwaredl.
   ti.com / msp430 / msp430_public_sw / mcu / msp430 / MSP430BaudRateConverter / index.html
   const eUSCI_UART_Config config = {
      EUSCI_A_UART_CLOCKSOURCE_SMCLK, //SMCLK Clock Source
      19, //BRDIV = 19
      8, //UCxBRF = 8
      0, //UCxBRS = 0
      EUSCI_A_UART_NO_PARITY, //No Parity
      EUSCI_A_UART_LSB_FIRST, //MSB First
      EUSCI_A_UART_ONE_STOP_BIT, //One stop bit
      EUSCI_A_UART_MODE, //UART mode
      EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION //Oversampling
   };
   //Configure GPIO pins for UART. RX: P1.2, TX:P1.3.
   GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2 | GPIO_PIN3,
      GPIO_PRIMARY_MODULE_FUNCTION);
   UART_initModule(EUSCI_A0_BASE, & config);
   UART_enableModule(EUSCI_A0_BASE);
}
//Use P6.1/A14 to sample
void initADC14(void) {
   ADC14_enableModule();
   ADC14_initModule(ADC_CLOCKSOURCE_SMCLK, ADC_PREDIVIDER_1,
      ADC_DIVIDER_1, ADC_NOROUTE);
   //Configure P6.1 as A14
   //GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4, GPIO_PIN5,
   GPIO_TERTIARY_MODULE_FUNCTION);
ADC14_configureSingleSampleMode(ADC_MEM0, true);
ADC14_configureConversionMemory(ADC_MEM0,
   ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A14, false);
//See TechRef Section 20.2.6 for sample timing consideration.
ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);
ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);
ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_4, ADC_PULSE_WIDTH_4);
ADC14_enableConversion();
ADC14_enableInterrupt(ADC_INT0); //Interrupt for ADC_MEM0
Interrupt_enableInterrupt(INT_ADC14);
}
//Transmit a string through UART0.
void uart0_transmitStr(const char * str) {
   uint32_t len, i = 0;
   len = strlen(str);
   while (i < len) {
      UART_transmitData(EUSCI_A0_BASE, str[i++]);
      while (!UART_getInterruptStatus(EUSCI_A0_BASE,
            EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT_FLAG));
      UART_clearInterruptFlag(EUSCI_A0_BASE,
         EUSCI_A_UART_TRANSMIT_COMPLETE_INTERRUPT_FLAG);
   }
}
//Timer32_0 ISR
void T32_INT1_IRQHandler(void) {
   Timer32_clearInterruptFlag(TIMER32_0_BASE);
   if (GPIO_getInputPinValue(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2))
      GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0 | GPIO_PIN1 | GPIO_PIN2);
   else
      GPIO_setOutputHighOnPin(GPIO_PORT_P2, currentLED);
}
void ADC14_IRQHandler(void) {
   uint64_t status;
   uint8_t PINfour_six;
   float data;
   status = ADC14_getEnabledInterruptStatus();
   ADC14_clearInterruptFlag(status);
   if (status & ADC_INT0) {
      data = ADC14_getResult(ADC_MEM0);
      data = 3.3 * data / 0x3FFF;
      PINfour_six = GPIO_getInputPinValue(GPIO_PORT_P4, GPIO_PIN6);
      snprintf(strBuffer, MAX_STR_BUFFER_LEN, "P4.5 voltage = %.2f V, P4.6 digital = %d\
        r\ n > ", data, PINfour_six);
      uart0_transmitStr(strBuffer);
   }
}