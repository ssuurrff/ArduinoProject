#include "DHT.h"
#define DHTTYPE DHT22 
#define DHTPIN 2 
#include <Wire.h>
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include <RTClib.h>
#include <BH1750FVI.h>
#include <SD.h>

RTC_DS3231 RTC;
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);
DHT dht(DHTPIN, DHTTYPE);
int analogPin = A0; 
int val = 0;
File myFile; 
const int chipSelect = D0;
int hourNow, minuteNow, secondNow;


void setup() {
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  DateTime now = RTC.now();
  RTC.setAlarm1Simple(21, 58);
  RTC.turnOnAlarm(1);
  if (RTC.checkAlarmEnabled(1)) {
    Serial.println("Alarm Enabled");
  }
  LightSensor.begin();
  dht.begin();
  Serial.println();
  Serial.println("Initializing SD card...");
  delay(300);
  pinMode(SS, OUTPUT);
  
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
    }
    Serial.println("initialization done.");
  createNew:
    myFile = SD.open("Excel_Test.txt", FILE_WRITE);
    myFile.close();
  
  if (SD.exists("Excel_Test.txt")) {
    Serial.println("File exists.");
  } else {
    Serial.println("File doesn't exists.");
    goto createNew;
  }
  delay(3000);
  newWrite:
    myFile = SD.open("Excel_Test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to Excel_Test.txt...");
    myFile.close();
    Serial.println("done.");
    } else {
    Serial.println("error opening file");
    goto newWrite;
    }
    
}

void loop() {
  DateTime now = RTC.now();
  DateTime nowTime = RTC.now();
  secondNow = nowTime.second();
  minuteNow = nowTime.minute();
  hourNow = nowTime.hour();
  uint16_t lux = LightSensor.GetLightIntensity();
  delay(100);
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
  Serial.print("Light = ");
  Serial.print(lux);
  Serial.print(" Lux");
  Serial.print(' ');
    Serial.print("Temp = ");
    Serial.print(t);
    Serial.print(" °C");
    Serial.print(' ');
    Serial.print("Humid = ");
    Serial.print(h);
    Serial.print(" %");
    Serial.print(' ');
      Serial.print("Soil = ");  
      Serial.print(cal_val);
      Serial.print(" %");
      
  //Serial.println();
  
  Serial.println();
  
    myFile = SD.open("Excel_Test.txt", FILE_WRITE);
    if(myFile){
     
      Serial.println("Writing RTC_Lux+Temp+Soil to Excel_Test.txt...");
        myFile.print(now.day(), DEC);
        myFile.print('/');
        myFile.print(now.month(), DEC);
        myFile.print('/');
        myFile.print(now.year(), DEC);
        myFile.print(' ');
       /* myFile.print(now.hour(), DEC);
        myFile.print(':');
        myFile.print(now.minute(), DEC);
        myFile.print(':');
        myFile.print(now.second(), DEC);*/
        if(hourNow < 10)(myFile.print("0"));
        myFile.print(hourNow);
        myFile.print(":");
        if(minuteNow < 10)(myFile.print("0"));
        myFile.print(minuteNow);
        myFile.print(":");
        if(secondNow < 10)(myFile.print("0"));
        myFile.print(secondNow);
        myFile.print("\t");
        myFile.print("Light = ");
        myFile.print(lux);
        myFile.print(" Lux");
        myFile.print("\t");
        //myFile.print("Wait-3");
        myFile.print("Temp = ");
        myFile.print(t);
        myFile.print(" °C");
        myFile.print("\t");
        myFile.print("Humid = ");
        myFile.print(h);
        myFile.print(" %");
        myFile.print("\t");
        //myFile.print("Wait-4");
        myFile.print("Soil = "); 
        myFile.print(cal_val);
        myFile.print(" %");
        //myFile.print("\t");
        //myFile.print("Wait-5");
        //myFile.print("Wait-6");
        myFile.println();
        myFile.close();
        Serial.println("done.");
  }

  delay(300000); //แก้เวลาบันทึกข้อมูล 10 นาที = 6 แสน 
  
}
