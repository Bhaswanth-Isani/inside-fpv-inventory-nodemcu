#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

const char* ssid = "Venky";  //Your network SSID
const char* password = "987654321";

String serverName = "https://inside-fpv-server-production.up.railway.app";


void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to Wi-Fi...");
  }
  Serial.println("Connected to Wi-Fi");
}

void loop() {
  char buffer[60] = "";
  // read incoming data from the NodeMCU
  while (Serial.available() > 0) {
    // read the incoming data and split it into separate values
    Serial.readBytesUntil('\n', buffer, 60);
    String stock = String(buffer);
    Serial.println(stock);


    WiFiClientSecure client;
    HTTPClient http;
    client.setFingerprint("7A 19 06 B0 69 72 A3 77 A4 92 FE A2 29 59 D2 87 76 F7 77 FE");
    client.setTimeout(15000);

    client.connect(serverName, 443);
      Serial.println("Connected to server");
      String postJson = "{\"stock\":\"" + String(*(buffer))+String(*(buffer +1))+String(*(buffer +2))+String(*(buffer +3))+String(*(buffer +4)) + "\"}";
      http.begin(client, serverName + "/stock");
      http.addHeader("Content-Type", "application/json");
      int httpResponseCode = http.POST(postJson);
      Serial.println("HTTP response code: " + String(httpResponseCode));
      if (httpResponseCode > 0) {
        Serial.println("HTTP response code: " + String(httpResponseCode));
        String response = http.getString();
        Serial.println("Response: " + response);
      } else {
        Serial.println("Error on HTTP request");
      }
      http.end();
    // } else {
    //   Serial.println("Connection failed");
    // }

    delay(10000);
  }
}