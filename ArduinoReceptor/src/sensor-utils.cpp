/**
 * @file sensor-utils.cpp
 * @brief This file contains the implementation of the functions for the sensors.
 * @author Braulio Silva
*/
#include <Arduino.h>
#include <sensor-utils.hpp>

void startUltrasonicSensor(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
}

/**
 * This function reads the distance from the ultrasonic sensor, and converts it into a distance in cm.
 * Constant 0.0343 is the speed of sound in cm/microseconds, which is multiplied by the time it took for the sound to return.
*/
long readUltrasonicSensor(int trigPin, int echoPin) {
  startUltrasonicSensor(trigPin, echoPin);
  long duration = pulseIn(echoPin, HIGH);
  return (duration*.0343)/2;
}

/**
 * This function transforms a value from a range of 0-100 to a range of min_value-max_value.
 * Used for calibrating the motor and shaft servos.
*/
int transformToRange(int min_value, int max_value, int value) {
  return (value * (max_value - min_value) / 100) + min_value;
}
