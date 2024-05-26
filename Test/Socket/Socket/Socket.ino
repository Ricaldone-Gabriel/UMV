#include <WiFi.h>

const char* ssid = "testnetwork";
const char* password = "testPassword";

const IPAddress serverIP = IPAddress(151,60,143,184);

double arrayTest[] = {1,2,3};
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

void setup() {
  Serial.begin(115200);
  WiFi.disconnect(true);
  delay(1000);

  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  WiFi.begin(ssid, password);
  Serial.println();
  Serial.println();
  Serial.println("Wait for WiFi... ");
  delay(1000);
}

void loop() {
  WiFiClient client;

  if(!client.connect(serverIP, 3030)){
    Serial.println("Connection to host failed");
    return;
  } else {
    client.print(String(arrayTest[0]) + "/" + String(arrayTest[1]) + "/" + String(arrayTest[2]));
  }
  
  client.stop();

}