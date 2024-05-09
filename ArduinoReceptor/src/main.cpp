#include <Arduino.h>
#include <Servo.h>

struct 
{
  double leftSensorDistance;
  double rightSensorDistance;
  double forwardSensorDistance;
} sensorData;

struct messageESP
{
  char type;
  int value;
} message;

const int FORWARD_TRIG = 7;
const int LEFT_TRIG = 5;
const int RIGHT_TRIG = 3;
const int FORWARD_ECHO = 6;
const int LEFT_ECHO = 4;
const int RIGHT_ECHO = 2;

const int MOTOR = 8;
const int MOTOR_MIN = 90;
const int MOTOR_MAX = 120;

const int SHAFT = 9;
const int SHAFT_MIN = 70;
const int SHAFT_MAX = 150;

const int BAUD_RATE = 115200;

void startUltrasonicSensor(int trigPin, int echoPin);
long readUltrasonicSensor(int trigPin, int echoPin);

int transformToRange(int min_value, int max_value, int value);
int pathPlanning(double leftSensorDistance, double rightSensorDistance, double forwardSensorDistance);
messageESP readESPmessage(char * message);

Servo shaft;
Servo motor;
int velocity = 0;
int pos = 0;
int mode = 0;

void setup() {
  Serial.begin(BAUD_RATE);   // initialise serial monitor port
  while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }  
  Serial1.begin(9600);  // initialise Serial1
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

  motor.write(85);
  delay(5000);
  Serial.println("Motor ready");
}

void loop() {
  messageESP msg;
  String message;

  if (Serial1.available()){
    message = Serial1.readStringUntil('\n');
    msg.type = message[0];
    msg.value = message.substring(1).toInt();
    if (msg.type == 'M') {
        Serial.println("Setting mode: ");
        Serial.println(msg.value);
        mode = msg.value;
        Serial.println("Mode: ");
        Serial.println(mode);
    }  
  }


  if (mode == 0) {
    if (msg.type == 'S') {
        Serial.println("Setting velocity: ");
        Serial.println(msg.value);
        velocity = msg.value;
        motor.write(transformToRange(MOTOR_MIN, MOTOR_MAX, velocity));
    } else if (msg.type == 'T') {
        Serial.println("Setting position: ");
        Serial.println(msg.value);
        if (pos != msg.value) {
          pos = msg.value;
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, pos));
        } 
    } else if (msg.type == 'C') {
        Serial.write(message.c_str(), message.length());
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
        // Serial.print("Left sensor distance: ");
        // Serial.print(sensorData.leftSensorDistance);
        // Serial.print(" cm, Right sensor distance: ");
        // Serial.print(sensorData.rightSensorDistance);
        // Serial.print(" cm, Forward sensor distance: ");
        // Serial.print(sensorData.forwardSensorDistance);
        // Serial.println(" cm");

        int logicChoice = pathPlanning(
          sensorData.leftSensorDistance,
          sensorData.rightSensorDistance,
          sensorData.forwardSensorDistance
        );

        // Serial.print("Logic choice: ");
        // Serial.println(logicChoice);

        delayMicroseconds(2000);

        switch (
          logicChoice
        )
        {
        case 0:
          // Stop
          motor.write(90);
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 50));
          break;

        case 1:
          // Turn right
          motor.write(100);
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 100));
          break;

        case 2:
          // Turn left
          motor.write(100);
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 0));
          break;

        case 3:
          // Turn right
          motor.write(100);
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 100));
          break;

        case 4:
          // Turn left
          motor.write(100);
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 0));
          break;

        case 5:
          // Turn left or right just to avoid obstacle in front
          motor.write(100);
          // Turn left in this case
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 0));
          break;  
        
        default:
          // Go straight
          motor.write(100);
          shaft.write(transformToRange(SHAFT_MIN, SHAFT_MAX, 50));
          break;
        }
      }
    }     
}

int transformToRange(int min_value, int max_value, int value) {
  // min value = 0, max value = 180 value = 0..100
  return (value * (max_value - min_value) / 100) + min_value;
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
  return (duration*.0343)/2;
}

messageESP readESPmessage(char * message) {
  // Example: message = "S100", type = S, value = 100
  messageESP msg;
  msg.type = message[0];
  msg.value = atoi(message+1);
  return msg;
}

int pathPlanning(
  double leftSensorDistance,
  double rightSensorDistance,
  double forwardSensorDistance
) {
  if (forwardSensorDistance < 30 && leftSensorDistance < 30 && rightSensorDistance < 30) {
    return 0;
  } else if (forwardSensorDistance < 30 && leftSensorDistance < 30) {
    return 1;
  } else if (forwardSensorDistance < 30 && rightSensorDistance < 30) {
    return 2;
  } else if (leftSensorDistance < 30) {
    return 3;
  } else if (rightSensorDistance < 30) {
    return 4;
  } else if (forwardSensorDistance < 30) {
    return 5;
  } else {
    return 6;
  }
}