#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "pigpio.h"

#include "i2clcd.h"

///////////////////////////////////////////////////////////////////////////////
// i2clcd_toggle_enable
// 

int i2clcd_toggle_enable( unsigned h, uint8_t byte )
{
    usleep( I2CLCD_E_DELAY );
    i2cWriteByte( h, ( byte | I2CLCD_ENABLE ) );
    usleep( I2CLCD_E_PULSE );
    i2cWriteByte( h, ( byte & ~I2CLCD_ENABLE ) );
    usleep( I2CLCD_E_DELAY );

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// i2clcd_write_byte
//
// Send byte to data pins
//
// b = the data
// mode = 1 for data
// 	  0 for command
//

int i2clcd_write_byte( unsigned h, uint8_t b, uint8_t mode )
{
    uint8_t buf[4];

    uint8_t nibble_high = mode | ( b & 0xF0 ) | I2CLCD_BACKLIGHT_ON;
    uint8_t nibble_low = mode | ( ( b<<4 ) & 0xF0 ) | I2CLCD_BACKLIGHT_ON;

 /*   
    // High bits
    if ( 0 != i2cWriteByte( h, nibble_high ) ) return -1;
    i2clcd_toggle_enable( h, nibble_high );

    //  Low bits
    if ( 0 != i2cWriteByte( h, nibble_low ) ) return -1 ;
    i2clcd_toggle_enable( h, nibble_low );
*/
    buf[0] = nibble_high | I2CLCD_ENABLE;
    buf[1] = nibble_high & ~I2CLCD_ENABLE;
    buf[2] = nibble_low | I2CLCD_ENABLE;
    buf[3] = nibble_low & ~I2CLCD_ENABLE;

    i2cWriteDevice( h, (char *)buf, 4 );

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// i2clcd_init
//

unsigned i2clcd_init( int bus, int addr )
{
    unsigned h;

    if ( PI_INIT_FAILED == gpioInitialise() ) {
    	return -1;
    }
    
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
// i2clcd_close
//

int i2clcd_close( unsigned h ) 
{
    int rv = i2cClose( h );
    gpioTerminate();
    return rv;
}


///////////////////////////////////////////////////////////////////////////////
// i2clcd_string
//
// Send string to display
//

int i2clcd_string( unsigned h, char *str, uint8_t line, uint8_t width )
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
    	i2clcd_write_byte( h, buf[i], I2CLCD_CHR );
    }

    return 0;
 }

