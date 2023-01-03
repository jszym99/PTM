#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
String text = "";
char buf[10];
char *help[3];
unsigned long total_seconds;
bool non_digit = false;

struct time
{
    unsigned long hour;
    unsigned long min;
    unsigned long sec;
} ctime;

void setup()
{
    lcd.init();
    lcd.backlight();
    lcd.print("Initialized");

    Serial.begin(9600);
    Serial.println("Initialized");
    delay(1000);
    lcd.clear();
}

void loop()
{
    while(Serial.available() != 0){
        text = Serial.readString();
        text.toCharArray(buf, text.length()+1);

    
        //Serial.println(buf);

        help[0] = strtok(buf, ":");
        for(int i=1; i < 3; i++){
            help[i] = strtok(NULL, ":");
        }

        for(int i = 0; i < 3; i++)
        {
            for(int j = 0; j < 2; j++)
            {
                if(!isdigit(*help[i]))
                {
                    non_digit = true;
                }
            }
        }
        

        if(atoi(help[0]) >= 24 || atoi(help[0]) < 0 || atoi(help[1]) >= 60 || atoi(help[1]) < 0 || atoi(help[2]) >= 60 || atoi(help[2]) < 0 || non_digit){
            Serial.println("Zla forma :(");
            non_digit = false;
        }
        else
        {
            total_seconds = atol(help[0])*3600 + atol(help[1])*60 + atol(help[2]);
        }
    }
    
    ctime.hour = (total_seconds/3600);
    ctime.min = (total_seconds - (ctime.hour*3600))/60;
    ctime.sec = (total_seconds - (ctime.hour*3600) - ctime.min*60);

    if(ctime.hour < 10)
        Serial.print("0");
    Serial.print(ctime.hour);
    Serial.print(":");
    if(ctime.min < 10)
        Serial.print("0");
    Serial.print(ctime.min);
    Serial.print(":");
    if(ctime.sec < 10)
        Serial.print("0");
    Serial.println(ctime.sec);

    //lcd.setCursor(0,0);
    if(ctime.hour < 10)
        lcd.print("0");
    lcd.print(ctime.hour);
    lcd.print(":");
    if(ctime.min < 10)
        lcd.print("0");
    lcd.print(ctime.min);
    lcd.print(":");
    if(ctime.sec < 10)
        lcd.print("0");
    lcd.print(ctime.sec);

    delay(1000);
    if(total_seconds == 86399)
    {
        total_seconds = 0;
    }
    else
    {
        total_seconds++;
    }
    
    lcd.clear();
}
