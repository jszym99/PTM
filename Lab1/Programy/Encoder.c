#include <avr/io.h>
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
 
int main() {
    DDRC = 0x10;
    PORTC = 0x0F;

    DDRB = 0xFF;
    PORTB = 0x00;

    // Pin variables
    int A,B,pA,pB = 0;
    int count = 0;

    while (1) {
        A = ~(0x0F & PINC) & 0b00000001;
        B = ~(0x0F & PINC) & 0b00000010;

        if(A && !pA && !B && !pB)
            if(count < 5)
                count ++;

        _delay_ms(5);

        if(!A && !pA && B && !pB)
            if(count > 0)
                count --;

        _delay_ms(5);
        
        PORTB = 0b00000100 << count;

        pA = A;
        pB = B;
    }
}