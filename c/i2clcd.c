#include <unistd.h>
#include <string.h>
#include "pigpio.h"

#include "i2clcd.h"

///////////////////////////////////////////////////////////////////////////////
//
// 

int i2clcd_toggle_enable( unsigned h, uint8_t byte )
{
    //  Toggle enable
    usleep( I2CLCD_E_DELAY );
    i2cWriteByte( h, ( byte | I2CLCD_ENABLE ) );
    usleep( I2CLCD_E_PULSE );
    i2cWriteByte( h, ( byte & ~I2CLCD_ENABLE ) );
    usleep( I2CLCD_E_DELAY );

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
// Send byte to data pins
//
// b = the data
// mode = 1 for data
// 	  0 for command


int i2clcd_write_byte( unsigned h, uint8_t b, uint8_t mode )
{
    uint8_t byte_high = mode | ( b & 0xF0 ) | I2CLCD_BACKLIGHT_ON;
    uint8_t byte_low = mode | ( ( b<<4 ) & 0xF0 ) | I2CLCD_BACKLIGHT_ON;

    // High bits
    //
    if ( 0 != i2cWriteByte( h, byte_high ) ) return -1;
    i2clcd_toggle_enable( h, byte_high );

    //  Low bits
    if ( 0 != i2cWriteByte( h, byte_low ) ) return -1 ;
    i2clcd_toggle_enable( h, byte_low );

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//
//

int i2clcd_open( int bus, int addr )
{
    unsigned h;

    if ( ( h = i2cOpen( bus, addr, 0 ) ) < 0 ) {
    	return -1;
    }

    i2clcd_write_byte( h, 0x33, I2CLCD_CMD ); // 110011 Initialise
    i2clcd_write_byte( h, 0x32, I2CLCD_CMD ); // 110010 Initialise 
    i2clcd_write_byte( h, 0x06, I2CLCD_CMD ); // 000110 Cursor move direction
    i2clcd_write_byte( h, 0x0c, I2CLCD_CMD ); // 001100 Display on, Cursor off, Blink off
    i2clcd_write_byte( h, 0x28, I2CLCD_CMD ); // 101000 Data length, number of lines, font size
    i2clcd_write_byte( h, 0x01, I2CLCD_CMD ); // 000001 Clear display

    usleep( I2CLCD_E_DELAY );

    return h;
}


///////////////////////////////////////////////////////////////////////////////
//
//

int i2clcd_close( unsigned h ) 
{
    return i2cClose( h );
}


///////////////////////////////////////////////////////////////////////////////
//
//
// Send string to display
//

int lcd_string( unsigned h, char *str, uint8_t line, uint8_t width )
{
   int i; 
   char buf[128];

    memset( buf, 0, sizeof( buf ) );
    strcpy( buf, str );		    
    for ( i=strlen( buf); i<width; i++ ) {
    	buf[i] =  0x20;
    }

    // Select line
    i2clcd_write_byte( h, line, I2CLCD_CMD );

    // Write string
    for ( i=0; i<strlen( buf ); i++ ) {
    	i2clcd_write_byte( h, buf[i], I2CLCD_CMD );
    }

    return 0;
 }
