# ESP32 Tank Controlled by Infrared Signals

## Project Overview

This project uses an ESP32 board to control a tank using infrared (IR) signals. The tank can be moved in various directions, and the system also incorporates distance measurement to adjust the servo motor's position based on proximity to an object. Additionally, LED lights and other signals can be controlled using an IR remote.

## Features

- **IR Signal Reception**: The IR receiver is connected to pin 13 of the ESP32. It captures and decodes signals from an IR remote control.
- **Distance Measurement**: The ESP32 calculates the distance using an ultrasonic sensor, and the system reacts based on the measured distance.
- **Servo Motor Control**: The servo motor is adjusted based on the distance and the received IR signals.
- **LED Control**: Multiple LEDs (red, green, blue) are used to indicate different states, and they can be controlled via IR signals.
- **Motor Control**: The motors controlling the tank's movement respond to specific IR commands.
- **Serial Communication**: The serial communication is set to a baud rate of 115200 for monitoring and debugging.

## Video Demonstration

A video explaining the project and showing it in action is available [here](https://youtu.be/1CY9IlN5VvA).

## Getting Started

### Hardware Requirements

- ESP32 board
- IR receiver (connected to pin 13)
- Servo motor
- Ultrasonic distance sensor (connected to pins 26 and 25)
- LEDs (connected to pins 32, 33, 34, 35)
- Motor driver and motors (connected to pins 16, 17, 18, 19)
- IR remote control

### Software Requirements

- Arduino IDE (recommended version below 2.7 for compatibility)
- IRremote library
- ESP32Servo library
- ESPAsyncWebServer and SPIFFS libraries (if additional features are required)

### Installation

1. **Clone the repository**:
    ```bash
    git clone https://github.com/yourusername/esp32-tank-ir-control.git
    cd esp32-tank-ir-control
    ```

2. **Open the project in your IDE**.

3. **Upload the code to the ESP32**:
    - Ensure you are using an Arduino IDE version below 2.7 to avoid compatibility issues with the libraries.
    - Set the baud rate to 115200 in the `setup()` function:
      ```cpp
      Serial.begin(115200);
      ```
    - Connect your ESP32 and upload the code.

4. **Test and Calibrate**:
    - Use the serial monitor to view the received IR signals and adjust the code if necessary.
    - Ensure that the distance sensor, LEDs, and servo motor respond correctly to the received signals.

## How It Works

- **IR Signal Processing**: The IR receiver connected to pin 13 captures the signals from the remote. The ESP32 decodes these signals to control the tank's movement, LEDs, and servo motor.
- **Distance Calculation**: The ultrasonic sensor measures the proximity to an object, and based on this distance, the ESP32 adjusts the servo motor's position.
- **Movement and LED Control**: Depending on the received IR commands, the tank can move in different directions, and the LEDs can be turned on or off.

## Troubleshooting

- **IDE Version**: Use an Arduino IDE version below 2.7 to avoid issues with the IRremote, ESPAsyncWebServer, and SPIFFS libraries.
- **Signal Issues**: Ensure that the IR receiver is properly connected to pin 13 and that the remote control is functioning correctly.

## Future Enhancements

- Implement additional controls using Bluetooth or Wi-Fi.
- Add a camera for live video feed and control.
- Integrate advanced obstacle avoidance algorithms.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for more details.
