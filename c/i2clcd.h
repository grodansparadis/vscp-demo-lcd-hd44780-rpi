#include <stdint.h>

/*
// Flags for display entry mode
I2CLCD_ENTRYRIGHT = 0x00
LCD_ENTRYLEFT = 0x02
LCD_ENTRYSHIFTINCREMENT = 0x01
LCD_ENTRYSHIFTDECREMENT = 0x00

// Flags for display on/off control
LCD_DISPLAYON = 0x04
LCD_DISPLAYOFF = 0x00
LCD_CURSORON = 0x02
LCD_CURSOROFF = 0x00
LCD_BLINKON = 0x01
LCD_BLINKOFF = 0x00

// Flags for display/cursor shift
LCD_DISPLAYMOVE = 0x08
LCD_CURSORMOVE = 0x00
LCD_MOVERIGHT = 0x04
LCD_MOVELEFT = 0x00

// Flags for function set
LCD_8BITMODE = 0x10
LCD_4BITMODE = 0x00
LCD_2LINE = 0x08
LCD_1LINE = 0x00
LCD_5x10DOTS = 0x04
LCD_5x8DOTS = 0x00

// Flags for backlight control
LCD_BACKLIGHT = 0x08
LCD_NOBACKLIGHT = 0x00

*/				       

/*
 *  http://www.dinceraydin.com/lcd/index.html
 *  http://www.dinceraydin.com/lcd/commands.htm
 *
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
#define I2CLCD_CMD_CLEARDISPLAY		0x01
#define I2CLCD_CMD_RETURNHOME		0x02
#define I2CLCD_CMD_ENTRYMODESET		0x04
#define I2CLCD_CMD_DISPLAYCONTROL	0x08
#define I2CLCD_CMD_CURSORSHIFT		0x10
#define I2CLCD_CMD_FUNCTIONSET		0x20
#define I2CLCD_CMD_SETCGRAMADDR		0x40
#define I2CLCD_CMD_SETDDRAMADDR		0x80


typedef struct i2clcd {
    uint8_t m_bus;
    uint8_t m_addr;
    uint8_t m_width;
    uint8_t m_bBackLight;
    unsigned m_spihandle;
    unsigned m_pihandle;
} i2clcd_t;


int i2clcd_write_byte( i2clcd_t *pSession, uint8_t b, uint8_t mode );
i2clcd_t *i2clcd_init( int bus, int addr, int width );
int i2clcd_close( i2clcd_t *pSession ); 
int i2clcd_string_row( i2clcd_t *pSession, char *str, uint8_t line  );
int i2clcd_string( i2clcd_t *pSession, char *buf );
void i2clcd_backlight( i2clcd_t *pSession, int bState );
int i2clcd_moveto( i2clcd_t *pSession, int row, int column );
int i2clcd_put_char( i2clcd_t *pSession, char c );



