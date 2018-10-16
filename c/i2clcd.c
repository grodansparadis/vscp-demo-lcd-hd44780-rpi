#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#ifdef USE_PIGPIOD
#include <pigpiod_if2.h>
#else
#include <pigpio.h>
#endif

#include "i2clcd.h"


///////////////////////////////////////////////////////////////////////////////
// i2clcd_write_byte
//
// Send byte to data pins
//
// b = the data
// mode = 1 for data
// 	  0 for command
//

int i2clcd_write_byte( i2clcd_t *pSession, uint8_t b, uint8_t mode )
{
    uint8_t buf[4];

    uint8_t nibble_high = mode | ( b & 0xF0 ) | I2CLCD_BACKLIGHT_ON;
    uint8_t nibble_low = mode | ( ( b<<4 ) & 0xF0 ) | I2CLCD_BACKLIGHT_ON;

    buf[0] = nibble_high | I2CLCD_ENABLE;
    buf[1] = nibble_high & ~I2CLCD_ENABLE;
    buf[2] = nibble_low | I2CLCD_ENABLE;
    buf[3] = nibble_low & ~I2CLCD_ENABLE;

    i2cWriteDevice( pSession->m_spihandle, (char *)buf, 4 );

    return 0;
}
/*
struct i2clcd {
    uint8_t m_bus;
    uint8_t m_addr;
    uint8_t m_width;
    uint8_t m_bBackLight;
    unsigned spihandle;
    unsigned pihandle;
} i2clcd_t;
*/
///////////////////////////////////////////////////////////////////////////////
// i2clcd_init
//

i2clcd_t *i2clcd_init( int bus, int addr, int width  )
{
    unsigned h;
    i2clcd_t *pSession;

    if ( PI_INIT_FAILED == gpioInitialise() ) {
    	return NULL;
    }
    
    if ( ( h = i2cOpen( bus, addr, 0 ) ) < 0 ) {
    	return NULL;
    }

    pSession = (i2clcd_t *)malloc( sizeof( struct i2clcd ) );
    if ( NULL == pSession ) return NULL;

    pSession->m_spihandle = h;
    pSession->m_bus = bus;
    pSession->m_addr = addr;
    pSession->m_width = width;

    i2clcd_write_byte( pSession, 0x33, I2CLCD_CMD ); // 110011 Initialise
    i2clcd_write_byte( pSession, 0x32, I2CLCD_CMD ); // 110010 Initialise 
    i2clcd_write_byte( pSession, 0x06, I2CLCD_CMD ); // 000110 Cursor move direction
    i2clcd_write_byte( pSession, 0x0c, I2CLCD_CMD ); // 001100 Display on, Cursor off, Blink off
    i2clcd_write_byte( pSession, 0x28, I2CLCD_CMD ); // 101000 Data length, number of lines, font size
    i2clcd_write_byte( pSession, 0x01, I2CLCD_CMD ); // 000001 Clear display

    usleep( I2CLCD_E_PULSE );

    return pSession;
}


///////////////////////////////////////////////////////////////////////////////
// i2clcd_close
//

int i2clcd_close( i2clcd_t *pSession  ) 
{
    int rv = i2cClose( pSession->m_spihandle );
    gpioTerminate();

    free( pSession );

    return rv;
}

///////////////////////////////////////////////////////////////////////////////
// i2clcd_string
//
// Send string to display
//

int i2clcd_string( i2clcd_t *pSession, char *str, uint8_t line, uint8_t width )
{
   int i; 
   char buf[128];

    memset( buf, 0, sizeof( buf ) );
    strcpy( buf, str );		    
    for ( i=strlen( buf); i<width; i++ ) {
    	buf[i] =  0x20;
    }

    // Select line
    i2clcd_write_byte( pSession, line, I2CLCD_CMD );

    // Write string
    for ( i=0; i<strlen( buf ); i++ ) {
    	i2clcd_write_byte( pSession, buf[i], I2CLCD_CHR );
    }

    return 0;
 }

