/**************************************************************************//**
* @file        ST7735.c
* @ingroup 	   ST7735
* @brief       Basic display driver for Adafruit 358 1.8" TFT LCD with ST7735R chip
*
* @details     Basic display driver for Adafruit 358 1.8" TFT LCD with ST7735R chip
*
*
* @copyright
* @author	   J. Ye
* @date        April 19, 2021
* @version		1.0
*****************************************************************************/

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include "ST7735.h"

/******************************************************************************
* Local Functions
******************************************************************************/

/**************************************************************************//**
* @fn			static void lcd_pin_init(void)
* @brief		Initialize SPI for LCD
* @note
*****************************************************************************/
static void lcd_pin_init(void)
{
	//Setup digital pins
	LCD_DDR |= (1<<LCD_DC)|(1<<LCD_RST)|(1<<LCD_TFT_CS)|(1<<LCD_MOSI)|(1<<LCD_SCK);	//Set up output pins
	LCD_LITE_DDR |= (1<<LCD_LITE);	//Set up output pins

	//Setup PWM for LCD Backlight
	TCCR0A |= (1<<COM0A1)|(1<<WGM01)|(1<<WGM00);	//Fast PWM: clear OC0A on match, set at bottom
	TCCR0B |= (1<<CS02);	//clk/1024/256=244Hz
	OCR0A = 127;	//Set starting PWM value

	//Enable LCD by setting RST high
	_delay_ms(50);
	set(LCD_PORT, LCD_RST);
}

/**************************************************************************//**
* @fn			static void SPI_Controller_Init(void)
* @brief		Initialize SPI for LCD
* @note
*****************************************************************************/
static void SPI_Controller_Init(void)
{
	SPCR = (1<<SPE) | (1<<MSTR);		//Enable SPI, Master, set clock rate fck/64
	SPSR = (1<<SPI2X);										//SPI 2X speed
}


/******************************************************************************
* Global Functions
******************************************************************************/

/**************************************************************************//**
* @fn			void Delay_ms(unsigned int n)
* @brief		Delay function using variables
* @note
*****************************************************************************/
void Delay_ms(unsigned int n)
{
	while (n--)
	{
		_delay_ms(1);
	}
}

/**************************************************************************//**
* @fn			void SPI_ControllerTx(uint8_t data)
* @brief		Send 8-bit SPI data to peripheral
* @note
*****************************************************************************/
void SPI_ControllerTx(uint8_t data)
{
	clear(LCD_PORT, LCD_TFT_CS);	//CS pulled low to start communication

	SPI_ControllerTx_stream(data);

	set(LCD_PORT, LCD_TFT_CS);	//set CS to high
}

/**************************************************************************//**
* @fn			void SPI_ControllerTx_stream(uint8_t stream)
* @brief		Send a command to LCD through SPI without setting CS or DC
* @note
*****************************************************************************/
void SPI_ControllerTx_stream(uint8_t stream)
{
	SPDR = stream;		//Place data to be sent on registers
	while(!(SPSR & (1<<SPIF)));	//wait for end of transmission
}

/**************************************************************************//**
* @fn			void SPI_ControllerTx_16bit(uint16_t data)
* @brief		Send 16-bit data to peripheral
* @note			Used for color information
*****************************************************************************/
void SPI_ControllerTx_16bit(uint16_t data)
{
	uint8_t temp = data >> 8;
	clear(LCD_PORT, LCD_TFT_CS);	//CS pulled low to start communication
	
	SPDR = temp;		//Place data to be sent on registers
	while(!(SPSR & (1<<SPIF)));	//wait for end of transmission
	SPDR = data;		//Place data to be sent on registers
	while(!(SPSR & (1<<SPIF)));	//wait for end of transmission
	
	set(LCD_PORT, LCD_TFT_CS);	//set CS to high
}

/**************************************************************************//**
* @fn			void SPI_ControllerTx_16bit_stream(uint16_t data)
* @brief		Send 16 bit data to LCD through SPI without setting CS or DC
* @note			Used for color information
*****************************************************************************/
void SPI_ControllerTx_16bit_stream(uint16_t data)
{
	uint8_t temp = data >> 8;

	SPDR = temp;		//Place data to be sent on registers
	while(!(SPSR & (1<<SPIF)));	//wait for end of transmission
	SPDR = data;		//Place data to be sent on registers
	while(!(SPSR & (1<<SPIF)));	//wait for end of transmission
}

/**************************************************************************//**
* @fn			void lcd_init(void)
* @brief		Initialize LCD settings
* @note
*****************************************************************************/
void lcd_init(void)
{
	lcd_pin_init();
	SPI_Controller_Init();
	_delay_ms(5);

	static uint8_t ST7735_cmds[]  =
	{
		ST7735_SWRESET, 0, 150,       // Software reset. This first one is needed because of the RC reset.
		ST7735_SLPOUT, 0, 255,       // Exit sleep mode
		ST7735_FRMCTR1, 3, 0x01, 0x2C, 0x2D, 0,  // Frame rate control 1
		ST7735_FRMCTR2, 3, 0x01, 0x2C, 0x2D, 0,  // Frame rate control 2
		ST7735_FRMCTR3, 6, 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D, 0,  // Frame rate control 3
		ST7735_INVCTR, 1, 0x07, 0,	// Display inversion
		ST7735_PWCTR1, 3, 0x0A, 0x02, 0x84, 5, // Power control 1
		ST7735_PWCTR2, 1, 0xC5, 5,       // Power control 2
		ST7735_PWCTR3, 2, 0x0A, 0x00, 5,	// Power control 3
		ST7735_PWCTR4, 2, 0x8A, 0x2A, 5,	// Power control 4
		ST7735_PWCTR5, 2, 0x8A, 0xEE, 5,	// Power control 5
		ST7735_VMCTR1, 1, 0x0E, 0, // Vcom control 1
		ST7735_INVOFF, 0, 0,	//Inversion off
		ST7735_MADCTL, 1, 0xC8, 0,	//Memory Access control
		ST7735_COLMOD, 1, 0x05, 0,	//Interface pixel format
		ST7735_CASET, 4, 0x00, 0x00, 0x00, 0x7F, 0,		//Column
		ST7735_RASET, 4, 0x00, 0x00, 0x00, 0x9F, 0,		//Page
		ST7735_GMCTRP1, 16, 0x02, 0x1C, 0x07, 0x12, 0x37, 0x32, 0x29, 0x2D,
		0x29, 0x25, 0x2B, 0x39, 0x00, 0x01, 0x03, 0x10, 0, // Positive Gamma
		ST7735_GMCTRN1, 16, 0x03, 0x1D, 0x07, 0x06, 0x2E, 0x2C, 0x29,0x2D,
		0x2E, 0x2E, 0x37, 0x3F, 0x00, 0x00, 0x02, 0x10, 0, // Negative Gamma
		ST7735_NORON, 0, 10,	//Normal display on
		ST7735_DISPON, 0, 100,            // Set display on
		ST7735_MADCTL, 1, MADCTL_MX | MADCTL_MV | MADCTL_RGB, 10		//Default to rotation 3
	};

	sendCommands(ST7735_cmds, 22);
}

/**************************************************************************//**
* @fn			void sendCommands (const uint8_t *cmds, uint8_t length)
* @brief		Parse and send array of commands thru SPI
* @note
*****************************************************************************/
void sendCommands (const uint8_t *cmds, uint8_t length)
{
	//Command array structure:
	//Command Code, # of data bytes, data bytes (if any), delay in ms
	uint8_t numCommands, numData, waitTime;

	numCommands = length;	// # of commands to send

	clear(LCD_PORT, LCD_TFT_CS);	//CS pulled low to start communication

	while (numCommands--)	// Send each command
	{
		clear(LCD_PORT, LCD_DC);	//D/C pulled low for command
		
		SPI_ControllerTx_stream(*cmds++);
		
		numData = *cmds++;	// # of data bytes to send

		set(LCD_PORT, LCD_DC);	//D/C set high for data
		while (numData--)	// Send each data byte...
		{
			SPI_ControllerTx_stream(*cmds++);
			
		}

		waitTime = *cmds++;     // Read post-command delay time (ms)
		if (waitTime!=0)
		{
			Delay_ms((waitTime==255 ? 500 : waitTime));
		}
	}

	set(LCD_PORT, LCD_TFT_CS);	//set CS to high
}

/**************************************************************************//**
* @fn			void LCD_setAddr(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
* @brief		Set pixel memory address to write to
* @note
*****************************************************************************/
void LCD_setAddr(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1)
{
	uint8_t ST7735_cmds[]  =
	{
		ST7735_CASET, 4, 0x00, x0, 0x00, x1, 0,		// Column
		ST7735_RASET, 4, 0x00, y0, 0x00, y1, 0,		// Page
		ST7735_RAMWR, 0, 5				// Into RAM
	};
	sendCommands(ST7735_cmds, 3);
}

/**************************************************************************//**
* @fn			void LCD_brightness(uint8_t intensity)
* @brief		Changes the intensity of the LCD screen (max = 255)
* @note
*****************************************************************************/
void LCD_brightness(uint8_t intensity)
{
	OCR0A = intensity;	//Set PWM value
}

/**************************************************************************//**
* @fn			void LCD_rotate(uint8_t r)
* @brief		Rotate display to another orientation
* @note
*****************************************************************************/
void LCD_rotate(uint8_t r)
{
	uint8_t madctl = 0;
	uint8_t rotation = r % 4; // can't be higher than 3

	switch (rotation) {
		case 0:
			madctl = MADCTL_MX | MADCTL_MY | MADCTL_RGB;
			break;
		case 1:
			madctl = MADCTL_MY | MADCTL_MV | MADCTL_RGB;
			break;
		case 2:
			madctl = MADCTL_RGB;
			break;
		case 3:
			madctl = MADCTL_MX | MADCTL_MV | MADCTL_RGB;
			break;
		default:
			madctl = MADCTL_MX | MADCTL_MY | MADCTL_RGB;
			break;
	}
	
	uint8_t ST7735_cmds[]  =
	{
		ST7735_MADCTL, 1, madctl, 0
	};
	
	sendCommands(ST7735_cmds, 1);
}