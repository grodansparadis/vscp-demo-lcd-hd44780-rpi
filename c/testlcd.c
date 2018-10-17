///////////////////////////////////////////////////////////////////////////////
// testlcd.c:
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

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "i2clcd.h"

int main( void )
{
    i2clcd_t *pSession = i2clcd_init( 1, I2CLCD_DEVICE_ADDRESS, 16 );
    if ( NULL == pSession ) {
        printf("Failed to initialise pigpio library.");
	exit(-1);
    }

    // Clear display
    i2clcd_write_byte( pSession, I2CLCD_CMD_CLEARDISPLAY, I2CLCD_CMD );
    sleep( 1 );

    // Write text
    i2clcd_string_row( pSession, "1 - Hello World!", 0 );
    sleep( 1 );
    i2clcd_string_row( pSession, "2 - i2clcd", 1 );    
    sleep( 1 );

    // Return home
    i2clcd_write_byte( pSession, I2CLCD_CMD_RETURNHOME, I2CLCD_CMD );
    sleep( 1 );
    // Write character
    i2clcd_put_char( pSession, 'A' );
    sleep( 1 );
    // Move to new mpsition
    i2clcd_moveto( pSession, 1, 0 );
    // Write character
    i2clcd_put_char( pSession, 'B' );
    sleep( 1 );

    // Light block cursor
    i2clcd_write_byte( pSession, 0x0F, I2CLCD_CMD );

    i2clcd_moveto( pSession, 1, 11 );
    sleep( 1 );
    i2clcd_put_char( pSession, '1' );
    sleep( 1 );
    i2clcd_put_char( pSession, '2' );
    sleep( 1 );
    i2clcd_put_char( pSession, '3' );
    sleep( 1 );
    i2clcd_put_char( pSession, '4' );
    sleep( 1 );

    // Move to new mpsition
    i2clcd_moveto( pSession, 0, 4 );

    // Light block cursor
    i2clcd_write_byte( pSession, 0x0F, I2CLCD_CMD );
    sleep( 4 );

    // Light underline cursor
    i2clcd_write_byte( pSession, 0x0E, I2CLCD_CMD );
    sleep( 1 );

    // Move cursor right
    i2clcd_write_byte( pSession, 0x14, I2CLCD_CMD );
    sleep( 1 );
    i2clcd_write_byte( pSession, 0x14, I2CLCD_CMD );
    sleep( 1 );
    i2clcd_write_byte( pSession, 0x14, I2CLCD_CMD );
    sleep( 1 );
    i2clcd_write_byte( pSession, 0x14, I2CLCD_CMD );
    sleep( 1 );

    // Make cursor invisible
    i2clcd_write_byte( pSession, 0x0C, I2CLCD_CMD );

    for ( int i=0; i<16; i++ ) {
       i2clcd_write_byte( pSession, 0x18, I2CLCD_CMD );
       usleep( 500000 );
    }

    for ( int i=0; i<16; i++ ) {
       i2clcd_write_byte( pSession, 0x1E, I2CLCD_CMD );
       usleep( 500000 );
    }

    // Blank display
    i2clcd_write_byte( pSession, 0x08, I2CLCD_CMD );
    sleep( 3 );

    // Resore display
    i2clcd_write_byte( pSession, 0x0C, I2CLCD_CMD );
    sleep( 1 );

    return 0;
}
