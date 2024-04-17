This software was developed by Braulio Silva.

### Introduction (Planing)
This software is intended to be used in a RC car, with the objective of achiving some degree of autonomy while driving. Two [HC-SR05](docs\resources\M_HY-SRF05_0003.pdf) ultrasonic sensors are mounted on an arduino used to detect obstacles in front of the car, messages of the distance and errors will be sent over serial communication to the ROS 2 system which will be responsible for controlling the logic of the car, such as sending commands to the motors to stop or turn.   
![Driverless System](resources\DriverlessSys.png)

Ideas
 - Calculate the threshold of stop based on the velocity. This needs to be done since the car will not stop instantly and turning with bigger speeds will result in bigger radius.
 - Calculate the velocity
 - Calculate the distance in front of the car with some trigonometry sheninigans

## Breadboard Setup
Here we can visualize the setup of the sensors on the breadboard. The sensors are connected to the arduino using the following pins:
- Sensor 1:
    - Vcc: Arduino 5V Pin
    - Trigger: Pin I/O 7
    - Echo: Pin I/O 6
    - GND: Arduino GND 
- Sensor 2:
    - Vcc: Arduino 5V Pin
    - Trigger: Pin I/O 8
    - Echo: Pin I/O 9
    - GND: Arduino GND 

![Arduino Setup with HC-SR05](resources\ArduinoSetup.png)
This setup uses an arduino uno and two HC-SR04 sensors.
The setup should be similiar with arduino due and HC-SR05 sensors. We can observe three green LEDs which indicate where the car should go, and one red LED which indicates that the car should stop. The LEDs are connected to the arduino using the following pins:
 - Green LEDs: Pin I/O 2, 3, 4
 - Red LED: Pin I/O 5

## ROS 2 Setup

## Sensor Setup
(In the working)
The ideia is to have the sensors be pointed in inverse directions to be able to detect obstacles in front of the car. If the distance is inferior to the intersection distance on both sensors the car should stop. Here is an illustrative image of the sensors:

![Sensor Setup](resources\PaintMaster.png) 

### Demonstration Logic
Each green LED represents a direction the car should go, the left LED represents the left direction, the middle LED represents the forward direction and the right LED represents the right direction. The red LED represents that the car should stop. Dont forget that while the distance of the sensor is bigger than the intersection distance, the direction the car should turn is inversed.The logic of the car is as follows:
- If the distance of the left sensor is less than the threshold, the car should turn left or continue to move forward.
- If the distance of the right sensor is less than the threshold, the car should turn right or continue to move forward.
- If the distance of both sensors is equal or inferior to the [intersection distance of the sensors](#sensor-setup), this means there is an obstacle in front of the car, now there is multiple possibilities:
    - If both distances are equal to the threshold, the car can either turn left or right.
    - If the distance of the left sensor is less than the distance of the right sensor, the car should turn right.
    - If the distance of the right sensor is less than the distance of the left sensor, the car should turn left.
    - If the distance of the left sensor is equal to the distance of the right sensor while being under the intersection distance, the car should stop.