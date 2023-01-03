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
char *dayTime[3];
unsigned long total_seconds;
bool non_digit = false;
bool blink = false;
int wait = 0;
bool previous = false;
bool alarm_on = false;

struct time
{
    unsigned long hour;
    unsigned long min;
    unsigned long sec;
} alarm;

void setup()
{
    lcd.init();
    lcd.backlight();
    lcd.print("Initialized");

    Serial.begin(9600);
    Serial.println("Initialized");
    delay(2000);
    lcd.clear();

    pinMode(2, OUTPUT);
    digitalWrite(2, LOW);
    pinMode(4, INPUT_PULLUP);
}

void loop()
{
    while(Serial.available() != 0){
        text = Serial.readString();
        text.toCharArray(buf, text.length()+1);


        dayTime[0] = strtok(buf, ":");
        for(int i=1; i < 3; i++){
            dayTime[i] = strtok(NULL, ":");
        }
    

        for(int i = 0; i < 3; i++)
        {
            //Serial.println(dayTime[i]);
            for(int j = 0; j < 2; j++)
            {
                if(!isdigit(*dayTime[i]))
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
        else if(atoi(dayTime[0]) >= 24 || atoi(dayTime[0]) < 0 || atoi(dayTime[1]) >= 60 || atoi(dayTime[1]) < 0 || atoi(dayTime[2]) >= 60 || atoi(dayTime[2]) < 0){
            Serial.println("Poza zakresem");
        }
        else
        {
            alarm.hour = atoi(dayTime[0]);
            alarm.min = atoi(dayTime[1]);
            alarm.sec = atoi(dayTime[2]);
            Serial.print("Alarm set to:");
            Serial.print(alarm.hour);
            Serial.print(":");
            Serial.print(alarm.min);
            Serial.print(":");
            Serial.println(alarm.sec);
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

    if(tm.Hour == alarm.hour && tm.Minute == alarm.min && tm.Second == alarm.sec && !alarm_on)
    {
        alarm_on = true;
    }
        


    if(alarm_on)
    {
       if(wait < 10)
       {
           lcd.backlight();
            digitalWrite(2, HIGH);
           wait++;
       }
       else if(wait < 20)
       {
           lcd.noBacklight();
           digitalWrite(2, LOW);
           wait++;
       }
       else
       {
           wait = 0;
       }
          
    }

    if(digitalRead(4) && digitalRead(4) != previous){
        alarm_on = false;
        digitalWrite(2, LOW);
        lcd.noBacklight();
    }
    
    previous = digitalRead(4);
    delay(10);
}