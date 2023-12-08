[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-24ddc0f5d75046c5622901739e7c5dd533143b0c8e959d652212380cedb1ea36.svg)](https://classroom.github.com/a/NZx8S8dt)

# ESE5190 Final Project - Cyberpet

## Project Description

The concept of a Cyberpet revolves around the development of an interactive companion robot designed to provide users with companionship and infuse a fresh, vibrant energy into their daily lives. This innovative project aims to bridge the gap between technology and personal interaction, offering a unique and engaging experience for users seeking companionship through technological means.

## Key Feature of Cyberpet

The Cyberpet companion robot features several innovative modes designed to enhance user interaction and utility:

1. **Accompany Mode**:

   - Exhibits random movements and facial expressions for dynamic and lifelike interaction.
   - Adapts behavior based on user's mood and environment for a personalized experience.
2. **Temp-Humid-Time Mode**:

   - Acts as an environmental monitor, displaying real-time temperature, humidity, and time.
   - Integrates high-precision sensors for accurate data, enhancing its practicality in daily life.
3. **User-Tracking Mode**:

   - Utilizes advanced sensors to track the user's position, ensuring the robot always faces the user.
   - Facilitates focused and interactive companionship, especially useful during movement.

These modes collectively make the Cyberpet a multifaceted companion, blending emotional connection, environmental awareness, and responsive interaction.

## Key Component

1. Arduino UNO board
2. ESP32-CAM
3. ST7735 LCD Screen
4. DHT11 Temperature and Humidity Sensor
5. PCF8563 Real Time Clock (RTC) Module
6. Servo Motors

## Code Structure Explanation

In general, our code has four main bodies: CameraWebServer, Screen, ServoControl and facce_tracking.

### CameraWebServer

The `CameraWebServer` folder contains essential driver code enabling the ESP32-CAM module to capture and stream video data. Key features of this setup include:

1. **Video Data Streaming**: The code facilitates the ESP32-CAM to capture video and stream it effectively, ensuring a steady flow of video data.
2. **Targeted IP Address Transmission**: The streamed video data is sent to a specified IP address. This feature allows users to access the video stream by visiting the designated IP.
3. **.ino File Inclusion**:
   - The folder includes an `.ino` file, which is crucial for the functionality of this setup.
   - This file specifically deals with WiFi communication, enabling the ESP32-CAM to connect to a network and transmit video data.
   - Its focus is solely on WiFi communication, adhering to the policy of using `.ino` files for specific, limited applications.

### Screen Folder Overview

The "Screen" folder in our project is dedicated to handling the integration and functionality of various components: an LCD screen, a DHT11 Temperature and Humidity Sensor, and a PCF8563 Real Time Clock (RTC) module. Here's a breakdown of each component's role:

1. **LCD Screen Management**:

   - The code within this folder is designed to control the LCD screen, handling aspects such as display output, interface customization, and user interaction.
   - It includes functions for initializing the screen, drawing text and graphics, and updating the display based on various inputs or sensor readings.
2. **DHT11 Temperature and Humidity Sensor Integration**:

   - The DHT11 sensor is a common, low-cost digital sensor for measuring temperature and humidity.
   - The code in the "Screen" folder probably includes routines for reading data from the DHT11 sensor, processing this data, and displaying the readings on the LCD screen.
3. **PCF8563 RTC Module Usage**:

   - The PCF8563 is a Real Time Clock module, which provides accurate timekeeping.
   - This part of the code would handle the communication with the RTC module, ensuring that the current time and date are accurately maintained and can be displayed or used in conjunction with other functionalities, like logging sensor data with timestamps.

### ServoControl

The "ServoControl" folder is specifically designated for controlling servo motors within our project. Here's a closer look at its structure and functionality:

1. **Direct Servo Motor Control**:

   - The primary purpose of this folder is to manage the operation of servo motors. This includes commands for movement, positioning, and speed control of the servo.
2. **Consolidated Code in `main.c`**:

   - Instead of creating a separate library for motor control, all the necessary code for servo operation is consolidated within a single `main.c` file.
   - This approach keeps the servo control code straightforward and easily manageable, especially if the codebase is relatively small or simple.
3. **Features and Functions in `main.c`**:

   - Initialization of servo motor(s).
   - Functions to rotate the servo to specified angles or positions.
   - Code to handle the timing and pulse-width modulation (PWM) necessary for servo operation.
   - Possible integration with other components or sensors, depending on the project's requirements.

### Face Tracking

The script is a Python program for facial tracking using a video stream, often from an ESP32-CAM, combined with servo motor control. Key aspects of the script include:

1. **Importing Necessary Libraries**: The script uses `cv2` for OpenCV functions, `serial` for serial communication, and `time` for timing control.
2. **Face Detection and Tracking**:

   - Utilizes a Haar Cascade classifier for detecting faces in the video stream.
   - Processes video frames to detect faces and calculates the center of each detected face.
3. **Servo Motor Control**:

   - Communicates with a servo motor via a serial connection.
   - The `send_and_receive` function sends position data to the servo and receives feedback, although the receiving part is currently commented out to prevent blocking the camera updates.
4. **Video Stream Handling**:

   - The script can be configured to either use a video stream from an ESP32-CAM or the default camera connected to the computer.
   - Displays the video stream with detected faces highlighted in real time.
5. **Face Position Analysis**:

   - Determines if the detected face is close to the edge of the frame.
   - Sends appropriate commands to the servo motor to adjust its position based on the face's position relative to the center of the frame.
6. **Real-Time Interaction**:

   - Continuously reads video frames and adjusts the servo motor's position to keep the face centered.
   - Includes the capability to exit the loop with a 'q' key press.
7. **Clean Up**:

   - Releases the video stream and closes all OpenCV windows at the end of the session.
   - Closes the serial connection to the servo motor.

> The inclusion of a `Makefile` in almost every folder of our project is particularly important for streamlining the code uploading process, especially when working with a macOS system.
