/*
*Name: Kenzie Moore
* Program: Electrical Engineering Technology
* Year: 3rd year
* Class: Microcontroller Systems
* Section: CPET 253
* Exercise: Lab 6
* Date : 3/13/2022
*/


void static commandwrite(uint8_t command){
// you write this as part of Lab 6

        while(!(UCBUSY & 0x0001)){}   //wait for SPI to be idle(check flag)
        P9OUT &= ~0x40;;   //set DC (P9.6) for command
        UCA3TXBUF = command;   //fill the TXBUF, this starts transfer
        while(!(UCBUSY & 0x0001)){}   //wait for SPI to be idle
}


void static datawrite(uint8_t data){
// you write this as part of Lab 6
       while(!(UCBUSY & 0x0001)){}   //wait for SPI to be idle (check flag)
       P9OUT |= 0x40;   //set DC (P9.6) for data
       UCA3TXBUF = data;   //fill the TXBUF, this starts transfer
}