#include <stdio.h>
#include <stdint.h>
#include <string.h>
//#include <wiringPi.h>
#include <wiringPiSPI.h>

/*
 * 19: MOSI
 * 21: MISO
 * 23: SCLK
 * 24: CEO0 (Chip Enable 0)
 */

typedef uint8_t byte;


int main()
{
	if(wiringPiSPISetup(0, 500000) == -1) {
		puts("Error setting up WiringPI");
		return 1;
	}
	

	//char foo[] = "hello world";
	char foo[129];
	strcpy(foo, "hello worlds");
	//wiringPiSPIDataRW(0, foo, strlen(foo)+1);
	wiringPiSPIDataRW(0, foo, 128);
	puts(foo);
	return 0;

	FILE* fp = fopen("psy.raw", "rb");
	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	rewind(fp);
	byte amps[size];
	//fread(amps, size, 1, fp);
	fread(amps, 8000, 1, fp);
	fclose(fp);
	puts("File read");

	wiringPiSPIDataRW(0, amps, 8000);
	return 0;
}
