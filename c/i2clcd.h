
// Define some device parameters
#define I2CLCD_DEVICE_ADDRESS	0x27 	// I2C device address, if any error, change this address to 0x3f
#define I2CLCD_WIDTH 		16   	// LCD Maximum characters per line

// Define some device constants
#define I2CLCD_CHR		1 	// LCD Mode - Sending data
#define I2CLCD_CMD		0 	// LCD Mode - Sending command

#define I2CLCD_LINE_1 		0x80 	// LCD RAM address for the 1st line
#define I2CLCD_LINE_2 		0xC0 	// LCD RAM address for the 2nd line
#define I2CLCD_LINE_3		0x94 	// LCD RAM address for the 3rd line
#define I2CLCD_LINE_4		0xD4 	// LCD RAM address for the 4th line

#define I2CLCD_BACKLIGHT_ON 	0x08  	// LCD On
#define I2CLCD_BACKLIGHT_OFF	0x00  	// LCD Off

#define I2CLCD_ENABLE		0x04 	// Enable bit

//  Timing constants
#define I2CLCD_E_PULSE		500	// 500 us
#define I2CLCD_E_DELAY		500	// 500 us


