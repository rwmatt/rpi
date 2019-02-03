/* 
 * 2019-02-03 mcarter create
 *
 * Put random digits to ZeroSeg
 */
#include <iostream>
#include <errno.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <vector>
#include <array>
#include <ctime>

using namespace std;


typedef unsigned char uchar;

int main()
{
	srand(time(NULL));
	static const int CHANNEL = 0; // either 0 or 1

	// bus speed seems about right ...
	// Lowering it to e.g. 500'000 seems to be too slow
	// and cause display corruption
	int fd = wiringPiSPISetup(CHANNEL, 800'000); // last num is bus speed

	// the numbers 0 .. 9
	auto numerals =  array<uchar,10> {0x7e, 0x30, 0x6d, 0x79, 0x33,
		0x5b, 0x5f, 0x70, 0x7f, 0x7b};

	uchar buffer[2];


	// choose 8 numbers at random
	for(int i = 0; i<8; ++i) {
		int random = rand() % 10;
		//buf.push_back(numerals[random]);
		buffer[0] = 8-i; // position 8 is left-most segment
		buffer[1] = numerals[random];
		wiringPiSPIDataRW(CHANNEL, buffer, 2);
		cout << (int)random << " " ;
	}
	cout << "\n";


}

