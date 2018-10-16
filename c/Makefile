all: testlcd

testlcd: testlcd.c i2clcd.c i2clcd.h
	gcc  -Wall -o testlcd testlcd.c i2clcd.c -lpigpio -lpthread

clean:
	rm -f testlcd
	rm -f *.o

