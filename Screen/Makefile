name = output
device = /dev/tty.usbmodem2101
src = main.c lib/uart.c lib/DHT11.c lib/LCD_GFX.c lib/ST7735.c lib/servo.c lib/I2C.c lib/PCF8563.c
flags = -std=c11 -Os
baud = 9600

printf_min = -Wl,-u,vfprintf -lprintf_min #only int/string support
printf_float = -Wl,-u,vfprintf -lprintf_flt -lm #with float/double support

link_printf = #default
#link_printf = $(printf_min)
#link_printf = $(printf_float)

all : build program clean

build: $(src)
	avr-gcc $(flags) -DBAUD=$(baud) -DF_CPU=16000000UL -mmcu=atmega328p \
    $(link_printf) -o $(name).elf $(src)
	avr-objcopy -O ihex -R .eeprom $(name).elf $(name).ihex
	rm $(name).elf

list: $(src)
	avr-gcc -c -g -Wa,-a,-ad $(flags) -DBAUD=$(baud) -DF_CPU=16000000UL \
    -mmcu=atmega328p $(src) > $(name).lst
	rm $(name).elf

program: build
	avrdude -F -V -c arduino -p ATMEGA328P -P $(device) -b 115200 \
    -U flash:w:$(name).ihex

clean:
	rm -f $(name).ihex $(name).lst
