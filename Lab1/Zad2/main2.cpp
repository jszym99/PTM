int getKey(){
    for (int i=0; i < 4; i++){
        if (bit_is_clear(PINC, i)){
            return i+1;
        }
    }
    return 0;
}

void check_and_wait(int val){
    for (int i=0; i < val; ++i){
        int pressed_button = getKey();
        
        if (pressed_button == 1)
            sbi(PORTB,0);
        else
            cbi(PORTB,0);

        if (pressed_button == 2){
            if(bit_is_clear(PORTB,1))
                sbi(PORTB,1);
            else
                cbi(PORTB,1);
        }

        _delay_ms(1);
    }
}

int main(){
    DDRC = 0b00010000;
    PORTC = 0b00001111;

    DDRB = 0xFF;
    PORTB = 0x00;
    
    while(1) {
        int pressed_button = getKey();
        if (pressed_button == 1){
            while (bit_is_clear(PINC, 0))
                PORTB = 0b00000001;
            cbi(PORTB,0);
        }

        if (pressed_button == 2){
            if(bit_is_clear(PORTB,1))
                PORTB = 0b00000010;
            else
                PORTB = 0b00000000;
        }

        if (pressed_button == 3){
            while (!bit_is_clear(PINC, 3)){
                for(int i=0;i<6;i++){
                    PORTB = ((0b00000100 << i));
                    check_and_wait(500);
                }

                for(int i=1;i<5;i++){
                    PORTB = ( (0b10000000 >> i));
                    check_and_wait(500);
                }
            }
        }
    }
    return 0;
}