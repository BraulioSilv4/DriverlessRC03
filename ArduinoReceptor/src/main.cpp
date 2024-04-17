#include <Arduino.h>

const int TRIG_PIN1 = 7;
const int TRIG_PIN2 = 8;
const int ECHO_PIN1 = 6;
const int ECHO_PIN2 = 9;
const int BAUD_RATE = 9600;
double currVelocity = 0.0;
double velocityBuffer[10];

void startUltrasonicSensor(int trigPin, int echoPin);
long readUltrasonicSensor(int trigPin, int echoPin);

/*
* All these functions from here will need to be implemented in ROS2 system.
* For now they will be all implemented in arduino.
*/ 
int pathPlanning(double leftSensorDistance, double rightSensorDistance);
double calculateVelocity(double velocityBuffer[]);
void send2ROS2();
char * receiveFromROS2();

void setup() {
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(ECHO_PIN2, INPUT);

  Serial.begin(9600);
}

void loop() {
  long leftSensorDistance, rightSensorDistance;
  leftSensorDistance = readUltrasonicSensor(TRIG_PIN1, ECHO_PIN1);
  rightSensorDistance = readUltrasonicSensor(TRIG_PIN2, ECHO_PIN2);

  if (leftSensorDistance < 0 || rightSensorDistance < 0) {
    Serial.println("Error reading sensor");
  } else {
    // Send over serial to ros2 system.
    Serial.print("Left sensor distance: ");
    Serial.print(leftSensorDistance);
    Serial.print(" cm, Right sensor distance: ");
    Serial.print(rightSensorDistance);
    Serial.println(" cm");
  }

  // Receive data from ros2 system.

  // This delay needs to be tested 
  delay(50);
}


void startUltrasonicSensor(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

long readUltrasonicSensor(int trigPin, int echoPin) {
  startUltrasonicSensor(trigPin, echoPin);
  long duration = pulseIn(echoPin, HIGH);
  return (duration * 0.034) / 2;
}

/*
* All these functions from here will need to be implemented in ROS2 system.
* For now they will be all implemented in arduino.
*/ 
int pathPlanning(double leftSensorDistance, double rightSensorDistance) {
  // This function will use the leftSensorDistance and rightSensorDistance to calculate the path of the car.
  return 0;
}

double calculateVelocity(double velocityBuffer[]) {
  // This function will use the velocityBuffer to calculate the velocity of the car.
  return 0;
}

void send2ROS2() {
  // This function will send data to the ROS2 system.
}

char * receiveFromROS2() {
  // This function will receive data from the ROS2 system.
  return "Hello";
}