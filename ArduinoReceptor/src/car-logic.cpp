/**
 * @file car-logic.cpp
 * @brief This file contains the implementation of the functions for the car logic.
 * @author Braulio Silva
*/
#include <Arduino.h>
#include <Servo.h>

#include <car-logic.hpp>

/**
 * @brief This function calculates the path the car should take based on the sensor data.
 * @param leftSensorDistance The distance read by the left sensor.
 * @param rightSensorDistance The distance read by the right sensor.
 * @param forwardSensorDistance The distance read by the forward sensor.
 * @return An int representig the state of the environment the car is in.
 * @example 0 - Stop, 1 - Turn right, 2 - Turn left, 3 - Turn right, 4 - Turn left, 5 - Turn left or right, 6 - Go straight
 * @see docs/Documentation.md
*/
int pathPlanning(
  double leftSensorDistance,
  double rightSensorDistance,
  double forwardSensorDistance
) {
  if (
    forwardSensorDistance < DISTANCE_THRESHOLD &&
    leftSensorDistance < DISTANCE_THRESHOLD &&
    rightSensorDistance < DISTANCE_THRESHOLD) {
    return 0;
  } else if (forwardSensorDistance < DISTANCE_THRESHOLD && leftSensorDistance < DISTANCE_THRESHOLD) {
    return 1;
  } else if (forwardSensorDistance < DISTANCE_THRESHOLD && rightSensorDistance < DISTANCE_THRESHOLD) {
    return 2;
  } else if (leftSensorDistance < DISTANCE_THRESHOLD) {
    return 3;
  } else if (rightSensorDistance < DISTANCE_THRESHOLD) {
    return 4;
  } else if (forwardSensorDistance < DISTANCE_THRESHOLD) {
    return 5;
  } else {
    return 6;
  }
}