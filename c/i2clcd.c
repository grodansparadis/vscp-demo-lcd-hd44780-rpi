#include <unistd.h>
#include <stdio.h>
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

#ifdef USE_PIGPIOD
    i2c_write_device( pSession->m_pi, 
		    	pSession->m_spihandle, 
			(char *)buf, 
			4 );  
#else   
    i2cWriteDevice( pSession->m_spihandle, (char *)buf, 4 );
#endif    

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
// i2clcd_init
//

i2clcd_t *i2clcd_init( int bus, int addr, int width  )
{
    unsigned h;
    i2clcd_t *pSession;

#ifdef USE_PIGPIOD
    int pi;

    if ( ( pi = pigpio_start( "127.0.0.1", "8888") ) < 0 ) {
	printf("xxxx\n");    
    	return NULL;
    }

    if ( ( h = i2c_open( pi, bus, addr, 0 ) ) < 0 ) {
	printf("yyyy\n");    
    	return NULL;
    } 
#else   
    if ( PI_INIT_FAILED == gpioInitialise() ) {
    	return NULL;
    }
    
    if ( ( h = i2cOpen( bus, addr, 0 ) ) < 0 ) {
    	return NULL;
    }
#endif    

    pSession = (i2clcd_t *)malloc( sizeof( struct i2clcd ) );
    if ( NULL == pSession ) return NULL;

    pSession->m_spihandle = h;
    pSession->m_bus = bus;
    pSession->m_addr = addr;
    pSession->m_width = width;
 #ifdef USE_PIGPIOD   
    pSession->m_pi = pi;
#endif   

    i2clcd_write_byte( pSession, 0x33, I2CLCD_CMD ); // 110011 Initialise
    usleep( I2CLCD_E_PULSE );
    i2clcd_write_byte( pSession, 0x32, I2CLCD_CMD ); // 110010 Initialise 
    usleep( I2CLCD_E_PULSE );
    i2clcd_write_byte( pSession, 0x06, I2CLCD_CMD ); // 000110 Cursor move direction
    usleep( I2CLCD_E_PULSE );
    i2clcd_write_byte( pSession, 0x0C, I2CLCD_CMD ); // 001100 Display on, Cursor off, Blink off
    usleep( I2CLCD_E_PULSE );
    i2clcd_write_byte( pSession, 0x28, I2CLCD_CMD ); // 101000 Data length, number of lines, font size
    usleep( I2CLCD_E_PULSE );
    i2clcd_write_byte( pSession, 0x01, I2CLCD_CMD ); // 000001 Clear display
    usleep( I2CLCD_E_PULSE );

    return pSession;
}


///////////////////////////////////////////////////////////////////////////////
// i2clcd_close
//

int i2clcd_close( i2clcd_t *pSession  ) 
{
    int rv;	
#ifdef USE_PIGPIOD
    rv = i2c_close( pSession->m_pi, pSession->m_spihandle );
    pigpio_stop( pSession->m_pi );
#else
    rv = i2cClose( pSession->m_spihandle );
    gpioTerminate();
#endif    

    free( pSession );

    return rv;
}

///////////////////////////////////////////////////////////////////////////////
// i2clcd_string_row
//
// Replace a row (0 based) of the LCD with a new string.
//

int i2clcd_string_row( i2clcd_t *pSession, char *str, uint8_t line  )
{
   int i; 
   char buf[128];

    memset( buf, 0, sizeof( buf ) );
    strcpy( buf, str );		    
    for ( i=strlen( buf); i<pSession->m_width; i++ ) {
    	buf[i] =  0x20;
    }

    switch ( line ) {

	default:    
    	case 0:
	   // Select line 0
           i2clcd_write_byte( pSession, I2CLCD_LINE_0, I2CLCD_CMD );
	   break;
	
        case 1:
           // Select line 1
           i2clcd_write_byte( pSession,I2CLCD_LINE_1, I2CLCD_CMD );
           break;

        case 2:
           // Select line 2
           i2clcd_write_byte( pSession, I2CLCD_LINE_2, I2CLCD_CMD );
           break;

	case 3:
           // Select line 3
           i2clcd_write_byte( pSession, I2CLCD_LINE_3, I2CLCD_CMD );
	   break;
    }


    // Write string
    for ( i=0; i<strlen( buf ); i++ ) {
    	i2clcd_write_byte( pSession, buf[i], I2CLCD_CHR );
    }

    return 0;
 }

///////////////////////////////////////////////////////////////////////////////
// i2clcd_string
//
// Write a string at the current cursor postion.  The cursor will
// end up at the character after the end of the string.
//

int i2clcd_string( i2clcd_t *pSession, char *buf )
{
    // Write string
    for ( int i=0; i<strlen( buf ); i++ ) {
    	i2clcd_write_byte( pSession, buf[i], I2CLCD_CHR );
    }

    return 0;    
}

///////////////////////////////////////////////////////////////////////////////
// i2clcd_backlight
//
// set backlight on/off
//

void i2clcd_backlight( i2clcd_t *pSession, int bState )
{
    if ( bState ) {
	pSession->m_bBackLight = 1;	    
    }	    
    else {
    	pSession->m_bBackLight = 0;
    }
}

///////////////////////////////////////////////////////////////////////////////
// i2clcd_moveto
//
// Move too new position
//

int i2clcd_moveto( i2clcd_t *pSession, int row, int column )
{
   switch ( row ) {

       default:	   
       case 0:
	   i2clcd_write_byte( pSession, I2CLCD_LINE_0 + column, I2CLCD_CMD );
           break;

       case 1:
           i2clcd_write_byte( pSession, I2CLCD_LINE_1 + column, I2CLCD_CMD );
           break;

       case 2:
	   i2clcd_write_byte( pSession, I2CLCD_LINE_2 + column, I2CLCD_CMD );
           break;

       case 3:
           i2clcd_write_byte( pSession, I2CLCD_LINE_3 + column, I2CLCD_CMD );
           break;	   
   }	  

   return 0; 
}


///////////////////////////////////////////////////////////////////////////////
// i2clcd_put_char
//
// Put a symbol at the current cursor positionand then increment the cursor
//

int i2clcd_put_char( i2clcd_t *pSession, char c )
{
    i2clcd_write_byte( pSession, c, I2CLCD_CHR );
    return 0;
}


