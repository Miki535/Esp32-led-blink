#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#define LED_PIN 2

WebServer server(80);

const char* html = R"""(
  <!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="style.css">
    <title>Esp32 control</title>
</head>
<body>
    <div class="control-container">
    <h1>Esp32 Led Blink</h1>
    <button onclick="toggleLED('on')">Turn ON</button>
    <button onclick="toggleLED('off')">Turn OFF</button>
    </div>

    <script>
        const toggleLED = (state) => {
            fetch('/' + state)
                .then(response => response.text())
                .then(data => console.log(data))
                .catch(err => console.error(err));
        }
    </script>
</body>
</html>
)"""; 

const char* ap_ssid = "ESP32-AP";
const char* ap_password = "12345678";

void handleRoot() {
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ap_ssid, ap_password);      
  Serial.print("AP IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);

  server.on("/on", []() {
    digitalWrite(LED_PIN, HIGH);
    server.send(200, "text/html", "LED is ON");
  });

  server.on("/off", []() {
    digitalWrite(LED_PIN, LOW);
    server.send(200, "text/html", "LED is OFF");
  });

  server.begin();
}

void loop() {
  server.handleClient();
}
