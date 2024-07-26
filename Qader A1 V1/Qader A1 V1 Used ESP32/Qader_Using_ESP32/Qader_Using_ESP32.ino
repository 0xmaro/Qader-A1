//  This video to explain look and what's happening -> "  https://youtu.be/1CY9IlN5VvA   "
// In order to avoid any problems when uploading the code,
// use an old version of Arduino IDE and I recommend a version less than 2.7 because the library IRremote , ESPAsyncWebServer and SPIFFS 
// does not support modern versions.
// < I hope it helps you  >
#include <IRremote.h>
#include <ESP32Servo.h>

const unsigned long LED_ON_SIGNAL = 16597693; // Signal to turn the LED on
const unsigned long LED_OFF_SIGNAL = 16626253; // Signal to turn the LED off

const int IRpin = 13; // Pin for the IR receiver
IRrecv irrecv(IRpin); // Create IR receiver object
decode_results results; // Variable to store IR results

const int trigPin = 26; // Pin for ultrasonic sensor trigger
const int echoPin = 25; // Pin for ultrasonic sensor echo
const int redPin = 32; // Pin for red LED
const int greenPin = 33; // Pin for green LED
const int bluePin = 34; // Pin for blue LED
const int blueLED = 35; // Pin for an additional blue LED

Servo servoMotor; // Servo motor object
int servoAngle = 90; // Initial angle of the servo motor
long distance; // Variable to store distance

const int motor1Pin1 = 16; // Motor 1 pin 1
const int motor1Pin2 = 17; // Motor 1 pin 2
const int motor2Pin1 = 18; // Motor 2 pin 1
const int motor2Pin2 = 19; // Motor 2 pin 2

void setup() {
  Serial.begin(115200); // Initialize serial communication with a baud rate of 115200
  irrecv.enableIRIn(); // Start the IR receiver
  pinMode(trigPin, OUTPUT); // Set trigPin as output
  pinMode(echoPin, INPUT); // Set echoPin as input
  pinMode(redPin, OUTPUT); // Set redPin as output
  pinMode(greenPin, OUTPUT); // Set greenPin as output
  pinMode(bluePin, OUTPUT); // Set bluePin as output
  pinMode(blueLED, OUTPUT); // Set blueLED as output
  pinMode(motor1Pin1, OUTPUT); // Set motor1Pin1 as output
  pinMode(motor1Pin2, OUTPUT); // Set motor1Pin2 as output
  pinMode(motor2Pin1, OUTPUT); // Set motor2Pin1 as output
  pinMode(motor2Pin2, OUTPUT); // Set motor2Pin2 as output
  servoMotor.attach(14); // Attach the servo motor to pin 14
}

void loop() {
  if (irrecv.decode(&results)) { // If an IR signal is received
    Serial.print("Received IR signal: "); // Print "Received IR signal: "
    Serial.println(results.value); // Print the hex value of the received signal
    switch (results.value) {
      case LED_ON_SIGNAL:
        if (distance < 50) {
          for (int i = 0; i <= 15; i++) {
            digitalWrite(bluePin, HIGH);
            delay(40);
            digitalWrite(bluePin, LOW);
            delay(40);
            digitalWrite(blueLED, LOW);
          }
          for (int i = 0; i <= 25; i++) {
            digitalWrite(motor1Pin1, LOW);
            digitalWrite(motor1Pin2, HIGH);
            digitalWrite(motor2Pin1, HIGH);
            digitalWrite(motor2Pin2, LOW);
          }
        }
        break;
      case LED_OFF_SIGNAL:
        digitalWrite(bluePin, LOW);
        break;
      case 16609423: // Button 1
        if (distance < 50) {
          servoAngle += 10;
          servoMotor.write(servoAngle);
        }
        break;
      case 16599223: // Button 2
        if (distance < 50) {
          servoAngle -= 10;
          servoMotor.write(servoAngle);
        }
        break;
      case 16605343: // Forward signal
        digitalWrite(blueLED, HIGH);
        digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW);
        digitalWrite(motor2Pin1, HIGH);
        digitalWrite(motor2Pin2, LOW);
        break;
      case 16613503: // Turn off front light
        digitalWrite(blueLED, LOW);
        break;
      case 16640023: // Turn on front light
        digitalWrite(blueLED, HIGH);
        break;
      case 16607383: // Reverse signal
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH);
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH);
        break;
      case 16635943: // Turn right signal
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, HIGH);
        digitalWrite(motor2Pin1, HIGH);
        digitalWrite(motor2Pin2, LOW);
        break;
      case 16603813: // Turn left signal
        digitalWrite(motor1Pin1, HIGH);
        digitalWrite(motor1Pin2, LOW);
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, HIGH);
        break;
      case 16620133: // Stop movement signal
        digitalWrite(blueLED, HIGH);
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW);
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, LOW);
        break;
      case 16589533: // Pause all signal
        digitalWrite(trigPin, LOW); // Turn off ultrasonic trigger
        digitalWrite(echoPin, LOW);
        digitalWrite(redPin, LOW);
        digitalWrite(greenPin, LOW);
        digitalWrite(bluePin, LOW);
        digitalWrite(blueLED, LOW);
        servoMotor.detach();
        digitalWrite(motor1Pin1, LOW);
        digitalWrite(motor1Pin2, LOW);
        digitalWrite(motor2Pin1, LOW);
        digitalWrite(motor2Pin2, LOW);
        break;
      case 16605853: // Play all signal
        digitalWrite(trigPin, HIGH);
        digitalWrite(echoPin, HIGH);
        digitalWrite(redPin, HIGH);
        digitalWrite(greenPin, HIGH);
        digitalWrite(bluePin, LOW);
        digitalWrite(blueLED, HIGH);
        servoMotor.attach(14);
        break;  
    }
    irrecv.resume(); // Receive the next value
  }
  
  long duration; // Variable to store duration of the pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Read the pulse duration
  distance = (duration / 2) / 29.1; // Calculate distance

  if (distance < 50) {
    digitalWrite(redPin, HIGH);
    delay(45);
    digitalWrite(redPin, LOW);
    delay(45);
    digitalWrite(greenPin, LOW);
    servoMotor.write(servoAngle);
  }
  else if (distance < 20) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, LOW);
    digitalWrite(motor2Pin1, LOW);
    digitalWrite(motor2Pin2, LOW);
    servoMotor.write(servoAngle); 
    digitalWrite(redPin, HIGH);
    delay(45);
    digitalWrite(redPin, LOW);
    delay(45);
    digitalWrite(greenPin, LOW);
  } else {
    digitalWrite(redPin, LOW);
    digitalWrite(greenPin, HIGH);
    int angle = (millis() / 15) % 180;
    servoMotor.write(angle);
    delay(2);
  }
}
