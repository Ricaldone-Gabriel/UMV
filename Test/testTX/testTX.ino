// SimpleTxAckPayload - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


#define CE_PIN   12
#define CSN_PIN 13

const byte slaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

char dataToSend[10] = "Message 0";
char txNum = '0';
int ackData[3] = {-1, -1,-1}; // to hold the two values coming from the slave
bool newData = false;

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 100; // send once per second

//===============

void setup() {

    Serial.begin(115200);
    Serial.println("SimpleTxAckPayload Starting");

    radio.begin();
    radio.setDataRate(RF24_250KBPS);

    radio.enableAckPayload();

    radio.setRetries(5,5); // delay, count
                                       // 5 gives a 1500 µsec delay which is needed for a 32 byte ackPayload
    radio.openWritingPipe(slaveAddress);
}

//=============

void loop() {

    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
    }
    showData();
}

//================

void send() {

    bool rslt;
    rslt = radio.write( &dataToSend, sizeof(dataToSend) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2
    
    Serial.print("Data Sent ");
    Serial.print(dataToSend);
    Serial.print(rslt);
   
    if (radio.available()) {
      radio.read(&ackData, sizeof(ackData));
      newData = true;
    }
    else {
      Serial.println("  Acknowledge but no data ");
    }
    updateMessage();
    prevMillis = millis();
 }


//=================

void showData() {
    if (newData == true) {
        Serial.print("  Acknowledge data ");
        Serial.print(ackData[0]);
        Serial.print(", ");
        Serial.print(ackData[1]);
        Serial.print(", ");
        Serial.print(ackData[2]);
        Serial.println();
        newData = false;
    }
}

//================

void updateMessage() {
        // so you can see that new data is being sent
    txNum += 1;
    if (txNum > '9') {
        txNum = '0';
    }
    dataToSend[8] = txNum;
}