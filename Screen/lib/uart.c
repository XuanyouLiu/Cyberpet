#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "uart.h"
#include <avr/io.h>

// Function to initialize UART
void UART_init(int BAUD_PRESCALER)
{
    // Set the baud rate
    UBRR0H = (unsigned char)(BAUD_PRESCALER >> 8); // Upper 8 bits of the baud rate value
    UBRR0L = (unsigned char)BAUD_PRESCALER;        // Lower 8 bits of the baud rate value

    // Enable receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);          // RXEN0: Receiver Enable, TXEN0: Transmitter Enable

    // Set frame format to 8 data bits and 2 stop bits
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);        // UCSZ01 and UCSZ00: 8-bit character size
    UCSR0C |= (1 << USBS0);                        // USBS0: 2 stop bits
}

// Function to send a single byte via UART
void UART_send(unsigned char data)
{
    // Wait for the transmit buffer to be empty
    while (!(UCSR0A & (1 << UDRE0)));              // UDRE0: UART Data Register Empty Flag

    // Load data into the UART data register; this starts transmission
    UDR0 = data;
}

// Function to send a string via UART
void UART_putstring(char* StringPtr)
{
    // Loop through each character in the string until reaching the null terminator
    while (*StringPtr != 0x00)
    {
        UART_send(*StringPtr);                     // Send the current character
        StringPtr++;                               // Move to the next character
    }
}

// Function to receive a string via UART
char* UART_receive(char* buf, unsigned int buf_size)
{
    unsigned int i = 0;
    unsigned char data;

    // Loop to receive characters until the buffer is full
    while (i < (buf_size - 1))
    {
        // Wait for data to be available in the receive buffer
        while (!(UCSR0A & (1 << RXC0)));           // RXC0: UART Receive Complete Flag

        data = UDR0;                               // Read the received data

        // Check for newline or carriage return (end of message)
        if (data == '\n' || data == '\r')
        {
            break;
        }

        // Store the received character in the buffer
        buf[i] = data;
        i++;
    }
    buf[i] = '\0';                                // Null-terminate the received string
    return buf;                                   // Return the buffer with received data
}
