#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
 
#ifndef _BV
#define _BV(bit)        (1<<(bit))
#endif
#ifndef sbi
#define sbi(reg,bit)    reg |= (_BV(bit))
#endif
 
#ifndef cbi
#define cbi(reg,bit)    reg &= ~(_BV(bit))
#endif

int COUNT = 0;

ISR (INT0_vect) //External interrupt_zero ISR
{
    if(~(0x0F & PIND) & 0b00001000)
    {
        if(COUNT < 5)
            COUNT++;
    }
    else{
        if(COUNT > 0)
            COUNT--;
    }
}

int main() {
    DDRD = 0x10;
    PORTD = 0x0F;

    DDRB = 0xFF;
    PORTB = 0x00;

    EICRA = 0x03; // Input type (rising edge)
    EIMSK = 0x01; // Enable interrupt on INT0
    SREG += 0b10000000; // Enable global interrupt
    
    while (1) {
        
        PORTB = 0b00000100 << COUNT;

    }
    return 0;
}