#include <Arduino.h>

#define LED1 13
#define LED2 13
#define YEAR 2022
#define A_POT1 A0


void setup()
{
    // Led pin setup
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
}

void loop()
{
    int interval = 1000;
    unsigned long ptime = 0;
    unsigned long ptime2 = 0;
    char menu;
    bool ledFlag = false;
    bool blinkFlag = false;
    bool bdayFlag = false;
    bool voltFlag = false;
    float volt = 0.0;

    // Serial comunication initialization
    Serial.begin(9600);

    while (1)
    {
        // Led blinking
        if (blinkFlag)
        {
            if (ptime < (millis() - interval))
            {
                digitalWrite(LED2, LOW);
                ptime = millis(); // Update time
            }
            if (ptime < (millis() - (interval / 2)))
            {
                digitalWrite(LED2, HIGH);
            }
        }

        // Analog cyclic measurement
        if (voltFlag)
        {
            if (ptime2 < (millis() - interval))
            {
                // Make analog reading and convert to volts
                volt = analogRead(A_POT1) / 204.8;
                // Print to serial
                Serial.print("Napiecie:");
                Serial.print(volt);
                Serial.println("V");

                // Update time
                ptime2 = millis();
            }
        }

        // Age calculation
        if (bdayFlag)
        {
            int year, age = 0;

            if (Serial.available()) // Wait for input
            {
                year = Serial.parseInt(); // Get year from Serial
                if (year > 0)
                {
                    // Calculate age
                    age = YEAR - year;

                    // Print age to Serial
                    Serial.print("Twój wiek: ");
                    Serial.print(age);
                    Serial.println(" lata");
                    // Set the flag
                    bdayFlag = false;
                }
            }
        }

        menu = Serial.read();
        switch (menu)
        {
        // Print 'hello wolrd' message
        case 'a':
        {
            Serial.println("Hello world");
            break;
        }
        // Turn on/off LED1
        case 'b':
        {
            if (ledFlag)
            {
                // Turn led ON and change flag
                digitalWrite(LED1, LOW);
                ledFlag = false;
            }
            else
            {
                // Turn led OFF and change flag
                digitalWrite(LED1, HIGH);
                ledFlag = true;
            }
            break;
        }
        // Calculate age
        case 'c':
        {
            Serial.println("Podaj rok urodzenia");
            bdayFlag = true;
            break;
        }
        // Turn on/off blinking LED2
        case 'd':
        {
            // Change blink flag
            if (blinkFlag)
                blinkFlag = false;
            else
                blinkFlag = true;
            break;
        }
        // Make one voltage measurement
        case 'e':
        {
            // Make analog reading and convert to volts
            volt = analogRead(A_POT1) / 204.8;
            // Print to Serial
            Serial.print("Napiecie:");
            Serial.print(volt);
            Serial.println("V");
            break;
        }
        // Turn on voltage measurement
        case 'f':
        {
            voltFlag = true;
            break;
        }
        // Turn off voltage measurement
        case 'x':
        {
            voltFlag = false;
            break;
        }
        // Display menu
        case '?':
        {
            Serial.print("a - 'Hello world'\nb - LED1 ON/OFF\nc - liczenie wieku\nd - miganie LED2\n? - wyswietlanie opcji\n");
            break;
        }
        default:
            break;
        }
    }
}