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
    i2clcd_string( pSession, "1 - Hello World!", I2CLCD_LINE_0, 16 );    
    i2clcd_string( pSession, "2 - i2clcd", I2CLCD_LINE_1, 16 );    

    return 0;
}
