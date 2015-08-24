// I2C interface by default
//
#include "Wire.h"
#include "SparkFunIMU.h"
#include "SparkFunLSM303C.h"
#include "LSM303CTypes.h"

// #define DEBUG 1 in SparkFunLSM303C.cpp turns on debugging statements.
// Redefine to 0 to turn them off.

LSM303C myIMU;

void setup() {

  Serial.begin(250000);
  if (myIMU.begin() != IMU_SUCCESS)
  {
    Serial.println("Failed setup.");
    while(1);
  }
// TODO
uint8_t data;
for (int i = 0x0F; i <= 0x3F; i++) {
  Serial.print("Register 0x");
  Serial.print(i, HEX);
  Serial.print(" read: 0x");
  myIMU.I2C_ByteRead(ACC_I2C_ADDR, i, data);
  Serial.println(data, HEX);
} // End test

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
  
  delay(1000);
}
