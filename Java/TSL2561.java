// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TSL2561
// This code is designed to work with the TSL2561_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Light?sku=TSL2561_I2CS#tabs-0-product_tabset-2

import com.pi4j.io.i2c.I2CBus;
import com.pi4j.io.i2c.I2CDevice;
import com.pi4j.io.i2c.I2CFactory;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.NumberFormat;

public class TSL2561
{
	public static void main(String args[]) throws Exception
	{
		// Create I2C bus
		I2CBus Bus = I2CFactory.getInstance(I2CBus.BUS_1);
		// Get I2C device, TSL2561 I2C address is 0x39(57)
		I2CDevice device = Bus.getDevice(0x39);

		// Select control register
		// Power ON mode
		device.write(0x00 | 0x80, (byte)0x03);
		// Select timing register
		// Nominal integration time = 402ms
		device.write(0x01 | 0x80, (byte)0x02);
		Thread.sleep(500);

		// Read 4 bytes of data
		// ch0 lsb, ch0 msb, ch1 lsb, ch1 msb
		byte[] data=new byte[4];
		device.read(0x0C | 0x80, data, 0, 4);

		// Convert the data
		double ch0 = ((data[1] & 0xFF)* 256 + (data[0] & 0xFF));
		double ch1 = ((data[3] & 0xFF)* 256 + (data[2] & 0xFF));

		// Output data to screen
		System.out.printf("Full Spectrum(IR + Visible) : %.2f lux %n", ch0);
		System.out.printf("Infrared Value : %.2f lux %n", ch1);
		System.out.printf("Visible Value : %.2f lux %n", (ch0 - ch1));
	}
}
	