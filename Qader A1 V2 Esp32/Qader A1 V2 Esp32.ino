//  This video to explain look and what's happening -> "  https://youtu.be/1CY9IlN5VvA   "
// In order to avoid any problems when uploading the code,
// use an old version of Arduino IDE and I recommend a version less than 2.7 because the library IRremote , ESPAsyncWebServer and SPIFFS 
// does not support modern versions.
// < I hope it helps you  >

#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ESP32Servo.h>

// Network settings
const char* ssid = "Qader A1"; // Access Point name
const char* password = "Qader-A1.V1"; // Access Point password

AsyncWebServer server(80); // Create an asynchronous web server on port 80
Servo myservo; // Servo object

volatile int currentSpeed = 100; // Default starting speed (100 is medium speed)
const int trigPin = 12; // Sensor pin for sending pulses
const int echoPin = 13; // Sensor pin for receiving pulses
const int redLedPin = 26; // Red LED pin
const int greenLedPin = 25; // Green LED pin
const int ledPin = 27; // New LED pin

long duration; // Duration of the ultrasonic pulse
int distance; // Measured distance
int pos = 0; // Servo position
bool increasing = true; // Control increasing or decreasing the servo angle
bool stopServo = false; // Control stopping the servo movement

bool ledState = false; // Track the state of the LED

String command; // Store control commands from the app
int speedCar = 800; // Car speed (400 - 1023)
int speed_Coeff = 3; // Speed coefficient

// Motor driver pins
const int ENA = 5;
const int IN_1 = 18;
const int IN_2 = 19;
const int ENB = 21;
const int IN_3 = 22;
const int IN_4 = 23;

// Struct to store movement state
struct MovementState {
  int speed;
  unsigned long timestamp;
  String direction;
};

MovementState startState; // Start state
unsigned long startTime; // Start time

void setup() {
  Serial.begin(115200); // Start serial communication
  myservo.attach(14);  // Attach the servo
  pinMode(trigPin, OUTPUT); // Set sensor pin as output
  pinMode(echoPin, INPUT); // Set sensor pin as input
  pinMode(redLedPin, OUTPUT); // Set red LED pin as output
  pinMode(greenLedPin, OUTPUT); // Set green LED pin as output
  pinMode(ledPin, OUTPUT); // Set new LED pin as output

  pinMode(ENA, OUTPUT); // Set motor A control pin as output
  pinMode(ENB, OUTPUT); // Set motor B control pin as output
  pinMode(IN_1, OUTPUT); // Set motor A pin as output
  pinMode(IN_2, OUTPUT); // Set motor A pin as output
  pinMode(IN_3, OUTPUT); // Set motor B pin as output
  pinMode(IN_4, OUTPUT); // Set motor B pin as output

  // Initialize SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An error occurred while initializing SPIFFS");
    return;
  }

  // Create Access Point
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println(IP);

  // Set up web routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html");
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css");
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/script.js", "text/javascript");
  });

  server.on("/p5.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/p5.min.js", "text/javascript");
  });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/favicon.ico", "image/x-icon");
  });

  // Route to get measured distance
  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request) {
    int measuredDistance = measureDistance();
    request->send_P(200, "text/plain", String(measuredDistance).c_str());
  });

  // Route to control servo angle
  server.on("/servo", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("angle", true)) {
      String angle = request->getParam("angle", true)->value();
      pos = angle.toInt();
      myservo.write(pos);
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Invalid request");
    }
  });

  // Route to get sensor data
  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"angle\":" + String(pos) + ",";
    json += "\"distance\":" + String(distance);
    json += "}";
    request->send(200, "application/json", json);
  });

  // Route to resume servo scanning
  server.on("/resume", HTTP_GET, [](AsyncWebServerRequest *request) {
    stopServo = false;
    request->send(200, "text/plain", "Resumed scanning");
  });

  // Route to control LED state
  server.on("/led", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("state", true)) {
      String state = request->getParam("state", true)->value();
      if (state == "on") {
        for (int i = 0; i <= 15; i++) {
          digitalWrite(ledPin, HIGH);
          delay(40);
          digitalWrite(ledPin, LOW);
          delay(40);
        }
        ledState = true;
      } else if (state == "off") {
        for (int i = 0; i <= 20; i++) {
          digitalWrite(ledPin, HIGH);
          delay(40);
          digitalWrite(ledPin, LOW);
          delay(40);
        }
        ledState = false;
      }
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Invalid request");
    }
  });

  // Route to control driving commands
  server.on("/command", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("command", true)) {
      command = request->getParam("command", true)->value();
      if (command == "goAhead") {
        goAhead();
      } else if (command == "goBack") {
        goBack();
      } else if (command == "goRight") {
        goRight();
      } else if (command == "goLeft") {
        goLeft();
      } else if (command == "stop") {
        stopRobot();
      } else if (command == "slow") {
        speedCar = 100;
      } else if (command == "medium") {
        speedCar = 323;
      } else if (command == "fast") {
        speedCar = 1023;
      } else if (command == "returnToStart") {
        returnToStart();
      }
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Invalid request");
    }
  });

  // Route to clear start state
  server.on("/clearState", HTTP_POST, [](AsyncWebServerRequest *request) {
    startState.speed = 0;
    startState.timestamp = 0;
    startState.direction = "";
    request->send(200, "text/plain", "Start state cleared");
  });

  server.begin(); // Start the web server
}

void loop() {
  distance = measureDistance(); // Measure the distance

  // Control servo movement
  if (!stopServo) {
    if (increasing) {
      pos += 1;
      if (pos >= 180) {
        increasing = false;
      }
    } else {
      pos -= 1;
      if (pos <= 0) {
        increasing = true;
      }
    }
    myservo.write(pos);
    delay(30);
  }

  // Control LED state based on distance
  if (distance >= 100) {
    digitalWrite(redLedPin, LOW);
    digitalWrite(greenLedPin, HIGH);
  } else if (distance > 30 && distance < 50) {
    digitalWrite(redLedPin, HIGH);
    digitalWrite(greenLedPin, HIGH);
  } else if (distance <= 30) {
    digitalWrite(greenLedPin, LOW);
    stopServo = true;
    digitalWrite(redLedPin, HIGH);
    delay(30);
    digitalWrite(redLedPin, LOW);
    delay(30);
  }
}

// Measure the distance using the ultrasonic sensor
int measureDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  return distance;
}

// Store the start state
void storeStartState(String direction) {
  startState.speed = speedCar;
  startState.timestamp = millis();
  startState.direction = direction;
}

// Move forward
void goAhead() {
  storeStartState("goAhead");
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

// Move backward
void goBack() {
  storeStartState("goBack");
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

// Turn right
void goRight() {
  storeStartState("goRight");
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

// Turn left
void goLeft() {
  storeStartState("goLeft");
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

// Stop the robot
void stopRobot() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, 0);
}

// Return to the start position
void returnToStart() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startState.timestamp;
  int returnSpeed = startState.speed;

  if (startState.direction == "goAhead") {
    goBack();
  } else if (startState.direction == "goBack") {
    goAhead();
  } else if (startState.direction == "goRight") {
    goLeft();
  } else if (startState.direction == "goLeft") {
    goRight();
  }

  delay(elapsedTime);
  stopRobot();
}
