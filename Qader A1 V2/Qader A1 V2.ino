#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ESP32Servo.h>

// إعدادات الشبكة
const char* ssid = "Ahmed";
const char* password = "12345678";

AsyncWebServer server(80);
Servo myservo;

volatile int currentSpeed = 100; // سرعة البداية الافتراضية (100 هي السرعة المتوسطة)
const int trigPin = 12;
const int echoPin = 13;
const int redLedPin = 26;
const int greenLedPin = 25;
const int ledPin = 27; // تعريف البن للـ LED الجديد

long duration;
int distance;
int pos = 0;
bool increasing = true;
bool stopServo = false;

bool ledState = false; // تتبع حالة LED

String command; // String to store app command state.
int speedCar = 800; // 400 - 1023.
int speed_Coeff = 3;

// Motor pins
const int ENA = 5;
const int IN_1 = 18;
const int IN_2 = 19;
const int ENB = 21;
const int IN_3 = 22;
const int IN_4 = 23;

struct MovementState {
  int speed;
  unsigned long timestamp;
  String direction;
};

MovementState startState;
unsigned long startTime;

void setup() {
  Serial.begin(115200);
  myservo.attach(14);  // توصيل السيرفو
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(ledPin, OUTPUT); // تهيئة البن للـ LED الجديد

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);

  // تهيئة SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // إنشاء نقطة وصول (Access Point)
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // توجيه الويب
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/index.html", "text/html", false, nullptr);
  });

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/style.css", "text/css", false, nullptr);
  });

  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/script.js", "text/javascript", false, nullptr);
  });

  server.on("/p5.min.js", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/p5.min.js", "text/javascript", false, nullptr);
  });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(SPIFFS, "/favicon.ico", "image/x-icon");
  });

  // إعداد الويب للتفاعل مع الحساس
  server.on("/distance", HTTP_GET, [](AsyncWebServerRequest *request) {
    int measuredDistance = measureDistance();
    request->send_P(200, "text/plain", String(measuredDistance).c_str());
  });

  server.on("/servo", HTTP_POST, [](AsyncWebServerRequest *request) {
    if (request->hasParam("angle", true)) {
      String angle = request->getParam("angle", true)->value();
      pos = angle.toInt();
      myservo.write(pos);
      request->send(200, "text/plain", "OK");
    } else {
      request->send(400, "text/plain", "Bad Request");
    }
  });

  server.on("/data", HTTP_GET, [](AsyncWebServerRequest *request) {
    String json = "{";
    json += "\"angle\":" + String(pos) + ",";
    json += "\"distance\":" + String(distance);
    json += "}";
    request->send(200, "application/json", json);
  });

  server.on("/resume", HTTP_GET, [](AsyncWebServerRequest *request) {
    stopServo = false;
    request->send(200, "text/plain", "Resumed scanning");
  });

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
      request->send(400, "text/plain", "Bad Request");
    }
  });

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
      request->send(400, "text/plain", "Bad Request");
    }
  });

  server.on("/clearState", HTTP_POST, [](AsyncWebServerRequest *request) {
    startState.speed = 0;
    startState.timestamp = 0;
    startState.direction = "";
    request->send(200, "text/plain", "Start state cleared");
  });

  server.begin();
}

void loop() {
  distance = measureDistance();
 

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

  // التحكم في LED
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

void storeStartState(String direction) {
  startState.speed = speedCar;
  startState.timestamp = millis();
  startState.direction = direction;
}

void goAhead() {
  storeStartState("goAhead");
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goBack() {
  storeStartState("goBack");
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void goRight() {
  storeStartState("goRight");
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, HIGH);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, HIGH);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, speedCar);
}

void goLeft() {
  storeStartState("goLeft");
  digitalWrite(IN_1, HIGH);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, speedCar);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, HIGH);
  analogWrite(ENB, speedCar);
}

void stopRobot() {
  digitalWrite(IN_1, LOW);
  digitalWrite(IN_2, LOW);
  analogWrite(ENA, 0);

  digitalWrite(IN_3, LOW);
  digitalWrite(IN_4, LOW);
  analogWrite(ENB, 0);
}

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
