#include <stdint.h>

/*
Commands

   LCD_CLEARDISPLAY = 0x01
      LCD_RETURNHOME = 0x02
         LCD_ENTRYMODESET = 0x04
	    LCD_DISPLAYCONTROL = 0x08
	       LCD_CURSORSHIFT = 0x10
	          LCD_FUNCTIONSET = 0x20
		     LCD_SETCGRAMADDR = 0x40
		        LCD_SETDDRAMADDR = 0x80

			   Flags for display entry mode

			      LCD_ENTRYRIGHT = 0x00
			         LCD_ENTRYLEFT = 0x02
				    LCD_ENTRYSHIFTINCREMENT = 0x01
				       LCD_ENTRYSHIFTDECREMENT = 0x00

Flags for display on/off control

   LCD_DISPLAYON = 0x04
      LCD_DISPLAYOFF = 0x00
         LCD_CURSORON = 0x02
	    LCD_CURSOROFF = 0x00
	       LCD_BLINKON = 0x01
	          LCD_BLINKOFF = 0x00

		     Flags for display/cursor shift

		        LCD_DISPLAYMOVE = 0x08
			   LCD_CURSORMOVE = 0x00
			      LCD_MOVERIGHT = 0x04
			         LCD_MOVELEFT = 0x00

				    Flags for function set

				       LCD_8BITMODE = 0x10
				          LCD_4BITMODE = 0x00
					     LCD_2LINE = 0x08
					        LCD_1LINE = 0x00
						   LCD_5x10DOTS = 0x04
						      LCD_5x8DOTS = 0x00

						         Flags for backlight control

							    LCD_BACKLIGHT = 0x08
							       LCD_NOBACKLIGHT = 0x00

*/				       

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

int i2clcd_toggle_enable( unsigned h, uint8_t byte );
int i2clcd_write_byte( unsigned h, uint8_t b, uint8_t mode );
unsigned i2clcd_init( int bus, int addr );
int i2clcd_close( unsigned h ); 
int i2clcd_string( unsigned h, char *str, uint8_t line, uint8_t width );


