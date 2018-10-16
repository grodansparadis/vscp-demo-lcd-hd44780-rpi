#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "i2clcd.h"

int main( void )
{
    int h = i2clcd_init( 1, I2CLCD_DEVICE_ADDRESS );
    if ( -1 == h ) {
        printf("Failed to initialise pigpio library.");
	exit(-1);
    }
    i2clcd_string( h, "Hello World!", I2CLCD_LINE_0, 16 );    
    i2clcd_string( h, "i2clcd", I2CLCD_LINE_1, 16 );    

    return 0;
}
