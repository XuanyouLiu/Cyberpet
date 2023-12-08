// Conditional inclusion guard to ensure the code is compiled for the correct microcontroller
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

// Header guard to prevent multiple inclusions of this header file
#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>

// Define the port and pin to which the DHT11 sensor is connected
#define DHT11_DDR DDRD // Data Direction Register for the port of the DHT11
#define DHT11_PORT PORTD // Port Register for controlling the DHT11 pin
#define DHT11_PIN PIND // PIN Register for reading the state of the DHT11 pin
#define DHT11_INPUTPIN PD4 // The specific pin on the port to which the DHT11 is connected

// Declaration of the function for reading temperature and humidity from the DHT11 sensor
// This function will be defined in the DHT11.c source file
// It takes pointers to float variables for temperature and humidity as parameters
// Returns a uint8_t value indicating success (1) or failure (0) of the read operation
uint8_t DHT11_Read(float *temperature, float *humidity);

#endif // End of the header guard for DHT11_H
