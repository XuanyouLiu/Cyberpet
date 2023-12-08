#ifndef UART_H
#define UART_H

// Function to initialize UART communication.
// The 'prescale' parameter is used to determine the baud rate for UART communication.
void UART_init(int prescale);

// Function to send a single byte of data over UART.
// 'data' is the byte to be sent.
void UART_send(unsigned char data);

// Function to send a string over UART.
// 'StringPtr' is a pointer to the string to be sent.
void UART_putstring(char* StringPtr);

// Function to receive a string over UART and store it in a buffer.
// 'buf' is a pointer to the buffer where the received string will be stored.
// 'buf_size' is the size of the buffer, which limits the number of bytes that can be received.
// Returns a pointer to the buffer containing the received string.
char* UART_receive(char* buf, unsigned int buf_size);

#endif // UART_H
