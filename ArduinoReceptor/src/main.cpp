#include <Arduino.h>
#include <Servo.h>

#include <sensor-utils.hpp>
#include <car-logic.hpp>
#include <esp32-communication.hpp>

#define BAUD_RATE   115200

sensorData_t sensorData;

messageESP msg;
String message;

Servo shaft;
Servo motor;
int velocity = 0;
int pos = 0;
int mode = 0;

void setup() {
  Serial.begin(BAUD_RATE); 
  while (!Serial) {
      ; 
    }  
  Serial1.begin(ESP32_BAUD); 
  Serial.println();

  pinMode(LEFT_TRIG, OUTPUT);
  pinMode(RIGHT_TRIG, OUTPUT);
  pinMode(FORWARD_TRIG, OUTPUT);
  pinMode(LEFT_ECHO, INPUT);
  pinMode(RIGHT_ECHO, INPUT);
  pinMode(FORWARD_ECHO, INPUT);

  shaft.attach(SHAFT);
  motor.attach(MOTOR);
  delay(1);

  motor.write(MOTOR_INIT);
  delay(5000);
  Serial.println("Motor ready");
}

void loop() {
  if (Serial1.available()){
    message = Serial1.readStringUntil('\n');
    msg.type = message[0];
    msg.value = message.substring(1).toInt();
    if (msg.type == 'M') {
        mode = msg.value;
    }  
  }

  if (mode == 0) {
    switch (msg.type) {
      case 'S':
        //Serial.println("Setting velocity: ");
        //Serial.println(msg.value);
        velocity = msg.value;
        velocity = msg.value;
        motor.write(transformToRange(MOTOR_MIN, MOTOR_MAX, velocity));
        break;

      case 'T':
        //Serial.println("Setting position: ");
        //Serial.println(msg.value);
        pos = msg.value;
        shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, pos));
        break;

      case 'C':
        Serial.write(message.c_str(), message.length());
        break;
    }
  } else {
    sensorData.leftSensorDistance = readUltrasonicSensor(LEFT_TRIG, LEFT_ECHO);
    sensorData.rightSensorDistance = readUltrasonicSensor(RIGHT_TRIG, RIGHT_ECHO);
    sensorData.forwardSensorDistance = readUltrasonicSensor(FORWARD_TRIG, FORWARD_ECHO);

    if (
      sensorData.leftSensorDistance < 0 
      || sensorData.rightSensorDistance < 0 
      || sensorData.forwardSensorDistance < 0
    ) {
      Serial.println("Error reading sensor");
    } else {
        int logicChoice = pathPlanning(
          sensorData.leftSensorDistance,
          sensorData.rightSensorDistance,
          sensorData.forwardSensorDistance
        );

        switch (logicChoice)
        {
        case 0:
          // Stop
          motor.write(MOTOR_STOP);
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 50));
          break;

        case 1:
          // Turn right
          motor.write(MOTOR_GO);
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 100));
          break;

        case 2:
          // Turn left
          motor.write(MOTOR_GO);
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 0));
          break;

        case 3:
          // Turn right
          motor.write(MOTOR_GO);
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 100));
          break;

        case 4:
          // Turn left
          motor.write(MOTOR_GO);
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 0));
          break;

        case 5:
          // Turn left or right just to avoid obstacle in front
          motor.write(MOTOR_GO);
          // Turn left in this case
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 0));
          break;  
        
        default:
          // Go straight
          motor.write(MOTOR_GO);
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 50));
          break;
        }
      }
    }     
}
