#include <Bluepad32.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   12
#define CSN_PIN 13

ControllerPtr myControllers[BP32_MAX_GAMEPADS];
RF24 radio(CE_PIN,CSN_PIN); // CE, CSN
const byte Address[5] = {'2','a','p','l','e'};

struct dati{
  int angoloServo = 90;
  int potenzaMotore = 0;
};
float ackData[3] = {-100,-100,-100};

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 25; // send once per 50 millis

bool newData = false;
dati datiTrasmessi;

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
    datiTrasmessi.potenzaMotore = map(ctl->throttle(),0,1023,0,180);

    if (ctl->y()) {

    }

    
    //if (ctl->x()) {
        // Some gamepads like DS3, DS4, DualSense, Switch, Xbox One S, Stadia support rumble.
        // It is possible to set it by calling:
        // Some controllers have two motors: "strong motor", "weak motor".
        // It is possible to control them independently.
    //    ctl->playDualRumble(0 /* delayedStartMs */, 250 /* durationMs */, 0x80 /* weakMagnitude */,
    //                        0x40 /* strongMagnitude */);
    //}

    // Another way to query controller data is by getting the buttons() function.
    // See how the different "dump*" functions dump the Controller info.
    //dumpGamepad(ctl);
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

    // Setup the Bluepad32 callbacks
    BP32.setup(&onConnectedController, &onDisconnectedController);

    // "forgetBluetoothKeys()" should be called when the user performs
    // a "device factory reset", or similar.
    // Calling "forgetBluetoothKeys" in setup() just as an example.
    // Forgetting Bluetooth keys prevents "paired" gamepads to reconnect.
    // But it might also fix some connection / re-connection issues.
    BP32.forgetBluetoothKeys();

    // Enables mouse / touchpad support for gamepads that support them.
    // When enabled, controllers like DualSense and DualShock4 generate two connected devices:
    // - First one: the gamepad
    // - Second one, which is a "virtual device", is a mouse.
    // By default, it is disabled.
    BP32.enableVirtualDevice(false);

    radio.begin();
    radio.setDataRate(RF24_250KBPS);
    radio.enableAckPayload();
    radio.setRetries(5, 5);
    radio.openWritingPipe(Address);
}

// Arduino loop function. Runs in CPU 1.
void loop() {
    // This call fetches all the controllers' data.
    // Call this function in your main loop.
    bool dataUpdated = BP32.update();
    if (dataUpdated) {
        processControllers();
    }
    currentMillis = millis();
    if (currentMillis - prevMillis >= txIntervalMillis) {
        send();
    }

}

void send() {
    bool rslt;
    rslt = radio.write( &datiTrasmessi, sizeof(datiTrasmessi) );
        // Always use sizeof() as it gives the size as the number of bytes.
        // For example if dataToSend was an int sizeof() would correctly return 2
    
    Serial.println("Data Sent");
    Serial.println(String(datiTrasmessi.angoloServo) + " " + String(datiTrasmessi.potenzaMotore));
    Serial.println(String(ackData[0])+ " " + String(ackData[1]) + " " +  String(ackData[2]));
   
    if (radio.available()) {
      radio.read(&ackData, sizeof(ackData));
      newData = true;
    } else {
      Serial.println("Acknowledge but no data");
    }
    prevMillis = millis();
 }
