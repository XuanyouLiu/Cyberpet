#include "DHT11.h"
#include "uart.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// Function to initialize and send start signal to DHT11
static void DHT11_StartSignal(void)
{
    // Configure the DHT11 data pin as output
    DHT11_DDR |= (1 << DHT11_INPUTPIN);

    // Pull the pin low for at least 18 milliseconds to start signal
    DHT11_PORT &= ~(1 << DHT11_INPUTPIN);
    _delay_ms(18);

    // Pull the pin high for 20-40 microseconds to end the start signal
    DHT11_PORT |= (1 << DHT11_INPUTPIN);
    _delay_us(40);

    // Reconfigure the DHT11 data pin as input for receiving data
    DHT11_DDR &= ~(1 << DHT11_INPUTPIN);
}

// Function to check the response signal from DHT11
static uint8_t DHT11_CheckResponse(void)
{
    uint8_t response = 0;
    uint16_t current_time = 0;

    // Wait for the pin to go low indicating the start of the response signal
    while ((DHT11_PIN & (1 << DHT11_INPUTPIN)))
    {
        _delay_us(1);
        current_time++;

        // Break if waiting too long (more than 300 microseconds) without a response
        if (current_time > 300)
        {
            return 0;
        }
    }

    // A response was detected
    response = 1;
    return response;
}

// Function to read a single byte from DHT11
static uint8_t DHT11_ReadByte(void)
{
    uint8_t i, byte = 0;
    for (i = 0; i < 8; i++)
    {
        // Wait for the pin to go high indicating the start of a bit transmission
        while (!(DHT11_PIN & (1 << DHT11_INPUTPIN)));
        _delay_us(30);

        // Check the state of the pin after 30 microseconds to determine the bit value
        if (DHT11_PIN & (1 << DHT11_INPUTPIN))
        {
            byte |= (1 << (7 - i));
        }

        // Wait for the pin to go low indicating the end of bit transmission
        while (DHT11_PIN & (1 << DHT11_INPUTPIN));
    }
    return byte;
}

// Public function to read temperature and humidity from DHT11
uint8_t DHT11_Read(float *temperature, float *humidity)
{
    uint8_t integral_humidity, integral_temperature, checksum, decimal_humidity, decimal_temperature;
    uint8_t computed_checksum, response;

    // Send start signal to DHT11 and check for its response
    DHT11_StartSignal();
    response = DHT11_CheckResponse();

    // Proceed if response is received
    if (response)
    {
        // Check and wait for the DHT11 data pin to be ready for data transmission
        if (!(DHT11_PIN & (1 << DHT11_INPUTPIN)))
        {
            _delay_us(80);
            if (DHT11_PIN & (1 << DHT11_INPUTPIN))
            {
                _delay_us(80);

                // Read the humidity and temperature data along with the checksum
                integral_humidity = DHT11_ReadByte();
                decimal_humidity = DHT11_ReadByte(); // Not used in this implementation
                integral_temperature = DHT11_ReadByte();
                decimal_temperature = DHT11_ReadByte(); // Not used in this implementation
                checksum = DHT11_ReadByte();

                // Compute the checksum by adding all received bytes
                computed_checksum = (integral_humidity + integral_temperature + decimal_humidity + decimal_temperature) & 0xFF;

                // Compare the computed checksum with the received checksum
                if (checksum == computed_checksum)
                {
                    // Set the temperature and humidity values
                    *humidity = (float)integral_humidity;
                    *temperature = (float)integral_temperature;

                    // Return 1 to indicate successful data reading
                    return 1;
                }
            }
        }
    }

    // Return 0 if data reading fails
    return 0;
}
