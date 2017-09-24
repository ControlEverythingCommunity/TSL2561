// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TSL2561
// This code is designed to work with the TSL2561_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Light?sku=TSL2561_I2CS#tabs-0-product_tabset-2

#include <stdio.h>
#include <stdlib.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>

void main() 
{
	// Create I2C bus
	int file;
	char *bus = "/dev/i2c-1";
	if((file = open(bus, O_RDWR)) < 0) 
	{
		printf("Failed to open the bus. \n");
		exit(1);
	}
	// Get I2C device, TSL2561 I2C address is 0x39(57)
	ioctl(file, I2C_SLAVE, 0x39);

	// Select control register(0x00 | 0x80)
	// Power ON mode(0x03)
	char config[2] = {0};
	config[0] = 0x00 | 0x80;
	config[1] = 0x03;
	write(file, config, 2);
	// Select timing register(0x01 | 0x80)
	// Nominal integration time = 402ms(0x02)
	config[0] = 0x01 | 0x80;
	config[1] = 0x02;
	write(file, config, 2);
	sleep(1);

	// Read 4 bytes of data from register(0x0C | 0x80)
	// ch0 lsb, ch0 msb, ch1 lsb, ch1 msb
	char reg[1] = {0x0C | 0x80};
	write(file, reg, 1);
	char data[4] = {0};
	if(read(file, data, 4) != 4)
	{
		printf("Erorr : Input/output Erorr \n");
	}
	else
	{
		// Convert the data
		float ch0 = (data[1] * 256 + data[0]);
		float ch1 = (data[3] * 256 + data[2]);

		// Output data to screen
		printf("Full Spectrum(IR + Visible) : %.2f lux \n", ch0);
		printf("Infrared Value : %.2f lux \n", ch1);
		printf("Visible Value : %.2f lux \n", (ch0 - ch1));
	}
}
