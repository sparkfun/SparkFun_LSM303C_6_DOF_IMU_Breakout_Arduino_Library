// Test derrived class for base class SparkFunIMU
#ifndef __SPARKFUN_LSM303C_H__
#define __SPARKFUN_LSM303C_H__

#include "Wire.h"
#include "SparkFunIMU.h"
#include "LSM303CTypes.h"
#include "DebugMacros.h"

#define SENSITIVITY_ACC     -0.06103515625      //LSB/mg
#define SENSITIVITY_MAG      0.00048828125      //LSB/Ga

class LSM303C : public SparkFunIMU
{
  protected:
    // Variables to store the most recently read raw data from sensor
    AxesRaw_t accelData = {NAN, NAN, NAN};
    AxesRaw_t   magData = {NAN, NAN, NAN};

    // The LSM303C functions over both I2C or SPI. This library supports both.
    // Interface mode used must be set!
    interface_mode interfaceMode = MODE_I2C;  // Set a default...

    // Methods required to get device up and running
    status_t MAG_SetODR(MAG_DO_t);
    status_t MAG_SetFullScale(MAG_FS_t);
    status_t MAG_BlockDataUpdate(MAG_BDU_t);
    status_t MAG_XY_AxOperativeMode(MAG_OMXY_t);
    status_t MAG_Z_AxOperativeMode(MAG_OMZ_t);
    status_t MAG_SetMode(MAG_MD_t);
    status_t ACC_SetFullScale(ACC_FS_t);
    status_t ACC_BlockDataUpdate(ACC_BDU_t);
    status_t ACC_EnableAxis(uint8_t);
    status_t ACC_SetODR(ACC_ODR_t);

    status_t ACC_Status_Flags(uint8_t&);
    status_t ACC_GetAccRaw(AxesRaw_t&);
    float    readAccel(AXIS_t); // Reads the accelerometer data from IC

    status_t MAG_GetMagRaw(AxesRaw_t&);
    status_t MAG_TemperatureEN(MAG_TEMP_EN_t);    
    status_t MAG_GetTemperatureRaw(uint16_t&);
    status_t MAG_XYZ_AxDataAvailable(MAG_XYZDA_t&);
    float    readMag(AXIS_t);   // Reads the magnetometer data from IC

    status_t MAG_ReadReg(MAG_REG_t, uint8_t&);
    uint8_t  MAG_WriteReg(MAG_REG_t, uint8_t);
    status_t ACC_ReadReg(ACC_REG_t, uint8_t&);
    uint8_t  ACC_WriteReg(ACC_REG_t, uint8_t);
    uint8_t  I2C_ByteWrite(I2C_ADDR_t, uint8_t, uint8_t);  
    status_t I2C_ByteRead(I2C_ADDR_t, uint8_t, uint8_t&);

  public:
    // These are the only methods are the only methods the user can use w/o mods
    LSM303C() {}
    ~LSM303C()  =  default;
    status_t   begin(void);
    float readAccelX(void);
    float readAccelY(void);
    float readAccelZ(void);
    float   readMagX(void);
    float   readMagY(void);
    float   readMagZ(void);
    float  readTempC(void);
    float  readTempF(void);
};

#endif
