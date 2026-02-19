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
    <title>Esp32 control</title>
    <style>
* {
    background-color: rgb(29, 29, 29);
}

.control-container {
    background-color: cadetblue;
    padding: 40px 40px;
    border-radius: 25px;
    width: 500px;
    height: max-content;
    text-align: center;
    margin-left: auto;
    margin-right: auto;
}
.control-container h1{
    font-size: 60px;
    color: aliceblue;
    font-weight: bold;
    font-family: monospace;
}

button {
    display: block;
    padding: 5px 50px;
    margin: 10px auto;
    font-size: 60px;
    font-weight: bold;
    color: aliceblue;
    background-color: black;
    border: none;
    border-radius: 15px;
    cursor: pointer;

}
button:hover{
    background-color: #2B3032;
}
    </style>
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
const char* ssid = "Your_SSID";
const char* password = "Your_Password";

void handleRoot() {
  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

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
