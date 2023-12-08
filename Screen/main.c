#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

// Define constants for the CPU clock frequency and UART baud rate
#define F_CPU 16000000UL // CPU frequency: 16 MHz
#define BAUD_RATE 9600 // Baud rate for serial communication
// Calculate the baud rate prescaler for UART configuration
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

// Include header files for various functionalities
#include "lib/uart.h"  // UART communication
#include "lib/DHT11.h"  // DHT11 temperature and humidity sensor
#include "lib/LCD_GFX.h"  // LCD graphics
#include "lib/ST7735.h"  // ST7735 LCD driver
#include "lib/PCF8563.h"  // PCF8563 real-time clock
#include "lib/I2C.h"  // I2C communication
#include <avr/io.h>  // Basic input/output functions for AVR
#include <util/delay.h>  // Functions for delays
#include <stdio.h>  // Standard input/output functions
#include <stdlib.h>  // Standard library functions
#include <avr/interrupt.h>  // Interrupt handling

// Global variables for state management
volatile uint8_t flagState = 0;
volatile uint8_t TurnFlag = 0;

// Function to display time, temperature, and humidity
void printTimeTemp()
{
    PCF8563_Time time; // Variable to store current time
    PCF8563_GetTime(&time); // Retrieve current time from RTC

    // Display time on LCD and send over UART
    char timeLCD[20];
    sprintf(timeLCD, "Time:%d:%d", (int)time.hours, (int)time.minutes);
    LCD_drawString(10, 10, timeLCD, WHITE, BLACK, 2);
    UART_putstring(timeLCD);

    // Display date on LCD and send over UART
    char dateLCD[20];
    sprintf(dateLCD, "Date:%d/%d", time.months, time.days);
    LCD_drawString(10, 40, dateLCD, WHITE, BLACK, 2);
    UART_putstring(dateLCD);

    // Read temperature and humidity from DHT11 sensor
    float temperature, humidity;
    if (DHT11_Read(&temperature, &humidity))
    {
        // Display temperature and humidity on LCD and UART
        char tempLCD[20];
        sprintf(tempLCD, "Temp:%dC", (int)temperature);
        LCD_drawString(10, 70, tempLCD, WHITE, BLACK, 2);
        UART_putstring(tempLCD);

        char humLCD[20];
        sprintf(humLCD, "Humid:%d%%", (int)humidity);
        LCD_drawString(10, 100, humLCD, WHITE, BLACK, 2);
        UART_putstring(humLCD);
    }
    else
    {
        // Display error messages if sensor read fails
        LCD_drawString(10, 70, "Temp: Error", WHITE, BLACK, 2);
        LCD_drawString(10, 100, "Humid: Error", WHITE, BLACK, 2);
        UART_putstring("Temp: Error");
        UART_putstring("Humid: Error");
    }

    _delay_ms(1000); // Delay to stabilize the display
}

// Function to initialize switch inputs for external interrupts
void initSwitch()
{
    DDRD &= ~(1 << PIND2); // Set PD2 (INT0) as input
    EICRA |= (1 << ISC01) | (1 << ISC00); // Trigger INT0 on rising edge
    EIMSK |= (1 << INT0); // Enable INT0

    DDRD &= ~(1 << PIND3); // Set PD3 (INT1) as input
    EICRA |= (1 << ISC11) | (1 << ISC10); // Trigger INT1 on rising edge
    EIMSK |= (1 << INT1); // Enable INT1
    sei(); // Enable global interrupts
}

// ISR for INT1 (switch press)
ISR(INT1_vect)
{
    // Debounce logic to ensure stable switch input
    _delay_ms(10); 
    if (PIND & (1 << PIND3)) 
    {
        _delay_ms(10);
        if (PIND & (1 << PIND3)) 
        {
            // Toggle TurnFlag and flagState based on switch state
            // ... [rest of the logic]
        }
    }
}

// ISR for INT0 (switch press)
ISR(INT0_vect)
{
    // Similar debounce logic as in ISR(INT1_vect)
    // Toggle flagState based on switch state
    // ... [rest of the logic]
}

// Function to display random expressions
void randomExpression()
{
    int emotion = rand() % 3; // Generate a random number between 0 and 2
    // Display different expressions based on the random number
    // ... [rest of the function implementation]
}

int main(void)
{
    // Initialization routines
    UART_init(BAUD_PRESCALER); // Initialize UART with calculated prescaler
    lcd_init(); // Initialize LCD display
    initSwitch(); // Initialize switches for input
    LCD_setScreen(BLACK); // Set LCD screen to black

    // Initialize PCF8563 real-time clock
    PCF8563_Init();
    PCF8563_Time currentTime = {
        .seconds = 0,
        .minutes = 20,
        .hours = 11,
        .days = 3,
        .weekdays = 1,
        .months = 12,
        .years = 23 // Set initial time (Year: 2023)
    };
    PCF8563_SetTime(&currentTime); // Set time in RTC

    srand(TCNT0); // Seed the random number generator

    // Main loop
    while (1)
    {
        if (flagState == 0 && TurnFlag == 0)
        {
            printTimeTemp(); // Display time and temperature
        }
        else
        {
            randomExpression(); // Display random expressions
        }

        LCD_setScreen(BLACK); // Clear the screen for next display
    }
}
