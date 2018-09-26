# Byggern2019

### Atmel microcontroller – ATmega162
#### Listening to output from the microcontroller
`picocom -b 9600 /dev/ttyS0`

*/dev/ttyS0:* DB9 port on PC.

#### Writing to the microcontroller using `printf`
Run `printf STRING > /dev/ttyS0` from source folder (Byggern2019).
*STRING:* Any string.

#### Writing to the microcontroller using `cat` (to write entire file)
Run `catf FILENAME > /dev/ttyS0` from source folder (Byggern2019).
*FILENAME:* Any file.

#### Usb multifunction board to breadboard pins
POWER+/- to 12V and 0 volts
/0-index/		/1-index/
OLED_BUS[0] to GAL[17]
OLED_BUS[1] to GAL[3]
OLED_BUS[2] to Atmega[16](¬WR)
OLED_BUS[3:11] to LATCH[9:2]

/0-index/	
FILTER[0] to PB[4] slider_L (0-index)
FILTER[1] to ADC[5]
FILTER[3] to ADC[6]
FILTER[4] to PD[1] slider_R (0-index)

JOYCON[0] to ADC[4]
JOYCON[1] to ADC[3]

####
DB9 
signal ground 5 - GND
transmit data 3 - 
receive data 2 	- 


