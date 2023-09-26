## Domotic Arduino with UI

This project provides a real-time view of the Arduino's actuators state and also allows users to control them with the UI.

Consisting of two parts:

* **Part 1:** A C++ program that is uploaded to the Arduino. This program ensures that the systems can work by itself and also guarantees the connection between the UI.
* **Part 2:** A Java program that is an UI where the actions taken in the Arduino are visible, and also you can make changes on the Arduino Board only using the UI.

The two parts of the project work together to create a synchronous environment with instant visibility of what's happening. 
This is done using Modbus communication protocol.



To use this project, follow these steps:

1. Upload the C++ program to the Arduino.
2. Run the Java program and choose the correct Serial Port.
3. Use the UI to interact with the Arduino Board and vice versa.

Contributing:

If you would like to contribute to this project, please feel free to submit a pull request.
