all: testlcd

testlcd: testlcd.c
	gcc  -Wall -o testlcd testlcd.c i2clcd.c -lpigpio -lpthread

clean:
	rm testlcd
	rm *.o

