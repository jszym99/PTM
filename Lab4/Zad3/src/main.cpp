#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Rtc_Pcf8563.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Rtc_Pcf8563 rtc;
tmElements_t tm;

String text = "";
char buf[10];
char *date[6];
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

        date[0] = strtok(buf, ":");
        for(int i=1; i < 2; i++){
            date[i] = strtok(NULL, ":");
        }
        date[2] = strtok(NULL, " ");
        for(int i=3; i < 6; i++){
            date[i] = strtok(NULL, "/");
        }
    

        for(int i = 0; i < 6; i++)
        {
            //Serial.println(date[i]);
            for(int j = 0; j < 2; j++)
            {
                if(!isdigit(*date[i]))
                {
                    non_digit = true;
                }
            }
        }
        
        if(non_digit)
        {
            Serial.println("Zla forma :(");
            non_digit = false;
        }
        else if(atoi(date[0]) >= 24 || atoi(date[0]) < 0 || atoi(date[1]) >= 60 || atoi(date[1]) < 0 || atoi(date[2]) >= 60 || atoi(date[2]) < 0
        || atoi(date[3]) > 31 || atoi(date[3]) <= 0 || atoi(date[4]) > 12 || atoi(date[4]) <= 0 || atoi(date[5]) >= 2160 || atoi(date[5]) < 2000){
            Serial.println("Poza zakresem");
        }
        else
        {
            tm.Hour = atoi(date[0]);
            tm.Minute = atoi(date[1]);
            tm.Second = atoi(date[2]);
            tm.Day = atoi(date[3]);
            tm.Month = atoi(date[4]);
            tm.Year = CalendarYrToTm(atoi(date[5]));
            RTC.write(tm);
        }
    }

    RTC.read(tm);

    lcd.setCursor(0, 0);
    if(tm.Hour < 10)
        lcd.print("0");
    lcd.print(tm.Hour);
    lcd.print(":");
    if(tm.Minute < 10)
        lcd.print("0");
    lcd.print(tm.Minute);
    lcd.print(":");
    if(tm.Second < 10)
        lcd.print("0");
    lcd.print(tm.Second);
    lcd.setCursor(0,1);
    if(tm.Day < 10)
        lcd.print("0");
    lcd.print(tm.Day);
    lcd.print("/");
    if(tm.Month < 10)
        lcd.print("0");
    lcd.print(tm.Month);
    lcd.print("/");
    lcd.print(tmYearToCalendar(tm.Year));
    
    delay(10);
}