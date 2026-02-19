#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>


#define LED_PIN 2

WebServer server(80);

void setup() {
  pinMode(LED_PIN, OUTPUT);
  
  Serial.begin(115200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP("ESP32-Blink", "12345678");
  Serial.printf("IP: %s\n", WiFi.softAPIP().toString().c_str());

  server.on("/", []() {
    server.send(200, "text/html", "<html><body><h1>ESP32 LED</h1></body><html>");
  });

    server.on("/on", []() {
    digitalWrite(LED_PIN, HIGH);
    server.send(200, "text/html", "LED is ON");
  });

  server.on("/off", []() {
    digitalWrite(LED_PIN, LOW);
    server.send(200, "text/html", "LED if OFF");
  });

  server.begin();
}

void loop() {
  server.handleClient();

  digitalWrite(LED_PIN, HIGH);
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  delay(1000);
}
