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
