#ifndef UART_H
#define UART_H

// Initializes the UART communication with a specified prescaler value.
// The 'prescale' parameter determines the baud rate for UART communication.
void UART_init(int prescale);

// Sends a single byte of data over UART.
// 'data' is the byte to be transmitted.
void UART_send(unsigned char data);

// Sends a string over UART.
// 'StringPtr' is a pointer to the string that needs to be sent.
void UART_putstring(char* StringPtr);

// Receives a string via UART and stores it in the provided buffer.
// 'buf' is a pointer to the buffer where the received string will be stored.
// 'buf_size' is the size of the buffer, which limits the number of bytes that can be received.
// The function returns a pointer to the buffer containing the received string.
char* UART_receive(char* buf, unsigned int buf_size);

#endif // UART_H