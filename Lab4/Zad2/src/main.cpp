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
    if(!RTC.read(tm))
    {
        //Todo
    }

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