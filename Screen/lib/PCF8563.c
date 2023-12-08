#include "PCF8563.h"
#include "I2C.h" // Include the I2C library header file

// Function to initialize the PCF8563 RTC chip
void PCF8563_Init(void) {
    I2C_init(100000); // Initialize I2C communication with 100kHz clock frequency
}

// Function to convert BCD (Binary-Coded Decimal) to binary
static uint8_t bcd2bin(uint8_t bcd) {
    return (bcd & 0x0F) + ((bcd >> 4) * 10); // Extract lower and upper nibbles and convert
}

// Function to convert binary to BCD
static uint8_t bin2bcd(uint8_t bin) {
    return ((bin / 10) << 4) | (bin % 10); // Convert to two BCD digits and combine
}

// Function to set the time on the PCF8563 RTC
void PCF8563_SetTime(const PCF8563_Time *time) {
    // Convert the year to BCD format, assuming the year is 2000 or later
    uint8_t year_bcd = bin2bcd((uint8_t)(time->years - 2000));

    // Start I2C communication and write the time data to the PCF8563
    I2C_start();
    I2C_write(PCF8563_ADDRESS << 1); // Send device address with write bit
    I2C_write(0x02); // Set the address pointer to the seconds register

    // Write time data in BCD format
    I2C_write(bin2bcd(time->seconds));
    I2C_write(bin2bcd(time->minutes));
    I2C_write(bin2bcd(time->hours));
    I2C_write(bin2bcd(time->days));
    I2C_write(bin2bcd(time->weekdays));
    I2C_write(bin2bcd(time->months));
    I2C_write(year_bcd);

    I2C_stop(); // Stop I2C communication
}

// Function to get the time from the PCF8563 RTC
void PCF8563_GetTime(PCF8563_Time *time) {
    // Start I2C communication and set the address pointer to the seconds register
    I2C_start();
    I2C_write(PCF8563_ADDRESS << 1); // Send device address with write bit
    I2C_write(0x02); // Set the address pointer to the seconds register

    // Restart I2C communication for reading
    I2C_start(); // Repeat start for read operation
    I2C_write((PCF8563_ADDRESS << 1) | 0x01); // Send device address with read bit

    // Read time data in BCD format and convert to binary
    time->seconds = bcd2bin(I2C_read_ack() & 0x7F); // Mask VL bit for seconds
    time->minutes = bcd2bin(I2C_read_ack());
    time->hours = bcd2bin(I2C_read_ack() & 0x3F); // 24-hour format assumed
    time->days = bcd2bin(I2C_read_ack() & 0x3F);
    time->weekdays = bcd2bin(I2C_read_ack());
    time->months = bcd2bin(I2C_read_ack() & 0x1F);
    time->years = 2000 + bcd2bin(I2C_read_nack()); // Convert to full year

    I2C_stop(); // Stop I2C communication
}
