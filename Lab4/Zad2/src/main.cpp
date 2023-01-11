#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <TimeLib.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
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

struct RTC
{
    byte hour;
    byte minute;
    byte sec;
    byte day;
    byte weekday;
    byte month;
    byte year;
    bool century;
}RTC_Time;

#define RTCC_R 0xA3
#define SQW_32KHZ B10000000
#define RTCC_STAT1_ADDR     0x0
#define RTCC_VLSEC_MASK     0x80
#define RTCC_CENTURY_MASK   0x80
#define RTCC_SEC_ADDR       0x02
extern TwoWire Wire;
int Rtcc_Addr;

void Rtc_Pcf8563(void)
{
    Wire.begin();
    Rtcc_Addr = RTCC_R>>1;
}


byte decToBcd(byte val)
{
    return ( (val/10*16) + (val%10) );
}

byte bcdToDec(byte val)
{
    return ( (val/16*10) + (val%16) );
}

void zeroClock()
{
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write((byte)0x0);        // start address

    Wire.write((byte)0x0);     //control/status1
    Wire.write((byte)0x0);     //control/status2
    Wire.write((byte)0x00);    //set seconds to 0 & VL to 0
    Wire.write((byte)0x00);    //set minutes to 0
    Wire.write((byte)0x00);    //set hour to 0
    Wire.write((byte)0x01);    //set day to 1
    Wire.write((byte)0x00);    //set weekday to 0
    Wire.write((byte)0x81);    //set month to 1, century to 1900
    Wire.write((byte)0x00);    //set year to 0
    Wire.write((byte)0x80);    //minute alarm value reset to 00
    Wire.write((byte)0x80);    //hour alarm value reset to 00
    Wire.write((byte)0x80);    //day alarm value reset to 00
    Wire.write((byte)0x80);    //weekday alarm value reset to 00
    Wire.write((byte)SQW_32KHZ); //set SQW to default, see: setSquareWave
    Wire.write((byte)0x0);     //timer off
    Wire.endTransmission();
}

void getDateTime(void)
{
    /* Start at beginning, read entire memory in one go */
    Wire.beginTransmission(Rtcc_Addr);
    Wire.write((byte)RTCC_STAT1_ADDR);
    Wire.endTransmission();

    /* As per data sheet, have to read everything all in one operation */
    uint8_t readBuffer[16] = {0};
    Wire.requestFrom(Rtcc_Addr, 16);
    for (uint8_t i=0; i < 16; i++)
        readBuffer[i] = Wire.read();


    // time bytes
    //0x7f = 0b01111111
    RTC_Time.sec = bcdToDec(readBuffer[2] & ~RTCC_VLSEC_MASK);
    RTC_Time.minute = bcdToDec(readBuffer[3] & 0x7f);
    //0x3f = 0b00111111
    RTC_Time.hour = bcdToDec(readBuffer[4] & 0x3f);

    // date bytes
    //0x3f = 0b00111111
    RTC_Time.day = bcdToDec(readBuffer[5] & 0x3f);
    //0x07 = 0b00000111
    RTC_Time.weekday = bcdToDec(readBuffer[6] & 0x07);
    //get raw month data byte and set month and century with it.
    RTC_Time.month = readBuffer[7];
    if (RTC_Time.month & RTCC_CENTURY_MASK)
        RTC_Time.century = true;
    else
        RTC_Time.century = false;
    //0x1f = 0b00011111
    RTC_Time.month = RTC_Time.month & 0x1f;
    RTC_Time.month = bcdToDec(RTC_Time.month);
    RTC_Time.year = bcdToDec(readBuffer[8]);
}


void setDateTime(byte day, byte weekday, byte month,
                              bool century, byte year, byte hour,
                              byte minute, byte sec)
{
    /* year val is 00 to 99, xx
        with the highest bit of month = century
        0=20xx
        1=19xx
        */
    month = decToBcd(month);
    if (century)
        month |= RTCC_CENTURY_MASK;
    else
        month &= ~RTCC_CENTURY_MASK;

    /* As per data sheet, have to set everything all in one operation */
    Wire.beginTransmission(Rtcc_Addr);    // Issue I2C start signal
    Wire.write(RTCC_SEC_ADDR);       // send addr low byte, req'd
    Wire.write(decToBcd(sec) &~RTCC_VLSEC_MASK); //set sec, clear VL bit
    Wire.write(decToBcd(minute));    //set minutes
    Wire.write(decToBcd(hour));        //set hour
    Wire.write(decToBcd(day));            //set day
    Wire.write(decToBcd(weekday));    //set weekday
    Wire.write(month);                 //set month, century to 1
    Wire.write(decToBcd(year));        //set year to 99
    Wire.endTransmission();
    // Keep values in-sync with device
    getDateTime();
}

void setup()
{
    lcd.init();
    lcd.backlight();
    lcd.print("Initialized");

    zeroClock();

    Serial.begin(9600);
    Serial.println("Initialized");
    delay(1000);
    lcd.clear();
}

void loop()
{
    getDateTime();
    lcd.setCursor(0, 0);
    if(RTC_Time.hour < 10)
        lcd.print("0");
    lcd.print(RTC_Time.hour);
    lcd.print(":");
    if(RTC_Time.minute < 10)
        lcd.print("0");
    lcd.print(RTC_Time.minute);
    lcd.print(":");
    if(RTC_Time.sec < 10)
        lcd.print("0");
    lcd.print(RTC_Time.sec);
    lcd.setCursor(0,1);
    if(RTC_Time.day < 10)
        lcd.print("0");
    lcd.print(RTC_Time.day);
    lcd.print("/");
    if(RTC_Time.month < 10)
        lcd.print("0");
    lcd.print(RTC_Time.month);
    lcd.print("/");
    lcd.print(RTC_Time.year);

    delay(10);
}