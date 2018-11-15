# Byggern2019

## Node 1: Breadboard: Atmel microcontroller: ATmega162 - USB Multifunction card
### 1.1 Communication
#### 1.1.1 Listening to output from the microcontroller
`picocom -b 9600 /dev/ttyS0`

*/dev/ttyS0:* DB9 port on PC.

#### 1.1.2 Writing to the microcontroller using `printf`
Run `printf STRING > /dev/ttyS0` from source folder (Byggern2019).
*STRING:* Any string.

#### 1.1.3 Writing to the microcontroller using `cat` (to write entire file)
Run `catf FILENAME > /dev/ttyS0` from source folder (Byggern2019).
*FILENAME:* Any file.

### 1.2 Pin configurations
#### 1.2.1 Usb multifunction card to breadboard pins
##### 1.2.1.1 POWER
mulitf[0-index]	|	breadboard[1-index] | comment
----------------|---------------------|--------
**+** | 12V | NW
**-** |  0V GND | NW 
USB| | Connect to PC


##### 1.2.1.2 OLED bus
mulitf[0-index]	|	breadboard[1-index] | comment
----------------|---------------------|--------
OLED_BUS[0] | GAL[17] |
OLED_BUS[1] | GAL[3] |
OLED_BUS[2] | Atmega[16] | ¬WR (The one to the right)
OLED_BUS[3:11] | LATCH[9:2] |

##### 1.2.1.3 Sliders
mulitf[0-index] | mulitf[0-index] | breadboard[1-index] | comment
----------------|-----------------|---------------------|--------
FILTER[0] | PB[4] slider_L (0-index)| | 
FILTER[1] | | ADC[5] | 
FILTER[3] | | ADC[6] | 
FILTER[4] | PD[1] slider_R (0-index)| | 

##### 1.2.1.4 Joystick
mulitf[0-index] | breadboard[1-index] | comment
----------------|---------------------|--------
JOYCON[0] | ADC[4] | 
JOYCON[1] | ADC[3] | 
JOYCON[2] | Atmega[3] PB2| Joy_button


##### 1.2.1.5 Touch buttons 
multif[index-0]| breadboard[1-index] | comment
---------------|---------------------|--------
PB[5] | Atmega[2] PB1 | Touch_button (LEFT)¿
PD[0] | Atmega[1] PB0 | Touch_button (RIGHT)¿

#### 1.2.2 DB9 
DB9|breadboard[1-index] | comment
---|--------------------|--------
signal ground 5 | GND
transmit data 3 | MAX233[5] T1_out
receive data 2 | MAX233[4] R1_in

#### 1.2.3 Atmel ICE JTAG
AVR JTAG[1-index]| breadboard[1-index] | comment
-----------------|---------------------|--------
TCK/1|	ATmega[25]|
TMS/5|	ATmega[26]|
TDO/3|ATmega[27]|
TDI/9|ATmega[28]|
GND/2|GND|
GND/(1)0|GND|

#### 1.2.4 Breadboard to IO-board: Node1 to Node 2 communication
Breadboard[1-index]|IO-board
-------------------|--------
MCP2551[7]|CAN high
MCP2551[6]|CAN low

## Node 2: Arduino: AVR ATmega 2560 - I/O Card - Motorbox - Servo - Solenoid
### 2.1 Communication
#### 2.1.1 Listening to output from the microcontroller
`picocom -b 9600 /dev/ttyACM0`

### 2.2 Shield to IO
Shield|IO-board
------|--------
IO-bus|JP6

### 2.3 IO to mini breadboard
IO|comment
--|-------
JP12[3]|PWM-signal
JP12[2]|VCC
JP12[1]|GND 

## Game board

### 3.1 Servo bus

#### Pin placement

red|yellow|blue|gray|black
---------|--------------|----------|----------|-----------
2|4|6|8|10
1|3|5|7|9
brown|orange|green|purple|white

#### IR - emitter

IR-Emitter|Servo bus|comment
----------|---------|-------
Anode (high)(Red)|10(Black)|5V through resistance
Cathode (low)(Black)|9(White)|GND

#### IR - receiver

IR-receiver|Servo bus|further|comment
-----------|---------|-------|-------
Anode (high)(Red)|8(Gray)|Shield[A0]|signal (currently yellow)
Cathode(low)(Orange)|7(Purple)|GND|

#### Joystick motor

Servo/joystick motor|Servo bus|IO-board|comment
--------------------|---------|--------|-------
Signal (orange)|6 (blue)|JP12[3]| currently yellow
VCC (red)| 4(yellow)|JP12[2]|currently red (switched from direct connection)
GND (brown)| 2(red(ish))|JP12[1]|currently green (switched from direct connection)

#### Solenoid motor 
Important notes:

* Transistor active low!

Shooting motor|Servo bus|further|comment
--------------|---------|-------|-------
Red wire|1 (brown)|12V/purple/relay[1] |no diff between red wires
Red wire|3 (orange)|GND/gray with diode |no diff between red wires



## Motorbox

#### Motorbox connections

Motorbox 1-index|Shield|IO-board|comment
----------------|------|--------|-------
MJ1|MJ1| |commands from arduino to motor
MJ2|MJ2| |data from motor to arduino
MJEX[1]/Down| |Analog Out[1]|V-signal
MJEX[2]/Up| |Analog Out[2]|GND
NaN|I2C|JP1/I2C|SCL and SDA

#### Power supply

Motorbox|Motor|comment
--------|-----|-------
Encoder (in)|Encoder (out)|white mark matching on cable
M+|+|
M-|-|
