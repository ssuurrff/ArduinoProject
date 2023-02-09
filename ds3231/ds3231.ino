// Date, Time and Alarm functions using a DS3231 RTC connected via I2C and Wire lib

#include "DHT.h"
#define DHTTYPE DHT22 
#define DHTPIN 2 
#include <Wire.h>
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include <RTClib.h>
#include <BH1750FVI.h>
RTC_DS3231 RTC;
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);
DHT dht(DHTPIN, DHTTYPE);
int analogPin = A0; 
int val = 0;
int hourNow, minuteNow, secondNow;

void setup () {
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
    LightSensor.begin();
    dht.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  DateTime now = RTC.now();
  RTC.setAlarm1Simple(21, 58);
  RTC.turnOnAlarm(1);
  if (RTC.checkAlarmEnabled(1)) {
    Serial.println("Alarm Enabled");
  }
}

void loop () {
    DateTime now = RTC.now();
    DateTime nowTime = RTC.now();
    secondNow = nowTime.second();
    minuteNow = nowTime.minute();
    hourNow = nowTime.hour();
    uint16_t lux = LightSensor.GetLightIntensity();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    val = analogRead(analogPin);
    int cal_val = map(analogRead(analogPin),0,1024,100,0);
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    /*Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);*/
    if(hourNow < 10)(Serial.print("0"));
    Serial.print(hourNow);
    Serial.print(":");
    if(minuteNow < 10)(Serial.print("0"));
    Serial.print(minuteNow);
    Serial.print(":");
    if(secondNow < 10)(Serial.print("0"));
    Serial.print(secondNow);
    Serial.print(' ');
    Serial.print("Temp = ");
    Serial.print(t);
    Serial.print(" °C");
    Serial.print(' ');
    Serial.print("Humid = ");
    Serial.print(h);
    Serial.print(" %");
    Serial.print(' ');
    Serial.print("Light = ");
    Serial.print(lux);
    Serial.print(" Lux");
    Serial.print(' ');
    Serial.print("Soil = "); 
    Serial.print(cal_val);
    Serial.print(" %");
    Serial.print(" (");
    Serial.print(val);
    Serial.print(")");
    Serial.println();

    if (RTC.checkIfAlarm(1)) {
      Serial.println("Alarm Triggered");
    }
    Serial.println();
    delay(3000);
}
