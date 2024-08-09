# Qader A1 - ESP32 Robot Control with Radar Interface

## Project Overview

The Qader A1 project involves an ESP32-controlled robot that can be navigated using a web interface. The robot features motor control, distance measurement with an ultrasonic sensor, and an LED that can be fired like a laser. The radar interface allows for real-time distance visualization, while the motor control section enables directional movement, speed control, and automatic return to the starting position.

## Features

- **Motor Control:** Move the robot forward, backward, left, and right. Control the speed with three settings (slow, medium, fast) and stop the robot at any time.
- **Radar Interface:** Real-time radar display with angle and distance measurements, highlighting objects within the robot's range.
- **LED Control:** Fire the LED as a laser beam with a visual indication on the web interface.
- **Return to Start:** Automatically return the robot to its starting point, tracking the distance and speed.

## File Structure

The project is organized as follows:

- **`Qader A1 V2.ino`**: The core C++ code for controlling the robot, handling Wi-Fi, and serving the web interface.
- **`index.html`**: The HTML file for the web interface, including the radar and motor control sections.
- **`style.css`**: The CSS file for styling the web interface, ensuring a polished and consistent look.
- **`script.js`**: The JavaScript file for handling client-side interactions, including button controls and radar visualization.
- **`p5.min.js`**: A library for creating graphical elements in the radar interface.

## JavaScript Summary

The `script.js` file handles various functionalities of the web interface:

- **Radar Control:**
  - Real-time data collection from the ultrasonic sensor.
  - Display of distance and angle on a radar-like interface.
  - Highlighting of objects detected within and beyond a certain range.
  - Controls for resetting the servo and resuming scanning.
  - LED toggle functionality, visualized as a "Fire" button on the interface.

- **Motor Control:**
  - Buttons for controlling the robot's movement in all directions.
  - Speed control with three levels: slow, medium, and fast.
  - Functionality to stop the robot immediately.
  - Return to Start button, which uses the robot's tracking data to navigate back to the starting point.

## How to Upload Files to ESP32 Using SPIFFS

To upload the web interface files (`index.html`, `style.css`, `script.js`, `p5.min.js`) to the ESP32:

### 1. Prepare the Files

Ensure the following files are in the `data` directory of your project:

- `index.html`
- `style.css`
- `script.js`
- `p5.min.js`

### 2. Uploading Files Using Arduino IDE

1. **Install the ESP32 SPIFFS Plugin:**
   - Download and install the [ESP32 SPIFFS Plugin](https://github.com/me-no-dev/arduino-esp32fs-plugin).
   
2. **Upload Files:**
   - Open the Arduino IDE.
   - Go to **Tools** > **ESP32 Sketch Data Upload** to upload the files.

### 3. Uploading Files Using PlatformIO

1. **Prepare the Environment:**
   - Ensure you have PlatformIO installed and set up.
   
2. **Upload Files:**
   - Run the following command in your terminal:
     ```bash
     platformio run --target uploadfs
     ```

### 4. Verify the Upload

1. **Connect to the ESP32's Wi-Fi:**
   - Use the IP address assigned to your ESP32.
   
2. **Access the Web Interface:**
   - Open a browser and navigate to the ESP32's IP address to see the radar and motor control interface.

## Conclusion

The Qader A1 project showcases the versatility of the ESP32 in robotics, combining motor control with real-time sensor data visualization. The intuitive web interface allows for easy interaction with the robot, whether for simple navigation or more complex tasks like returning to a starting position.
