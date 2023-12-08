#ifndef I2C_H
#define I2C_H

// This section is not typical for an I2C.h file. It seems to be a guard for the specific microcontroller.
#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <stdint.h>

// Define the CPU frequency if it's not already defined. 
// It's important for I2C timing calculations. The default is set to 16MHz.
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// Function declarations for I2C communication:

// Initialize the I2C interface with a specific SCL (clock) frequency.
void I2C_init(uint32_t scl_freq);

// Start an I2C communication. This function is used to begin a transmission 
// on the I2C bus and is usually followed by the address of the I2C device.
uint8_t I2C_start(void);

// Stop an I2C communication. This function sends a stop condition, 
// indicating the end of a transmission.
void I2C_stop(void);

// Write a byte of data to the I2C bus. This is used for sending data to an I2C device.
uint8_t I2C_write(uint8_t data);

// Read a byte from the I2C bus with acknowledgment. This function is used when 
// you are expecting more data from the I2C device.
uint8_t I2C_read_ack(void);

// Read a byte from the I2C bus without acknowledgment. This is usually used to 
// read the last byte of data from an I2C device.
uint8_t I2C_read_nack(void);

#endif // I2C_H
