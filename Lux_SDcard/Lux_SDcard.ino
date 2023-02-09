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
const int chipSelect = D4;

void setup() {
  Serial.begin(9600);
  LightSensor.begin();

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
    myFile = SD.open("Lux_Test.txt", FILE_WRITE);
    myFile.close();
  
  if (SD.exists("Lux_Test.txt")) {
    Serial.println("File exists.");
  } else {
    Serial.println("File doesn't exists.");
    goto createNew;
  }
  delay(3000);
  newWrite:
    myFile = SD.open("Lux_Test.txt", FILE_WRITE);
  if (myFile) {
    Serial.print("Writing to Lux_Test.txt...");
    myFile.close();
    Serial.println("done.");
    } else {
    Serial.println("error opening file");
    goto newWrite;
    }
    
}

void loop() {
  
  uint16_t lux = LightSensor.GetLightIntensity();
  Serial.print("Light = ");
  Serial.print(lux);
  Serial.print(" Lux");
  Serial.println();

  Serial.println();
    myFile = SD.open("Lux_Test.txt", FILE_WRITE);
    if(myFile){
      Serial.println("Writing Lux to Lux_Test.txt...");
        myFile.print("Light = ");
        myFile.print(lux);
        myFile.print(" Lux");
        myFile.print(' ');
        myFile.println();
        myFile.close();
        Serial.println("done.");
  }

  delay(3000);
}
