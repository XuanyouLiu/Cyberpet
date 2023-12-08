#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>

// Initializes the UART with a specified BAUD_PRESCALER.
void UART_init(int BAUD_PRESCALER)
{
    // Set the baud rate using the prescaler value
    UBRR0H = (unsigned char)(BAUD_PRESCALER >> 8);
    UBRR0L = (unsigned char)BAUD_PRESCALER;

    // Enable both receiver and transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);

    // Set frame format: 8 data bits, 2 stop bits
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8 data bits
    UCSR0C |= (1 << USBS0);                  // 2 stop bits
}

// Sends a single byte of data via UART.
void UART_send(unsigned char data)
{
    // Wait until the transmit buffer is empty
    while (!(UCSR0A & (1 << UDRE0)));

    // Put the data into the buffer, which sends it
    UDR0 = data;
}

// Sends a string via UART.
void UART_putstring(char* StringPtr)
{
    // Iterate over the string and send each character
    while (*StringPtr != 0x00)
    {
        UART_send(*StringPtr);
        StringPtr++;
    }
}

// Receives a string via UART into a buffer with a specified size.
// Includes a timeout feature to prevent blocking if no data is received.
char* UART_receive(char* buf, unsigned int buf_size)
{
    unsigned int i = 0;
    unsigned char data;

    // Loop to receive data until the buffer is full
    while (i < (buf_size - 1)) 
    {
        // Check if data is available in the receive buffer
        // Includes a timeout using _delay_ms
        if (!(UCSR0A & (1 << RXC0)))
        {
            _delay_ms(1);
            if (!(UCSR0A & (1 << RXC0)))
            {
                break; // Break the loop if no data is received
            }
        }

        // Read the received data
        data = UDR0;

        // Check for newline or carriage return, which indicates the end of the input
        if (data == '\n' || data == '\r') {
            break;
        }

        // Store the data in the buffer
        buf[i] = data;
        i++;
    }
    buf[i] = '\0'; // Null-terminate the received string
    return buf;    // Return the buffer with the received data
}
