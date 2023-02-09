// Date, Time and Alarm functions using a DS3231 RTC connected via I2C and Wire lib

#include "DHT.h"
#define DHTTYPE DHT22 
#define DHTPIN 2 
#include <Wire.h>
#include <SPI.h>  // not used here, but needed to prevent a RTClib compile error
#include <RTClib.h>
#include <BH1750FVI.h>
#include <SD.h>

File myFile; 
const int chipSelect = D4;
RTC_DS3231 RTC;
BH1750FVI LightSensor(BH1750FVI::k_DevModeContLowRes);
DHT dht(DHTPIN, DHTTYPE);
int analogPin = A0; 
int val = 0;

void setup () {
    Serial.begin(9600);
    Wire.begin();
    RTC.begin();
    LightSensor.begin();
    dht.begin();
  RTC.adjust(DateTime(__DATE__, __TIME__));
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
    myFile = SD.open("Full_Test.txt", FILE_WRITE);
    myFile.close();
  
  if (SD.exists("Full_Test.txt")) {
    Serial.println("File exists.");
  } else {
    Serial.println("File doesn't exists.");
    goto createNew;
  }
  delay(3000);
  newWrite:
    myFile = SD.open("Full_Test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to Full_Test.txt...");
    myFile.close();
    Serial.println("done.");
    } else {
    Serial.println("error opening file");
    goto newWrite;
    }
    
}

void loop () {
    DateTime now = RTC.now();
    uint16_t lux = LightSensor.GetLightIntensity();
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    val = analogRead(analogPin);
    int cal_val = map(analogRead(analogPin),0,1024,100,0);
    delay(1000);
    Serial.print(now.day(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.year(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
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
    myFile = SD.open("Full_Test.txt", FILE_WRITE);
    if(myFile){
      Serial.println("Writing Date to Full_Test.txt...");
        myFile.print('/');
        myFile.print(now.month(), DEC);
        myFile.print(now.year(), DEC);
        myFile.print(' ');
        myFile.print(now.hour(), DEC);
        myFile.print(':');
        myFile.print(now.minute(), DEC);
        myFile.print(':');
        myFile.print(now.second(), DEC);
        myFile.print(' ');
        delay(500);
        myFile.close();
        Serial.println("done.");
  }
  delay(1000);
    myFile = SD.open("Full_Test.txt", FILE_WRITE);
    if(myFile){
      
      Serial.write(myFile.read());
      
      myFile.close();
    }
     else {
Serial.println("error opening test.txt");
}
  delay(3000);
    myFile = SD.open("Full_Test.txt", FILE_WRITE);
    if(myFile){
      Serial.println("Writing Temp to Full_Test.txt...");
        myFile.print("Temp = ");
        myFile.print(t);
        myFile.print(" °C");
        myFile.print(' ');
        myFile.print("Humid = ");
        myFile.print(h);
        myFile.print(" %");
        myFile.print(' ');
        delay(500);
        myFile.close();
        Serial.println("done.");
  }
  delay(1000);
  myFile = SD.open("Full_Test.txt", FILE_WRITE);
    if(myFile){
      
      Serial.write(myFile.read());
     
      myFile.close();
    }
    else {
Serial.println("error opening test.txt");
}
  delay(3000);
    myFile = SD.open("Full_Test.txt", FILE_WRITE);
    if(myFile){
      Serial.println("Writing Lux to Full_Test.txt...");
        myFile.print("Light = ");
        myFile.print(lux);
        myFile.print(" Lux");
        myFile.print(' ');
        delay(500);
        myFile.close();
        Serial.println("done.");
  }
  delay(1000);
  myFile = SD.open("Full_Test.txt", FILE_WRITE);
    if(myFile){
      
      Serial.write(myFile.read());
      
      myFile.close();
    }
    else {
Serial.println("error opening test.txt");
}
  delay(3000);
    myFile = SD.open("Full_Test.txt", FILE_WRITE);
    if(myFile){
      Serial.println("Writing Soil to Full_Test.txt...");
        myFile.print("Soil = "); 
        myFile.print(cal_val);
        myFile.print(" %");
        myFile.print(" (");
        myFile.print(val);
        myFile.print(")");
        delay(500);
        myFile.close();
        Serial.println("done.");
  }
  delay(1000);
  myFile = SD.open("Full_Test.txt", FILE_WRITE);
    if(myFile){
      
      Serial.write(myFile.read());
      
      myFile.close();
    }
    else {
Serial.println("error opening test.txt");
}
  delay(3000);
    Serial.println();
    delay(10000);
}
