## Qader A1 - Autonomous Robotic System

**Project Overview:**

Qader A1 is an advanced autonomous robotic system that we developed during our time at university. The project aimed to create a unique and innovative solution that combined various hardware components and software technologies. The goal was to differentiate our work from our peers by integrating modern IoT concepts and applying what we learned in programming with C++.

## Versions

### Version 1:
- **Technology Used:** Arduino Uno, IR Receiver, Servo Motor, Ultrasonic Sensor, Recycled Components.
- **Features:** Basic control using IR signals, distance measurement, servo motor movement.
- **Description:** The first version of Qader A1 was developed using Arduino Uno, where the robot could receive signals via an IR receiver and control various components. The robot could measure distances using an ultrasonic sensor and execute specific tasks based on the distance data using simple `if` statements.

### Version 2:
- **Technology Used:** ESP32, WiFi, Web Server, Servo Motor, Ultrasonic Sensor, Laser Module.
- **Features:** Web-based control via Access Point, real-time distance measurement, enhanced motor control, return-to-start position functionality.
- **Description:** The second version was a significant upgrade, integrating ESP32 for enhanced functionality. We learned and applied HTML, CSS, and JavaScript to create a web interface that allowed control over a WiFi network. The robot could now interact with a web server to perform tasks like targeting enemies with a laser and returning to its start position using stored distance and speed data.

## Software Development

- **Role:** I was responsible for the entire software development, where I combined my knowledge of C++, HTML, CSS, and JavaScript. The software controls the robot's movements, distance measurement, and communication with the web server.  
- **Challenges:** Integrating different technologies and ensuring smooth communication between hardware and software components.

## How to Load the Project

- **Recommended Tools:**  
  - Arduino IDE (version less than 2.7 is recommended)  
  - Libraries: IRremote, ESPAsyncWebServer, SPIFFS

### Steps:

1. **Clone the repository from GitHub:**
   ```bash
   git clone https://github.com/0xmaro/Qader-A1.git
