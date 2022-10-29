#include <Arduino.h>

#define LED 13

void setup() {
    // Led pin setup
    pinMode(LED,OUTPUT);
}

void loop() {
    int i = 0;
    int interval = 1000;
    unsigned long ptime = 0;
    char menu;

    // Serial comunication initialization
    Serial.begin(9600);

    Serial.println("Initializing...");
    delay(1500); // Bez tego licznik odwala na starcie
    Serial.println("Initialized");

    while(1){
        if(ptime < (millis()-interval))
        {
            Serial.print("Hello World!!! ");
            Serial.println(i);
            i++;
            ptime = millis(); // Update time
        }
        menu = Serial.read();
        switch (menu)
        {
        // Turn LED on
        case 'a':
            digitalWrite(LED,HIGH);
            break;
        // Turn LED off
        case 'b':
            digitalWrite(LED,LOW);
            break;
        default:
            break;
        }
        
    }
    
}