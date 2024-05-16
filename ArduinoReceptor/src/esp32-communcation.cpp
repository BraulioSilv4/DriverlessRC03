/**
 * @file esp32-communication.cpp
 * @brief This file contains the implementation of the functions for the ESP32 communication.
 * @author Braulio Silva
*/
#include <Arduino.h>
#include <esp32-communication.hpp>

/**
 * This function reads a message from the ESP32 into a messageESP structure.
 * @example message = "S100", type = S, value = 100
*/
messageESP readESPmessage(char * message) {
  messageESP msg;
  msg.type = message[0];
  msg.value = atoi(message+1);
  return msg;
}
