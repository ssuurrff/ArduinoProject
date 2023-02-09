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

void setup() {
  Serial.begin(9600);
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
    myFile = SD.open("Lux+Temp+Soil_Test.txt", FILE_WRITE);
    myFile.close();
  
  if (SD.exists("Lux+Temp+Soil_Test.txt")) {
    Serial.println("File exists.");
  } else {
    Serial.println("File doesn't exists.");
    goto createNew;
  }
  delay(3000);
  newWrite:
    myFile = SD.open("Lux+Temp+Soil_Test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to Lux+Temp+Soil_Test.txt...");
    myFile.close();
    Serial.println("done.");
    } else {
    Serial.println("error opening file");
    goto newWrite;
    }
    
}

void loop() {
  
  uint16_t lux = LightSensor.GetLightIntensity();
  delay(100);
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  val = analogRead(analogPin);
  int cal_val = map(analogRead(analogPin),0,1024,100,0);
  
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
  Serial.println();
  
  Serial.println();
    myFile = SD.open("Lux+Temp+Soil_Test.txt", FILE_WRITE);
    if(myFile){
      Serial.println("Writing Lux+Temp+Soil to Lux+Temp_Test.txt...");
        myFile.print("Light = ");
        myFile.print(lux);
        myFile.print(" Lux");
        myFile.print(' ');
        //myFile.print("Wait-3");
        myFile.print("Temp = ");
        myFile.print(t);
        myFile.print(" °C");
        myFile.print(' ');
        myFile.print("Humid = ");
        myFile.print(h);
        myFile.print(" %");
        myFile.print(' ');
        //myFile.print("Wait-4");
        myFile.print("Soil = "); 
        myFile.print(cal_val);
        myFile.print(" %");
        myFile.print(' ');
        myFile.println();
        myFile.close();
        Serial.println("done.");
  }

  delay(3000);
}
