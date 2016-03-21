// Distributed with a free-will license.
// Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
// TSL2561
// This code is designed to work with the TSL2561_I2CS I2C Mini Module available from ControlEverything.com.
// https://www.controleverything.com/content/Light?sku=TSL2561_I2CS#tabs-0-product_tabset-2

#include<Wire.h>

// TSL2561 I2C address is 0x39(57)
#define Addr 0x39

void setup()
{
  // Initialise I2C communication as MASTER 
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);

  // Starts I2C communication
  Wire.beginTransmission(Addr);
  // Select control register
  Wire.write(0x00 | 0x80);
  // Power ON mode
  Wire.write(0x03);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Starts I2C communication
  Wire.beginTransmission(Addr);
  // Select timing register
  Wire.write(0x01 | 0x80);
  // Nominal integration time = 402ms
  Wire.write(0x02);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
  
}

void loop()
{ 
  unsigned int data[4];
  for(int i = 0; i < 4; i++)
  {
    // Starts I2C communication
    Wire.beginTransmission(Addr);
    // Select data register
    Wire.write((140 + i));
    // Stop I2C Transmission
    Wire.endTransmission();
    
    // Request 1 byte of data
    Wire.requestFrom(Addr, 1);
    
    // Read 1 bytes of data
    if(Wire.available() == 1)
    {
      data[i] = Wire.read();
     }
     delay(200);
  }
  
  // Convert the data
  double ch0 = ((data[1] & 0xFF) * 256) + (data[0] & 0xFF);
  double ch1 = ((data[3] & 0xFF) * 256) + (data[2] & 0xFF);

  // Output data to serial monitor
  Serial.print("Full Spectrum(IR + Visible) :");
  Serial.println(ch0);
  Serial.print("Infrared Value :");
  Serial.println(ch1);
  Serial.print("Visible Value :");
  Serial.println(ch0-ch1);
}
