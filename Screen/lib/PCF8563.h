#ifndef PCF8563_H
#define PCF8563_H

#include <stdint.h>

// Define the I2C address for the PCF8563 RTC module
#define PCF8563_ADDRESS 0x51  

// Structure to hold the time information in a human-readable format
typedef struct {
    uint8_t seconds;  // Stores seconds (0-59)
    uint8_t minutes;  // Stores minutes (0-59)
    uint8_t hours;    // Stores hours (0-23)
    uint8_t days;     // Stores day of the month (1-31)
    uint8_t weekdays; // Stores day of the week (0-6)
    uint8_t months;   // Stores month (1-12)
    uint8_t years;    // Stores year (0-99, representing 2000-2099)
} PCF8563_Time;

// Function to initialize the PCF8563 RTC module
void PCF8563_Init(void);

// Function to set the current time on the PCF8563 RTC module
// It expects a pointer to a `PCF8563_Time` structure containing the new time to be set
void PCF8563_SetTime(const PCF8563_Time *time);

// Function to get the current time from the PCF8563 RTC module
// It populates a `PCF8563_Time` structure with the current time
void PCF8563_GetTime(PCF8563_Time *time);

#endif // PCF8563_H
