#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

#define LED1 7
#define LED2 8
#define BLINK1 500
#define BLINK2 2000
#define A_PHOTO A0
#define A_POT A1
#define THRESHOLD 0.5
#define R_PIN 9
#define G_PIN 10
#define B_PIN 11
#define ENC_A 2
#define ENC_B 3
#define ENC_SW 4

unsigned long ptime1 = millis();
unsigned long ptime2 = millis();
unsigned long ptime3 = millis();
unsigned long ptime4 = millis();
unsigned long ptime5 = millis();
unsigned long ptime6 = millis();
double photo = 0;
double pot = 0;
long temperature = 0;
bool threshold = false;
bool twostate = false;
int option = 0, poption = 0;
bool CW = 0, CCW = 0, pCW = 0, pCCW = 0;

int R = 255;
int G = 0;
int B = 0;

// LED1 1Hz Blinking
void Blink1()
{
    if (millis() >= (ptime1 + BLINK1))
    {
        digitalWrite(LED1, !digitalRead(LED1));
        ptime1 = millis();
    }
}

// LED2 0.25Hz Blinking
void Blink2()
{
    if (millis() >= (ptime2 + BLINK2))
    {
        digitalWrite(LED2, !digitalRead(LED2));
        ptime2 = millis();
    }
}

// Two-state and threshold controllers
void Controllers()
{
    if (millis() >= (ptime3 + 1000))
    {
        // Read analog values
        photo = analogRead(A_PHOTO) * 5.0 / 1024;
        pot = analogRead(A_POT) * 5.0 / 1024;

        // Print regulator values
        Serial.print("Setpoint: ");
        Serial.println(pot);
        Serial.print("Current Value: ");
        Serial.println(photo);

        // Two-state controller
        if (twostate && photo <= pot)
            twostate = false;

        if (!twostate && photo > pot)
            twostate = true;

        // Threshold controller
        if (threshold && photo <= (pot - THRESHOLD))
            threshold = false;

        if (!threshold && photo >= (pot + THRESHOLD))
            threshold = true;

        // Print controller states
        Serial.print("Two-state controller state:");
        Serial.println(twostate);
        Serial.print("Threshold controller state:");
        Serial.println(threshold);

        ptime3 = millis();
    }
}

// RGB LED "Rainbow"
void RGB()
{
    if (millis() >= (ptime4 + 5))
    {
        if (R == 255 && G == 0 && B < 255)
            B++;

        if (R > 0 && G == 0 && B == 255)
            R--;

        if (R == 0 && G < 255 && B == 255)
            G++;

        if (R == 0 && G == 255 && B > 0)
            B--;

        if (R < 255 && G == 255 && B == 0)
            R++;

        if (R == 255 && G > 0 && B == 0)
            G--;

        analogWrite(R_PIN, R);
        analogWrite(G_PIN, G);
        analogWrite(B_PIN, B);

        // For debuging
        Serial.print(R);
        Serial.print(",");
        Serial.print(G);
        Serial.print(",");
        Serial.print(B);
        Serial.print("\n");

        ptime4 = millis();
    }
}

void Temp()
{
    if (millis() >= (ptime5 + 100))
    {
        temperature = analogRead(A_POT) * 5.0 * 20 / 1024;
        Serial.print("Temperature: ");
        Serial.println(temperature);

        // Green
        if (temperature <= 50)
        {
            R = 1;
            G = map(temperature, 0, 50, 1, 255);
            B = 1;
        }
        // Blue
        else if (temperature > 50 && temperature <= 70)
        {
            R = 1;
            G = map(temperature, 50, 70, 255, 1);
            B = map(temperature, 50, 70, 1, 255);
        }
        // Violet
        else if (temperature > 70 && temperature <= 80)
        {
            R = map(temperature, 70, 80, 1, 63);
            G = 1;
            B = 255;
        }
        // Yellow
        else if (temperature > 80 && temperature <= 90)
        {
            R = map(temperature, 80, 95, 63, 127);
            G = map(temperature, 80, 90, 1, 255);
            B = map(temperature, 80, 90, 255, 1);
        }
        // Red
        else if (temperature > 90 && temperature <= 100)
        {
            R = map(temperature, 90, 100, 127, 255);
            G = map(temperature, 90, 100, 255, 1);
            B = 1;
        }

        analogWrite(R_PIN, R);
        analogWrite(G_PIN, G);
        analogWrite(B_PIN, B);

        // For debuging
        Serial.print(R);
        Serial.print(",");
        Serial.print(G);
        Serial.print(",");
        Serial.print(B);
        Serial.print("\n");

        ptime5 = millis();
    }
}

void setup()
{
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);

    lcd.init();
    lcd.backlight();
    lcd.print("Initialized");

    Serial.begin(9600);
    Serial.println("Initialized");
}

void loop()
{
    // LED1 blinking
    Blink1();

    // LED2 blinking
    Blink2();

    // Controllers (with 100ms delay)
    // Controllers();

    // RGB "Rainbow"
    // RGB();

    // Temp();

    // Encoder reading
    if (millis() >= (ptime6 + 100))
    {
        CW = digitalRead(ENC_A);
        CCW = digitalRead(ENC_B);

        // Move up
        if (CW && !pCW && !CCW && !pCCW && option < 2)
            option++;

        // Move down
        else if (!CW && !pCW && CCW && !pCCW && option > 0)
            option--;

        pCW = CW;
        pCCW = CCW;

        ptime6 = millis();
    }


    if (option != poption)
    {
        switch (option)
        {
        case 0:
            lcd.clear();
            lcd.print("Controllers");
            R = 0;
            G = 0;
            B = 0;
            analogWrite(R_PIN, R);
            analogWrite(G_PIN, G);
            analogWrite(B_PIN, B);
            Controllers();
            break;
        case 1:
            lcd.clear();
            lcd.print("Rainbow");
            R = 255;
            G = 0;
            B = 0;
            RGB();
            break;
        case 2:
            lcd.clear();
            lcd.print("Temperature");
            R = 0;
            G = 0;
            B = 0;
            Temp();
            break;
        default:
            break;
        }
        poption = option;
    }

    switch (option)
        {
        case 0:
            Controllers();
            break;
        case 1:
            RGB();
            break;
        case 2:
            Temp();
            break;
        default:
            break;
        }
    
}