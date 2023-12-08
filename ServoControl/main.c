#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>

#include "uart.h"


#define SERVO_MIN 33  
#define SERVO_MAX 115  
#define MAX_DISTANCE 320
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

volatile uint8_t RandomFlag = 0;
volatile uint8_t random_number;
volatile uint8_t TurnFlag = 0;
void setupTimer0();

ISR(INT1_vect)
{
	_delay_ms(10);
	if(PIND & (1<<PIND3))
	{
		_delay_ms(10);
		if(PIND & (1<<PIND3))
		{ 
      if(RandomFlag == 0 && TurnFlag == 0) TurnFlag = !TurnFlag;
      else if(RandomFlag == 1 && TurnFlag == 0)
      {
        TurnFlag = !TurnFlag;
        RandomFlag = !RandomFlag;
      }
      else if(RandomFlag == 0 && TurnFlag == 1)
      {
        TurnFlag = !TurnFlag;
      }
        _delay_ms(500);

		}
	}
}

ISR(INT0_vect)
{
	_delay_ms(10);
	if(PIND & (1<<PIND2))
	{
		_delay_ms(10);
		if(PIND & (1<<PIND2))
		{
      if(RandomFlag == 0 && TurnFlag == 0) RandomFlag = !RandomFlag;
      else if(RandomFlag == 0 && TurnFlag == 1)
      {
        TurnFlag = !TurnFlag;
        RandomFlag = !RandomFlag;
      }
      else if(RandomFlag == 1 && TurnFlag == 0)
      {
        RandomFlag = !RandomFlag;
      }
			_delay_ms(500);
		}
	}
}

void initSwitch()
{
	DDRD &= ~(1<<PIND2);
	EICRA |= (1<<ISC00);
	EIMSK |= (1<<INT0);
	sei();
}

void initServo() {
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	ICR1 = 39999;
	DDRB |= (1 << PINB1);
	DDRB |= (1 << PINB2);
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void setupTimer0() {
	//  Fast PWM Mode
	TCCR0A |= (1 << WGM01) | (1 << WGM00);
	TCCR0A |= (1 << COM0A1);

	// Presclar 256
	TCCR0B |= (1 << CS02);

	DDRD |= (1 << DDD6); //OCR0A
}


void setupButton()
{
  cli();
	DDRD &= ~(1<<PIND3);
	EICRA |= (1<<ISC11);
	EIMSK |= (1<<INT1);
  sei(); 
}


void setServoAngle(uint8_t angle) {
	uint8_t value = SERVO_MIN + (angle * (SERVO_MAX - SERVO_MIN) / 180);
	OCR0A = value;
}

int GetDirection(char* str)
{

  if(str[0] == '0')
  {
    return 0;
  }
  else if(str[0] == '1')
    {
      return 1;
	}
    else if(str[0] == '2')
    {
      return 2;
    }
}	

void RandomMove()
{
  		random_number = rand() % 3;
			if(random_number == 0)
			{
				//mode 1 Synchronized hand wave
				OCR1B = map(60,0,180,950,4900);
				OCR1A = map(65,0,180,950,4900);
				_delay_ms(500);
				
				OCR1B = map(80,0,180,950,4900);
				OCR1A = map(45,0,180,950,4900);
				_delay_ms(500);
				
				OCR1B = map(40,0,180,950,4900);
				OCR1A = map(85,0,180,950,4900);
				_delay_ms(500);
				
				OCR1B = map(80,0,180,950,4900);
				OCR1A = map(45,0,180,950,4900);
				_delay_ms(500);
				
				OCR1B = map(40,0,180,950,4900);
				OCR1A = map(85,0,180,950,4900);
				_delay_ms(500);
				
				OCR1B = map(60,0,180,950,4900);
				OCR1A = map(65,0,180,950,4900);
				_delay_ms(500);
				
				_delay_ms(3000);
			}
			else if(random_number == 1)
			{
				//mode 2 Asynchronous wave
				
				OCR1B = map(120,0,180,950,4900);
				OCR1A = map(125,0,180,950,4900);
				_delay_ms(500);
				
				OCR1B = map(60,0,180,950,4900);
				OCR1A = map(65,0,180,950,4900);
				_delay_ms(500);
				
				OCR1B = map(0,0,180,950,4900);
				OCR1A = map(5,0,180,950,4900);
				_delay_ms(500);
				
				OCR1B = map(60,0,180,950,4900);
				OCR1A = map(65,0,180,950,4900);
				_delay_ms(500);
				
				OCR1B = map(120,0,180,950,4900);
				OCR1A = map(125,0,180,950,4900);
				_delay_ms(500);
				
				OCR1B = map(60,0,180,950,4900);
				OCR1A = map(65,0,180,950,4900);
				_delay_ms(500);
				
				_delay_ms(3000);
			}
			
			else if(random_number == 2)
			{
				//mode 3 Shake Hand
				
				OCR1A = map(60,0,180,950,4900);
				OCR1B = map(170,0,180,950,4900);
				_delay_ms(500);
				
				OCR1A = map(60,0,180,950,4900);
				OCR1B = map(180,0,180,950,4900);
				_delay_ms(500);
				
				OCR1A = map(60,0,180,950,4900);
				OCR1B = map(160,0,180,950,4900);
				_delay_ms(500);
				
				OCR1A = map(60,0,180,950,4900);
				OCR1B = map(180,0,180,950,4900);
				_delay_ms(500);
				
				OCR1A = map(60,0,180,950,4900);
				OCR1B = map(160,0,180,950,4900);
				_delay_ms(500);
				
				OCR1A = map(60,0,180,950,4900);
				OCR1B = map(65,0,180,950,4900);
				_delay_ms(500);
				
				_delay_ms(3000);
			}
}
int BigServoMove(int direction, int angle)
{	

	if(direction == 0)
	{
		angle = angle;
		setServoAngle(angle);
		return angle;
	}
	else if(direction == 1)
	{
		angle = angle-5;
		if( angle<50 || angle >140) return 50;
		setServoAngle(angle);
		return angle-5;
	}
	else if(direction == 2)
	{
		angle = angle+5;
	if( angle<50 || angle >140) return 140;
		setServoAngle(angle);
		return angle+5;
	}
	return angle;
}
int FaceMove(int angle)
{

			// receive data from python
			char buf[20];
			UART_receive(buf, 20);
			if(!buf) return angle;
			//char buffer[20];
			//sprintf(buffer, "%d", angle); 
			//UART_putstring(buffer);
			angle = BigServoMove(GetDirection(buf), angle);
			return angle;
}

void ResetFaceMove(int angle)
{
			angle = 95;
			setServoAngle(angle);
}

int main(void) {
	initServo();//range 950-4900
	initSwitch();
	int angle = 95;
  	setupButton();
  	setupTimer0();
  	UART_init(BAUD_PRESCALER);
  	setServoAngle(angle);
	
	while (1) {
		if(RandomFlag==1)
		{
      if(TurnFlag == 0)
			{
        ResetFaceMove(95);
        RandomMove();
			}
		}
		else if(RandomFlag == 0)
		{
			if(TurnFlag == 1)
			{
        angle = FaceMove(angle);
			}
      else if(TurnFlag == 0)
      {
        ResetFaceMove(95);
      }
		}
	}
}
