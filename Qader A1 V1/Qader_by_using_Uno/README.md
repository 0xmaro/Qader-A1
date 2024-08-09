# Qader A1 V1 - Arduino Uno Controlled Tank via IR Remote

This project demonstrates how to use an Arduino Uno to create a tank that is controlled via infrared (IR) signals. The tank can move in different directions, control LEDs, and adjust a servo motor based on IR signals and ultrasonic sensor readings.

## Features

- **Infrared Remote Control**: Control the tank's movement and LEDs using an IR remote.
- **Ultrasonic Sensor**: Measures distance and adjusts the servo motor accordingly.
- **Servo Motor**: Rotates based on distance readings from the ultrasonic sensor.
- **Multiple LEDs**: Provides visual feedback for the tank's status.
- **Motors**: Controls the tank's movement in forward, reverse, left, and right directions.

## Components

- Arduino Uno
- IR receiver (connected to pin 7)
- Ultrasonic sensor (trigger pin: 12, echo pin: 11)
- Servo motor (connected to pin 10)
- Multiple LEDs (connected to pins A0, A1, A2, A3)
- Motor driver and motors (connected to pins 2, 3, 4, 5)

## Code Overview

The code handles receiving IR signals and controlling the tank based on those signals. Here’s a brief overview of the main parts:

- **IR Signals**: The IR receiver reads signals from a remote and triggers corresponding actions (e.g., turning LEDs on/off, moving the tank).
- **Ultrasonic Sensor**: Measures distance to obstacles and adjusts the servo motor's angle accordingly.
- **Motor Control**: The tank moves forward, backward, left, or right based on the IR signals.
- **LED Control**: LEDs light up to indicate the tank's status, such as movement or when an obstacle is detected.

## Getting Started

### Prerequisites

- **Arduino IDE**: Make sure to use an older version of the Arduino IDE (recommended version < 2.7) because the libraries used (IRremote, Servo) may not be fully compatible with newer versions.

### Installation

1.**Clone the repository**:

    ```
    git clone https://github.com/0xmaro/Qader-A1.git
    cd Qader-A1/Qader\ A1\ V1/Qader_by_using_Uno
    ```
    
2. **Install Libraries**: Ensure you have the following libraries installed:
   - `IRremote`
   - `Servo`
3. **Upload to Arduino**: Open the code in the Arduino IDE and upload it to your Arduino Uno.
4.     - Set the baud rate to 115200 in the `setup()` function:
      ```cpp
      Serial.begin(115200);

### Usage

- **Control the Tank**: Use the IR remote to control the tank's movement, LEDs, and servo motor. The code responds to various signals to move the tank in different directions, adjust the servo, and control the LEDs.
- **Watch the Video**: [This video](https://youtu.be/1CY9IlN5VvA) explains the project setup and demonstrates the functionality.

### Important Note

To avoid any issues when uploading the code, **use an old version of the Arduino IDE** (recommended version less than 2.7), as the libraries `IRremote`, `Servo`, and others may not be supported in modern versions.

## Conclusion

This project showcases how to build a simple yet functional remote-controlled tank using an Arduino Uno. The combination of IR signals, ultrasonic sensors, and servo motors makes it a versatile project for learning and experimenting with Arduino.
