#include "I2C.h"

#define I2C_TIMEOUT 10000  // Define a timeout count for I2C operations

// Helper function to check if an operation has timed out
static uint8_t I2C_checkTimeout(uint32_t* timeoutCounter) {
    // Increment the timeout counter and check if it exceeds the predefined limit
    if ((*timeoutCounter)++ > I2C_TIMEOUT) {
        *timeoutCounter = 0;
        return 1;  // Timeout has occurred
    }
    return 0;  // No timeout
}

// Function to initialize the I2C interface
void I2C_init(uint32_t scl_freq) {
    TWSR = 0x00;  // Set prescaler to 1
    // Calculate and set the bitrate generator value for desired SCL frequency
    TWBR = (uint8_t)((((F_CPU / scl_freq) / 1) - 16 ) / 2);  
    TWCR = (1 << TWEN);  // Enable TWI (Two Wire Interface)
}

// Function to start I2C communication
uint8_t I2C_start(void) {
    uint32_t timeoutCounter = 0;

    // Send a start condition and wait for the end of the start condition
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT))) {
        if (I2C_checkTimeout(&timeoutCounter)) return 1; // Return 1 if timeout
    }
    return 0; // Return 0 if successful
}

// Function to stop I2C communication
void I2C_stop(void) {
    // Send a stop condition
    TWCR = (1 << TWSTO) | (1 << TWEN);
}

// Function to write a byte of data to the I2C bus
uint8_t I2C_write(uint8_t data) {
    uint32_t timeoutCounter = 0;

    TWDR = data; // Load data into the I2C data register
    TWCR = (1 << TWEN) | (1 << TWINT); // Start data transmission
    while (!(TWCR & (1 << TWINT))) {
        if (I2C_checkTimeout(&timeoutCounter)) return 1; // Return 1 if timeout
    }
    return 0; // Return 0 if successful
}

// Function to read a byte from the I2C bus with acknowledgment
uint8_t I2C_read_ack(void) {
    uint32_t timeoutCounter = 0;

    // Enable TWI, clear TWINT, and send ACK after reception
    TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT))) {
        if (I2C_checkTimeout(&timeoutCounter)) return 0xFF;  // Return 0xFF if timeout
    }
    return TWDR; // Return the received data
}

// Function to read a byte from the I2C bus without acknowledgment
uint8_t I2C_read_nack(void) {
    uint32_t timeoutCounter = 0;

    // Enable TWI and clear TWINT, without sending ACK after reception
    TWCR = (1 << TWEN) | (1 << TWINT);
    while (!(TWCR & (1 << TWINT))) {
        if (I2C_checkTimeout(&timeoutCounter)) return 0xFF;  // Return 0xFF if timeout
    }
    return TWDR; // Return the received data
}
