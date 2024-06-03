#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP32Servo.h>
#include <Wire.h>
#include "DHT.h"
#include "BlueDot_BME280.h"

#define CE_PIN 12
#define CSN_PIN 13
#define Sterzo_PIN 14
#define ESC_PIN 27
#define SDA_PIN 26
#define SCL_PIN 25
#define DHT11PIN 17

BlueDot_BME280 bme280 = BlueDot_BME280();
DHT dht(DHT11PIN, DHT11);
Servo ServoSterzo, ESC;
RF24 radio(CE_PIN, CSN_PIN);
const byte Address[5] = {'2', 'a', 'p', 'l', 'e'};

struct dati
{
  int angoloServo = 0;
  int potenzaMotore = 0;
};

dati datiRicevuti;

long int currentMillis = 0;
long int previousMillis = 0;

float ackData[3] = {1, 2, 3}; // the two values to be sent to the master
bool newData = false;

//==============

void setupMotor()
{
  currentMillis = millis();
  previousMillis = currentMillis;
  while (currentMillis - previousMillis < 500)
  {
    ESC.write(180);
    currentMillis = millis();
    Serial.println("mando 180");
  }
  previousMillis = currentMillis;
  while (currentMillis - previousMillis < 500)
  {
    ESC.write(0);
    currentMillis = millis();
    Serial.println("mando 0");
  }
  previousMillis = currentMillis;
  delay(3000);
}

void setup()
{

  Serial.begin(115200);
  ServoSterzo.attach(Sterzo_PIN);
  ESC.attach(ESC_PIN, 1000, 2000);

  Serial.println("SimpleRxAckPayload Starting");

  Wire.begin(SDA_PIN, SCL_PIN);
  bme280.parameter.communication = 0;        // Choose communication protocol
  bme280.parameter.I2CAddress = 0x76;        // Choose I2C Address
  bme280.parameter.sensorMode = 0b11;        // Choose sensor mode
  bme280.parameter.IIRfilter = 0b100;        // Setup for IIR Filter
  bme280.parameter.tempOversampling = 0b101; // Setup Temperature Ovesampling
  bme280.parameter.pressOversampling = 0b101;
  bme280.parameter.pressureSeaLevel = 1013.25;
  bme280.parameter.tempOutsideCelsius = 15; // default value of 15°C
  bme280.init();

  dht.begin();

  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, Address);
  radio.enableAckPayload();
  radio.startListening();
  radio.writeAckPayload(1, &ackData, sizeof(ackData)); // pre-load data
  delay(1500);
  // Setup motor
  setupMotor();
}

//==========

void loop()
{
  getData();
  useData();
}

//============

void getData()
{
  
  if (radio.available())
  {
    radio.read(&datiRicevuti, sizeof(datiRicevuti));
    updateReplyData();
    showData();
    newData = true;
  }
}

//================

void useData()
{
  if (newData == true)
  {
    ServoSterzo.write(datiRicevuti.angoloServo);
    ESC.write(datiRicevuti.potenzaMotore);
    newData = false;
  }
  else
  {
    datiRicevuti.potenzaMotore = 0;
  }
}

//================

void showData() {
  Serial.print("Data received ");
  Serial.println(String(datiRicevuti.angoloServo) + " " + String(datiRicevuti.potenzaMotore));
  Serial.println(" ackPayload sent ");
  Serial.print(ackData[0]);
  Serial.print(", ");
  Serial.print(ackData[1]);
  Serial.print(", ");
  Serial.println(ackData[2]);
}

//================
// T,H,P
void updateReplyData()
{
  ackData[0] = bme280.readTempC();
  ackData[1] = dht.readHumidity();
  ackData[2] = bme280.readPressure();
  radio.writeAckPayload(1, &ackData, sizeof(ackData)); // load the payload for the next time
}