# Firebase Realtime LED Control for ESP32 Nodes

This project demonstrates how to control LEDs connected to ESP32 nodes using Firebase Realtime Database. It consists of two Arduino sketches designed to run on ESP32 boards, enabling remote control of LEDs through a Firebase database.

## Features

- Realtime LED Control: Change the state of LEDs in real-time through the Firebase console or API.
- Multiple Node Support: Control LEDs connected to different ESP32 nodes individually within the same Firebase database.
- Automatic Reconnection: Automatically reconnect to the WiFi and Firebase service in case of disconnection.

## File Description

opt_1.ino: Arduino sketch for ESP32 Node 1. It includes FirebaseESP32 library and defines functions for connecting to WiFi, initializing Firebase, and controlling the LED based on Firebase data streams.

opt_2.ino: Arduino sketch for ESP32 Node 2. Similar to opt_1.ino, this sketch connects to WiFi, initializes Firebase, and controls the LED based on Firebase data streams. It utilizes a different library, Firebase_ESP_Client, for Firebase integration.

## Getting Started
- Setup Firebase: Create a Firebase project on the Firebase Console. Note down the API key and database URL.

- Hardware Setup: Connect LEDs to GPIO pins of your ESP32 boards.

- Configuration: Update the WiFi SSID, password, API key, and database URL in the sketches.

- Upload Sketches: Upload opt_1.ino to Node 1 and opt_2.ino to Node 2 using the Arduino IDE or preferred development environment.

- Monitor Output: Open the serial monitor to view connection status and LED control messages.

- Control LEDs: Use the Firebase console or API to update LED status in real-time.

## Dependencies
- FirebaseESP32: Library for Firebase integration on ESP32, used in opt_1.ino.
- Firebase_ESP_Client: Another Firebase library for ESP32, used in opt_2.ino.

## Note
- Ensure that your Firebase database rules allow read and write access to the appropriate paths for LED control.
