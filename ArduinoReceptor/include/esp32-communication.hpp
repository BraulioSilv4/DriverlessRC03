/**
 * @file esp32-communication.hpp
 * @brief This file contains the declaration of the functions for the ESP32 communication.
 * @author Braulio Silva
*/
#ifndef ESP32_COMMUNICATION_HPP_
#define ESP32_COMMUNICATION_HPP_

#define ESP32_BAUD  9600

struct messageESP
{
  char type;
  int value;
};

messageESP readESPmessage(char * message);


#endif