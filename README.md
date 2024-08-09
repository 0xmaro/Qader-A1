## Qader A1 - Autonomous Robotic System

**Project Overview:**

Qader A1 is an advanced autonomous robotic system that I developed during my time at university. The project aimed to create a unique and innovative solution that combined various hardware components and software technologies. The goal was to differentiate my work from my peers by integrating modern IoT concepts and applying what I learned in programming with C++.

**Versions:**

### Version 1:
- **Technology Used:** Arduino Uno, IR Receiver, Servo Motor, Ultrasonic Sensor, Recycled Components.
- **Features:** Basic control using IR signals, distance measurement, servo motor movement.
- **Description:** The first version of Qader A1 was developed using Arduino Uno, where the robot could receive signals via an IR receiver and control various components. The robot could measure distances using an ultrasonic sensor and execute specific tasks based on the distance data using simple `if` statements.

### Version 2:
- **Technology Used:** ESP32, WiFi, Web Server, Servo Motor, Ultrasonic Sensor, Laser Module.
- **Features:** Web-based control via Access Point, real-time distance measurement, enhanced motor control, return-to-start position functionality.
- **Description:** The second version was a significant upgrade, integrating ESP32 for enhanced functionality. I learned and applied HTML, CSS, and JavaScript to create a web interface that allowed control over a WiFi network. The robot could now interact with a web server to perform tasks like targeting enemies with a laser and returning to its start position using stored distance and speed data.

**Software Development:**
- **Role:** I was responsible for the entire software development, where I combined my knowledge of C++, HTML, CSS, and JavaScript. The software controls the robot's movements, distance measurement, and communication with the web server. 
- **Challenges:** Integrating different technologies and ensuring smooth communication between hardware and software components.

**How to Load the Project:**
- **Recommended Tools:** 
  - Arduino IDE (version less than 2.7 is recommended)
  - Libraries: IRremote, ESPAsyncWebServer, SPIFFS

- **Steps:**
  1. **Clone the repository from GitHub:**
     - Open Command Prompt or Terminal on your computer.
     - Use the following command to clone the repository to your local machine:
       ```bash
       git clone https://github.com/0xmaro/Qader-A1.git
       ```
     - This will download all project files into a new folder named `Qader-A1` on your machine.

  2. **Navigate to the project directory:**
     - After cloning, navigate to the project folder using the following command:
       ```bash
       cd Qader-A1
       ```

  3. **Load the provided code into the Arduino IDE:**
     - Open Arduino IDE on your computer.
     - In Arduino IDE, go to `File` > `Open`, then select the project file (usually with a `.ino` extension) inside the `Qader-A1` folder.

  4. **Install the required libraries:**
     - In Arduino IDE, make sure to install the required libraries like `IRremote`, `ESPAsyncWebServer`, and `SPIFFS`. You can install these libraries by going to `Sketch` > `Include Library` > `Manage Libraries`, then search for and install the necessary libraries.

  5. **Upload the code to the ESP32 board:**
     - Once the file is open in Arduino IDE and the required libraries are installed, upload the code to the ESP32 board by clicking the `Upload` button.

  6. **Connect to the "Qader A1" Access Point:**
     - After uploading the code, a WiFi network named "Qader A1" will appear. Connect to it using the password specified in the code.

  7. **Open the web interface to start controlling the robot:**
     - Once connected to the Access Point, open a web browser and enter the ESP32's IP address (typically 192.168.4.1) to access the control interface and start operating the robot.

**Additional Information:**
- Watch the project in action on [YouTube](https://youtu.be/1CY9IlN5VvA).
- The project is best experienced using an older version of the Arduino IDE to avoid compatibility issues with certain libraries.

**Acknowledgments:**
I would like to thank my university for providing me with the opportunity to work on this project. My role in this project grew as I was responsible for the software development. I also thank my friend Ahmed, who contributed to the hardware aspect of the project.
