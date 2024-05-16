/**
 * @file car-logic.hpp
 * @brief This file contains the declaration of the functions for the car logic.
 * @author Braulio Silva 
*/
#ifndef CAR_LOGIC_HPP_
#define CAR_LOGIC_HPP_

#define DISTANCE_THRESHOLD  30

#define MOTOR       8
#define MOTOR_MIN   90
#define MOTOR_MAX   150

#define MOTOR_INIT  85  
#define MOTOR_STOP  90
#define MOTOR_GO    100

#define SHAFT       9
#define SHAFT_MIN   60
#define SHAFT_MAX   160

int pathPlanning(double leftSensorDistance, double rightSensorDistance, double forwardSensorDistance);

#endif