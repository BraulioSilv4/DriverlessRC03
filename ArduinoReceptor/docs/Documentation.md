This software was developed by Braulio Silva.

### Introduction (Planing)
This software is intended to be used in a RC car, with the objective of achiving some degree of autonomy while driving. Two [HC-SR05](docs\resources\M_HY-SRF05_0003.pdf) ultrasonic sensors are mounted on an arduino used to detect obstacles in front of the car, messages of the distance and errors will be sent over serial communication to the ROS 2 system which will be responsible for controlling the logic of the car, such as sending commands to the motors to stop or turn.   
![Driverless System](resources\DriverlessSys.png)

Ideas
 - Calculate the threshold of stop based on the velocity
 - Calculate the velocity
 - Calculate the distance in front of the car with some trigonometry sheninigans

## Breadboard Setup
Here we can visualize the setup of the sensors on the breadboard. The sensors are connected to the arduino using the following pins:
![Arduino Setup with HC-SR05](resources\ArduinoSetup.png)
This setup uses an arduino uno and two HC-SR04 sensors.
The setup should be similiar with arduino due and HC-SR05 sensors.