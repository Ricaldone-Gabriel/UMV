#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <ESP32Servo.h>

#define CE_PIN   12
#define CSN_PIN 13
#define Sterzo_PIN 14
#define ESC_PIN 27
Servo ServoSterzo, ESC;
RF24 radio(CE_PIN, CSN_PIN);
const byte Address[5] = {'2','a','p','l','e'};

struct dati{
  int angoloServo = 0;
  int potenzaMotore = 0;
};

dati datiRicevuti;

float ackData[3] = {1,2,3}; // the two values to be sent to the master
bool newData = false;

//==============

void setup() {

    Serial.begin(115200);
    ServoSterzo.attach(Sterzo_PIN);
    ESC.attach(ESC_PIN,1000,2000);
    Serial.println("SimpleRxAckPayload Starting");

    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.openReadingPipe(1, Address);
    radio.enableAckPayload();
    radio.startListening();
    radio.writeAckPayload(1, &ackData, sizeof(ackData)); // pre-load data
}

//==========

void loop() {
    getData();
    useData();
}


//============

void getData() {
    if (radio.available() ) {
        radio.read( &datiRicevuti, sizeof(datiRicevuti) );
        updateReplyData();
        //showData();
        newData = true;
    }
}

//================

void useData() {
    if(newData == true) {
      ServoSterzo.write(datiRicevuti.angoloServo);
      ESC.write(datiRicevuti.potenzaMotore);
      newData = false;
    } else {
      datiRicevuti.potenzaMotore = 0;
    }
    

}

//================
/*
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
*/
//================

void updateReplyData() {
    ackData[0] = ackData[0]+1;
    ackData[1] = ackData[1]+1;
    ackData[2] = ackData[2]+1;
    radio.writeAckPayload(1, &ackData, sizeof(ackData)); // load the payload for the next time
}