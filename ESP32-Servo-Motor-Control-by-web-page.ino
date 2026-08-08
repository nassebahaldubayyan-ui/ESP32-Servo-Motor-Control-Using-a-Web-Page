#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Servo.h>

// =========================
// Pin Definitions
// =========================
const int redLED = 18;
const int greenLED = 19;
const int servoPin = 5;

// =========================
// Wi-Fi Access Point
// =========================
const char* ssid = "ESP32-Servo";
const char* password = "12345678";

// Create web server on port 80
WebServer server(80);

// Servo
Servo myServo;

// =========================
// HTML Webpage
// =========================
const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">

  <title>ESP32 Servo Control</title>

  <style>
    body {
      font-family: Arial, sans-serif;
      text-align: center;
      margin-top: 80px;
      background-color: #f2f2f2;
    }

    h1 {
      color: #333;
    }

    button {
      width: 200px;
      padding: 18px;
      margin: 15px;
      font-size: 22px;
      border: none;
      border-radius: 10px;
      cursor: pointer;
    }

    .open {
      background-color: #4CAF50;
      color: white;
    }

    .close {
      background-color: #f44336;
      color: white;
    }

    button:active {
      transform: scale(0.95);
    }
  </style>
</head>

<body>

  <h1>ESP32 Servo Control</h1>

  <button class="open" onclick="location.href='/open'">
    OPEN
  </button>

  <br>

  <button class="close" onclick="location.href='/close'">
    CLOSE
  </button>

</body>
</html>
)rawliteral";

// =========================
// OPEN Function
// =========================
void handleOpen() {

  myServo.write(180);

  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW);

  server.send(200, "text/html", webpage);
}

// =========================
// CLOSE Function
// =========================
void handleClose() {

  myServo.write(90);

  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);

  server.send(200, "text/html", webpage);
}

// =========================
// Home Page
// =========================
void handleRoot() {
  server.send(200, "text/html", webpage);
}

// =========================
// Setup
// =========================
void setup() {

  Serial.begin(115200);

  // LEDs
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Servo
  myServo.attach(servoPin);

  // Start in CLOSED position
  myServo.write(90);

  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);

  // =========================
  // Start Wi-Fi Access Point
  // =========================

  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("Wi-Fi Access Point Started!");
  Serial.print("Network Name: ");
  Serial.println(ssid);

  Serial.print("IP Address: ");
  Serial.println(WiFi.softAPIP());

  // =========================
  // Web Server Routes
  // =========================

  server.on("/", handleRoot);
  server.on("/open", handleOpen);
  server.on("/close", handleClose);

  server.begin();

  Serial.println("Web server started!");
}

// =========================
// Loop
// =========================
void loop() {
  server.handleClient();
}