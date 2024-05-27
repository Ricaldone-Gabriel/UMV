#include <Bluepad32.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <WiFi.h>

#define CE_PIN   12
#define CSN_PIN 13
#define ID 1 //ID of the UMV, MODIFY

ControllerPtr myControllers[BP32_MAX_GAMEPADS];
RF24 radio(CE_PIN,CSN_PIN); // CE, CSN
const byte Address[5] = {'2','a','p','l','e'};

const char* ssid = "testnetwork";
const char* password = "testPassword";
const IPAddress serverIP = IPAddress(151,60,143,184);

struct dati{
  int angoloServo = 90;
  int potenzaMotore = 0;
};
float ackData[3] = {-100,-100,-100};

bool newData = false;
dati datiTrasmessi;

void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Connected to AP successfully!");
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("Disconnected from WiFi access point");
  Serial.print("WiFi lost connection. Reason: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Trying to Reconnect");
  WiFi.begin(ssid, password);
}

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void onConnectedController(ControllerPtr ctl) {
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == nullptr) {
            Serial.printf("CALLBACK: Controller is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            ControllerProperties properties = ctl->getProperties();
            Serial.printf("Controller model: %s, VID=0x%04x, PID=0x%04x\n", ctl->getModelName().c_str(), properties.vendor_id,
                           properties.product_id);
            myControllers[i] = ctl;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot) {
        Serial.println("CALLBACK: Controller connected, but could not found empty slot");
    }
}

void onDisconnectedController(ControllerPtr ctl) {
    bool foundController = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
        if (myControllers[i] == ctl) {
            Serial.printf("CALLBACK: Controller disconnected from index=%d\n", i);
            myControllers[i] = nullptr;
            foundController = true;
            datiTrasmessi.potenzaMotore = 0;
            break;
        }
    }

    if (!foundController) {
        Serial.println("CALLBACK: Controller disconnected, but not found in myControllers");
    }
}

void dumpGamepad(ControllerPtr ctl) {
    Serial.printf(
    "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4d, %4d, axis R: %4d, %4d, brake: %4d, throttle: %4d, "
    "misc: 0x%02x, a: %d, x: %d, y: %d, b: %d\n",  // Aggiungi %d per ctl->a()
      ctl->index(),        
      ctl->dpad(),         
      ctl->buttons(),      
      ctl->axisX(),        
      ctl->axisY(),        
      ctl->axisRX(),       
      ctl->axisRY(),   // (-511 - 512) right Y axis     
      ctl->brake(),        
      ctl->throttle(),      // (0 - 1023): throttle (AKA gas) button
      ctl->miscButtons(),          
      ctl->a(),   
      ctl->x(),
      ctl->y(),
      ctl->b()       // Aggiungi l'argomento ctl->a()
    );
}

void processGamepad(ControllerPtr ctl) {
    // There are different ways to query whether a button is pressed.
    // By query each button individually:
    //  a(), b(), x(), y(), l1(), etc...
    datiTrasmessi.angoloServo = map(ctl->axisRX(),-511,512,70,100);
    datiTrasmessi.potenzaMotore = map(ctl->throttle(),0,1023,0,60);
}

void processControllers() {
    for (auto myController : myControllers) {
        if (myController && myController->isConnected() && myController->hasData()) {
            if (myController->isGamepad()) {
                processGamepad(myController);
            }
        }
    }
}

// Arduino setup function. Runs in CPU 1
void setup() {
    Serial.begin(115200);
    Serial.printf("Firmware: %s\n", BP32.firmwareVersion());
    const uint8_t* addr = BP32.localBdAddress();
    Serial.printf("BD Addr: %2X:%2X:%2X:%2X:%2X:%2X\n", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
    
    xTaskCreatePinnedToCore (
      loop2,     // Function to implement the task
      "loop2",   // Name of the task
      2000,      // Stack size in bytes
      NULL,      // Task input parameter
      0,         // Priority of the task
      NULL,      // Task handle.
      0          // Core where the task should run
    );

    // Setup radio 
    WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    // Setup Bluepad32 
    BP32.setup(&onConnectedController, &onDisconnectedController);
    BP32.forgetBluetoothKeys();
    BP32.enableVirtualDevice(false);

    // Setup radio 
    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.enableAckPayload();
    radio.setRetries(5, 5);
    radio.openWritingPipe(Address);

    WiFi.begin(ssid, password);
    Serial.println();
    Serial.println();
    Serial.println("Wait for WiFi... ");
    delay(1000);
}

//Runs in CPU2
void loop2 (void* pvParameters) {
  while (1) {
    bool dataUpdated = BP32.update();
    if (dataUpdated) {
      processControllers(); 
    }

    send();
  }

}

// Arduino loop function. Runs in CPU 1.
void loop() {
    WiFiClient client;

    if(newData) {
      if(!client.connect(serverIP, 3030)){
        Serial.println("Connection to host failed, data lost");
        newData = false;
        return;
      } else {
        client.print(String(ID ) +"/" + String(ackData[0]) + "/" + String(ackData[1]) + "/" + String(ackData[2]));
        newData = false;
      }
    }
    client.stop();
}

void send() {
    bool rslt;
    rslt = radio.write( &datiTrasmessi, sizeof(datiTrasmessi) );

    if (radio.available()) {
      radio.read(&ackData, sizeof(ackData));
      newData = true;
    }
 }
