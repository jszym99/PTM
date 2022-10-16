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

#define CLK 16000000 //Clock speed

 
int main(){
    DDRC = 0x10;
    PORTC = 0x0F;

    DDRB = 0xFF;
    PORTB = 0x00;

    int flag = 0;
    int snakeFlag = 0;
    int snake = 0b10000000;
    int time = 0;
    int n = 1;
    int del = 500;
    int rev = 1;
    int prevSW2=0;
    
    int OUT_MASK = 0x00000000;

    int SW1 = 0;
    int SW2 = 0;
    int SW3 = 0;
    int SW4 = 0;
    
    
    while(1) {
        
        // Clearing output mask
        OUT_MASK = 0x00000000;

        // Get switch inputs
        SW1 = ~(0x0F & PINC) & 0b00000001;
        SW2 = ~(0x0F & PINC) & 0b00000010;
        SW3 = ~(0x0F & PINC) & 0b00000100;
        SW4 = ~(0x0F & PINC) & 0b00001000;

        //int opt = getKey();

        // SW_1 operation
        if(SW1)
            OUT_MASK += 0b00000001;

        // SW_2 operation (with rising edge detection)
        if(SW2 && SW2 != prevSW2){
            // On/Off flag
            if(flag)
                flag = 0;
            else
                flag = 1;
        }

        prevSW2 = SW2; //rising edge flag

        // ledstate depending on flag
        if(flag)
            OUT_MASK += 0b00000010;

        // "Snake" animation set
        if(SW3)
            snakeFlag = 1;

        // "Snake" animation reset
        if(SW4)
            snakeFlag = 0;


        // "Snake" animation
        if(snakeFlag){
            if(time >= del){
                if(rev)
                    snake = 0b10000000 >> n;
                else
                    snake = 0b00000100 << n;

                if(n < 4)
                    n++;
                else{
                    n=0;
                    rev = !rev;
                }
                    
                time = 0;
            }
            else
                time++;
            OUT_MASK += snake;
        }

        // Set output ports
        PORTB = OUT_MASK;
        _delay_ms(1);
    }
    return 0;
}
