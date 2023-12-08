/*
 * LCD_GFX.c
 *
 * Created: 9/20/2021 6:54:25 PM
 *  Author: You
 */

#ifndef __AVR_ATmega328P__
#define __AVR_ATmega328P__
#endif

#include "LCD_GFX.h"
#include "ST7735.h"
#include <stdlib.h>


uint16_t rgb565(uint8_t red, uint8_t green, uint8_t blue)
{
	return ((((31 * (red + 4)) / 255) << 11) | (((63 * (green + 2)) / 255) << 5) | ((31 * (blue + 4)) / 255));
}


void LCD_drawPixel(uint8_t x, uint8_t y, uint16_t color)
{
	LCD_setAddr(x, y, x, y);
	SPI_ControllerTx_16bit(color);
}


void LCD_drawChar(uint8_t x, uint8_t y, uint16_t character, uint16_t fColor, uint16_t bColor, uint8_t scale)
{
	uint16_t row = character - 0x20; // Determine row of ASCII table starting at space
	int i, j, k, l;

	// Check scaled boundaries
	if ((LCD_WIDTH - x > 7 * scale) && (LCD_HEIGHT - y > 7 * scale))
	{
		for (i = 0; i < 5; i++)
		{
			uint8_t pixels = ASCII[row][i]; // Go through the list of pixels

			// Calculate the address area for the whole line of the character
			LCD_setAddr(x + (i * scale), y, x + (i * scale) + scale - 1, y + (8 * scale) - 1);

			for (j = 0; j < 8; j++)
			{
				if ((pixels >> j) & 1)
				{
					for (k = 0; k < scale; k++)
					{ // Scale pixel horizontally
						for (l = 0; l < scale; l++)
						{ // Scale pixel vertically
							// Write pixel color data to SPI
							SPI_ControllerTx_16bit(fColor);
						}
					}
				}
				else
				{
					for (k = 0; k < scale; k++)
					{ // Scale background horizontally
						for (l = 0; l < scale; l++)
						{ // Scale background vertically
							// Write background color data to SPI
							SPI_ControllerTx_16bit(bColor);
						}
					}
				}
			}
		}
	}
}



void LCD_drawCircle(uint8_t x0, uint8_t y0, uint8_t radius, uint16_t color)
{

	// when radius is 0, draw a point
	if (radius == 0)
	{
		LCD_drawPixel(x0, y0, color);
		return;
	}

	// set address
	LCD_setAddr(x0 - radius, y0 - radius, x0 + radius, y0 + radius);

	clear(LCD_PORT, LCD_TFT_CS);

	for (int x = x0 - radius; x <= x0 + radius; x++)
	{
		for (int y = y0 - radius; y <= y0 + radius; y++)
		{
			if ((x - x0) * (x - x0) + (y - y0) * (y - y0) <= radius * radius)
			{
				SPI_ControllerTx_16bit(color);
			}
			else
			{
				SPI_ControllerTx_16bit(BLACK);
			}
		}
	}

	set(LCD_PORT, LCD_TFT_CS);
}


void LCD_drawLine(short x0, short y0, short x1, short y1, uint16_t c)
{
	// Use Bresenham's line algorithm to draw a line

	// initailize the current point
	int x = x0;
	int y = y0;

	// inititial the dicision variable p
	int p = 2 * (y1 - y0) - (x1 - x0);

	// draw the line
	while (x <= x1)
	{
		LCD_drawPixel(x, y, c);
		x++;
		if (p < 0)
		{
			p = p + 2 * (y1 - y0);
		}
		else
		{
			y++;
			p = p + 2 * (y1 - y0) - 2 * (x1 - x0);
		}
	}
}


void LCD_drawBlock(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint16_t color)
{

	LCD_setAddr(x0, y0, x1, y1);

	clear(LCD_PORT, LCD_TFT_CS); // CS pulled low to start communication
	for (uint32_t i = 0; i <= ((x1 - x0 + 1) * (y1 - y0 + 1)); i++)
	{
		SPI_ControllerTx_16bit_stream(color);
	}
	set(LCD_PORT, LCD_TFT_CS); // set CS to high
}


void LCD_setScreen(uint16_t color)
{

	LCD_setAddr(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);

	clear(LCD_PORT, LCD_TFT_CS); // CS pulled low to start communication

	for (uint32_t i = 0; i < (LCD_WIDTH * LCD_HEIGHT); i++)
	{
		SPI_ControllerTx_16bit_stream(color);
	}

	set(LCD_PORT, LCD_TFT_CS); // set CS to high
}


void LCD_drawString(uint8_t x, uint8_t y, char *str, uint16_t fg, uint16_t bg, uint8_t scale)
{
	uint8_t currX = x;
	char *ptr = str;

	while (*ptr != '\0')
	{
		LCD_drawChar(currX, y, *ptr, fg, bg, scale);
		currX += 6 * scale;
		ptr++;
	}
}

void normalEyes(uint16_t x, uint16_t y, uint16_t color)
{
	// left eye
	LCD_drawCircle(x, y, 10, color);
	LCD_drawCircle(x, y + 34, 10, color);
	LCD_drawBlock(x - 10, y, x + 10, y + 34, color);

	// right eye
	LCD_drawCircle(x + 60, y, 10, color);
	LCD_drawCircle(x + 60, y + 34, 10, color);
	LCD_drawBlock(x + 50, y, x + 70, y + 34, color);
}


void angryEyes(uint16_t color)
{
	LCD_drawChar(25, 35, '>', color, BLACK, 8);
	LCD_drawChar(95, 35, '<', color, BLACK, 8);
}

void happyEyes(uint16_t color)
{
	LCD_drawChar(25, 45, '^', color, BLACK, 9);
	LCD_drawChar(95, 45, '^', color, BLACK, 9);
}


void eyesMoving(uint16_t move)
{
	uint16_t originalX = 50;
	uint16_t originalY = 45;
	uint16_t posX = originalX;
	uint16_t posY = originalY;
	for (int i = 0; i < 3; i++)
	{
		normalEyes(originalX, originalY, WHITE);
		Delay_ms(2000);
		normalEyes(originalX, originalY, BLACK);
		for (int i = 0; i < 2; i++)
		{
			// generate random number to determine 
			// the direction of the movement
			int random = rand() % 4;
			switch (random)
			{
			case 0:
				posX = originalX + move;
				break;
			case 1:
				posX = originalX - move;
				break;
			case 2:
				posY = originalY + move;
				break;
			case 3:
				posY = originalY - move;
				break;
			}
			normalEyes(posX, posY, WHITE);
			Delay_ms(500);
			normalEyes(posX, posY, BLACK);
		}
	}
}

void angryEyesMoving()
{

	for (int i = 0; i < 2; i++)
	{

		normalEyes(50, 45, WHITE);

		Delay_ms(500);

		normalEyes(50, 45, BLACK);

		angryEyes(WHITE);

		Delay_ms(1000);

		angryEyes(BLACK);


	}
}

void happyEyesMoving()
{

	for (int i = 0; i < 2; i++)
	{

		normalEyes(50, 45, WHITE);

		Delay_ms(500);

		normalEyes(50, 45, BLACK);

		happyEyes(WHITE);

		Delay_ms(1000);

		happyEyes(BLACK);

	}
}