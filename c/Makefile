
# make file for i2c lcd lib test
# Select one of the following

#USELIB=WIRINGPI
#USELIB=PIGPIO
USELIB=PIGPIOD


ifeq ($(USELIB),PIGPIO)
FLAGS="-DUSE_PIGPIO"	
INCLIBS=-lpigpio -lpthread
else ifeq ($(USELIB),PIGPIOD)
FLAGS="-DUSE_PIGPIOD"	
INCLIBS=-lpigpiod_if2 -lpthread
else ifeq ($(USELIB),WIRINGPI)
FLAGS="-DUSE_WIRINGPI"	
INCLIBS=-lwiringPi
endif



all: testlcd

testlcd: testlcd.c i2clcd.c i2clcd.h
	gcc $(FLAGS) -Wall -o testlcd testlcd.c i2clcd.c $(INCLIBS)

clean:
	rm -f testlcd
	rm -f *.o

