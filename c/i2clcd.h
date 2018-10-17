///////////////////////////////////////////////////////////////////////////////
// i2clcd.h:
//
// This file is part of the VSCP (http://www.vscp.org) 
//
// The MIT License (MIT)
// 
// Copyright (c) 2000-2018 Ake Hedman, Grodans Paradis AB <info@grodansparadis.com>
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE./

#include <stdint.h>


// Flags for display entry mode
#define I2CLCD_ENTRYRIGHT 		0x00
#define I2CLCD_ENTRYLEFT		0x02
#define I2CLCD_ENTRYSHIFTINCREMENT 	0x01
#define I2CLCD_ENTRYSHIFTDECREMENT 	0x00

// Flags for display on/off control
#define I2CLCD_DISPLAYON 		0x04
#define I2CLCD_DISPLAYOFF 		0x00
#define I2CLCD_CURSORON 		0x02
#define I2CLCD_CURSOROFF 		0x00
#define I2CLCD_BLINKON 			0x01
#define I2CLCD_BLINKOFF 		0x00

// Flags for display/cursor shift
#define I2CLCD_DISPLAYMOVE 		0x08
#define I2CLCD_CURSORMOVE 		0x00
#define I2CLCD_MOVERIGHT 		0x04
#define I2CLCD_MOVELEFT 		0x00

// Flags for function set
#define I2CLCD_8BITMODE 		0x10
#define I2CLCD_4BITMODE 		0x00
#define I2CLCD_2LINE 			0x08
#define I2CLCD_1LINE 			0x00
#define I2CLCD_5x10DOTS 		0x04
#define I2CLCD_5x8DOTS 			0x00

// Flags for backlight control
#define I2CLCD_BACKLIGHT 		0x08
#define I2CLCD_NOBACKLIGHT 		0x00

				       

/*
 * Good docs
 *
 * http://www.dinceraydin.com/lcd/index.html
 * http://www.dinceraydin.com/lcd/commands.htm
 *
 * */



// Define some device parameters
#define I2CLCD_DEVICE_ADDRESS	0x27 	// I2C device address, if any error, change this address to 0x3f
#define I2CLCD_WIDTH 		16   	// LCD Maximum characters per line

// Define some device constants
#define I2CLCD_CHR		1 	// LCD Mode - Sending data
#define I2CLCD_CMD		0 	// LCD Mode - Sending command

#define I2CLCD_LINE_0 		0x80 	// LCD RAM address for the 1st line
#define I2CLCD_LINE_1 		0xC0 	// LCD RAM address for the 2nd line
#define I2CLCD_LINE_2		0x94 	// LCD RAM address for the 3rd line
#define I2CLCD_LINE_3		0xD4 	// LCD RAM address for the 4th line

#define I2CLCD_BACKLIGHT_ON 	0x08  	// LCD On
#define I2CLCD_BACKLIGHT_OFF	0x00  	// LCD Off

#define I2CLCD_ENABLE		0x04 	// Enable bit

//  Timing constants
#define I2CLCD_E_PULSE		500	// 500 us
#define I2CLCD_E_DELAY		500	// 500 us

// Commands
#define I2CLCD_CMD_CLEARDISPLAY		0x01	// Clear Screen
#define I2CLCD_CMD_RETURNHOME		0x02	// Home (move cursor to top/left character
#define I2CLCD_CMD_DISPLAYCONTROL	0x08	// Blank the display (without clearing)
#define I2CLCD_CMD_CURSOR_INVISIBBLE	0x0C	// Make cursor invisible
#define I2CLCD_CMD_CURSOR_UNDERLINE	0x0E	// Turn  on visible underline cursor 
#define I2CLCD_CMD_BLINK_CURSOR_BLOCK	0x0F	// Turn on visible blinking-block cursor 
#define I2CLCD_CMD_CURSORSHIFT		0x10	// Move cursor one character left
#define I2CLCD_CMD_FUNCTIONSET		0x14	// Move cursor one character right
#define I2CLCD_CMD_SCROLL_LEFT		0x18	// Scroll display one character left (all lines)
#define I2CLCD_CMD_SCROLL_RIGHT		0x1E	// Scroll display one character right (all lines)
#define I2CLCD_CMD_FUNC_4_1_5_7		0x20	// Function set (4-bit interface, 1 line, 5*7 Pixels) 
#define I2CLCD_CMD_FUNC_4_2_5_7		0x28	// Function set (4-bit interface, 2 lines, 5*7 Pixels)
#define I2CLCD_CMD_FUNC_8_1_5_7		0x30	// Function set (8-bit interface, 1 line, 5*7 Pixels)
#define I2CLCD_CMD_FUNC_8_2_5_7		0x20	// Function set (8-bit interface, 2 lines, 5*7 Pixels)
#define I2CLCD_CMD_SETCGRAMADDR		0x40
#define I2CLCD_CMD_SETDDRAMADDR		0x80


typedef struct i2clcd {
    uint8_t m_bus;
    uint8_t m_addr;
    uint8_t m_width;
    uint8_t m_bBackLight;
    unsigned m_spihandle;
    unsigned m_pi;
} i2clcd_t;


int i2clcd_write_byte( i2clcd_t *pSession, uint8_t b, uint8_t mode );
i2clcd_t *i2clcd_init( int bus, int addr, int width );
int i2clcd_close( i2clcd_t *pSession ); 
int i2clcd_string_row( i2clcd_t *pSession, char *str, uint8_t line  );
int i2clcd_string( i2clcd_t *pSession, char *buf );
void i2clcd_backlight( i2clcd_t *pSession, int bState );
int i2clcd_moveto( i2clcd_t *pSession, int row, int column );
int i2clcd_put_char( i2clcd_t *pSession, char c );



