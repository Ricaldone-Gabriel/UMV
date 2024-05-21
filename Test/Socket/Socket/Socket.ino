#include <WiFi.h>

const char* ssid = "testnetwork";
const char* password = "testPassword";

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.println("...");
  }

  Serial.print("WiFi connected with IP:");
  Serial.println(WiFi.localIP());
}

void loop() {
  WiFiClient client;

  if(!client.connect(IPAddress(151,60,143,184), 3030)){
    Serial.println("Connection to host failed");

    delay(1000);
    return;
  }

  client.print("Hello from ESP32!");

  client.stop();

}
