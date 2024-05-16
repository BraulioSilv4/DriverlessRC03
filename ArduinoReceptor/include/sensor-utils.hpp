/**
 * @file sensor-utils.hpp
 * @brief This file contains the declaration of the functions for the sensors.
 * @author Braulio Silva
*/
#ifndef SENSOR_UTILS_HPP_
#define SENSOR_UTILS_HPP_

#define FORWARD_TRIG    7
#define FORWARD_ECHO    6
#define LEFT_TRIG       5
#define LEFT_ECHO       4
#define RIGHT_TRIG      3
#define RIGHT_ECHO      2

struct sensorData_t
{
  double leftSensorDistance;
  double rightSensorDistance;
  double forwardSensorDistance;
};

void startUltrasonicSensor(int trigPin, int echoPin); 

long readUltrasonicSensor(int trigPin, int echoPin);

int transformToRange(int min_value, int max_value, int value);

#endif
