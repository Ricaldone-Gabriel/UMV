#include <Wire.h>
#include "BlueDot_BME280.h"

BlueDot_BME280 bme280 = BlueDot_BME280();

void setup()
{
  Serial.begin(115200);
  Wire.begin(26, 25);  //sda scl

  bme280.parameter.communication = 0;                  //Choose communication protocol
  bme280.parameter.I2CAddress = 0x76;                  //Choose I2C Address
  bme280.parameter.sensorMode = 0b11;                   //Choose sensor mode
  bme280.parameter.IIRfilter = 0b100;                    //Setup for IIR Filter
  bme280.parameter.tempOversampling = 0b101;             //Setup Temperature Ovesampling
  bme280.parameter.pressOversampling = 0b101;  
  bme280.parameter.pressureSeaLevel = 1013.25; 
  bme280.parameter.tempOutsideCelsius = 15;              //default value of 15°C
  bme280.init();
}

void loop()
{
  Serial.print("Temperature in Celsius:\t\t");
  Serial.println(bme280.readTempC());
  delay(10);
  Serial.print(F("Pressure Sensor 1 [hPa]:\t\t")); 
  Serial.println(bme280.readPressure());
  delay(10);
  Serial.print(F("Altitude Sensor 1 [m]:\t\t\t")); 
  Serial.println(bme280.readAltitudeMeter());
  delay(10);
}