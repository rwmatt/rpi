//#include <unistd.h>				//Needed for I2C port
//#include <fcntl.h>				//Needed for I2C port
//#include <sys/ioctl.h>			//Needed for I2C port
//#include <linux/i2c-dev.h>		//Needed for I2C port
#include <wiringPiI2C.h>
#include <stdio.h>

int main()
{
	int handle = wiringPiI2CSetup(0x4);
	for(;;) {
		int val = wiringPiI2CRead(handle);
		printf("%d\n", val);
	}
	return 0;

}
