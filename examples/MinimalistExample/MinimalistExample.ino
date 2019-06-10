// I2C interface by default
//
#include "Wire.h"
#include "SparkFunIMU.h"
#include "SparkFunLSM303C.h"
#include "LSM303CTypes.h"

// #define DEBUG 1 in SparkFunLSM303C.h turns on debugging statements.
// Redefine to 0 to turn them off.

LSM303C myIMU;

void setup() {

  Wire.begin();//set up I2C bus, comment out if using SPI mode
  Wire.setClock(400000L);//clock stretching, comment out if using SPI mode

  Serial.begin(57600);//initialize serial monitor, maximum reliable baud for 3.3V/8Mhz ATmega328P is 57600

  if (myIMU.begin() != IMU_SUCCESS)
  {
    Serial.println("Failed setup.");
    while (1);
  }
}

void loop()
{
  //Get all parameters
  Serial.print("\nAccelerometer:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readAccelX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readAccelY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readAccelZ(), 4);

  // Not supported by hardware, so will return NAN
  Serial.print("\nGyroscope:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readGyroX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readGyroY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readGyroZ(), 4);

  Serial.print("\nMagnetometer:\n");
  Serial.print(" X = ");
  Serial.println(myIMU.readMagX(), 4);
  Serial.print(" Y = ");
  Serial.println(myIMU.readMagY(), 4);
  Serial.print(" Z = ");
  Serial.println(myIMU.readMagZ(), 4);

  Serial.print("\nThermometer:\n");
  Serial.print(" Degrees C = ");
  Serial.println(myIMU.readTempC(), 4);
  Serial.print(" Degrees F = ");
  Serial.println(myIMU.readTempF(), 4);

  delay(1000);//slow down output to make it easier to read, adjust as necessary
}
