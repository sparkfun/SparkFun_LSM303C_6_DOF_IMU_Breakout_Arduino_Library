// Test derrived class for base class SparkFunIMU
#ifndef __SPARKFUN_LSM303C_H__
#define __SPARKFUN_LSM303C_H__

#include "Wire.h"
#include "SparkFunIMU.h"
#include "LSM303CTypes.h"
#include "DebugMacros.h"

#define SENSITIVITY_ACC   0.06103515625   // LSB/mg
#define SENSITIVITY_MAG   0.00048828125   // LSB/Ga

#define DEBUG 0 // Change to 1 (nonzero) to enable debug messages

// Define a few error messages to save on space
static const char AERROR[] = "\nAccel Error";
static const char MERROR[] = "\nMag Error";

// Define SPI pins (Pro Mini)
//  D10 -> SDI/SDO
//  D11 -> SCLK
//  D12 -> CS_XL
//  D13 -> CS_MAG
#define CSPORT_MAG PORTB
#define CSBIT_MAG  5
#define CSPORT_XL  PORTB
#define CSBIT_XL   4
#define CLKPORT    PORTB
#define CLKBIT     3
#define DATAPORTI  PINB
#define DATAPORTO  PORTB
#define DATABIT    2
#define DIR_REG    DDRB
// End SPI pin definitions


class LSM303C : public SparkFunIMU
{
  public:
    // These are the only methods are the only methods the user can use w/o mods
    ~LSM303C()  =  default;
    status_t begin(void);
    // Begin contains hardware specific code (Pro Mini)
    status_t begin(InterfaceMode_t, MAG_DO_t, MAG_FS_t, MAG_BDU_t, MAG_OMXY_t,
       MAG_OMZ_t, MAG_MD_t, ACC_FS_t, ACC_BDU_t, uint8_t, ACC_ODR_t);
    float readAccelX(void);
    float readAccelY(void);
    float readAccelZ(void);
    float   readMagX(void);
    float   readMagY(void);
    float   readMagZ(void);
    float  readTempC(void);
    float  readTempF(void);

  protected:
    // Variables to store the most recently read raw data from sensor
    AxesRaw_t accelData = {NAN, NAN, NAN};
    AxesRaw_t   magData = {NAN, NAN, NAN};

    // The LSM303C functions over both I2C or SPI. This library supports both.
    // Interface mode used must be set!
    InterfaceMode_t interfaceMode = MODE_I2C;  // Set a default...

    // Hardware abstraction functions (Pro Mini)
    uint8_t  SPI_ReadByte(CHIP_t, uint8_t);
    status_t SPI_WriteByte(CHIP_t, uint8_t, uint8_t);
    uint8_t  I2C_ByteWrite(I2C_ADDR_t, uint8_t, uint8_t);  
    status_t I2C_ByteRead(I2C_ADDR_t, uint8_t, uint8_t&);

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
    status_t MAG_XYZ_AxDataAvailable(MAG_XYZDA_t&);
    float    readMag(AXIS_t);   // Reads the magnetometer data from IC

    status_t MAG_ReadReg(MAG_REG_t, uint8_t&);
    uint8_t  MAG_WriteReg(MAG_REG_t, uint8_t);
    status_t ACC_ReadReg(ACC_REG_t, uint8_t&);
    uint8_t  ACC_WriteReg(ACC_REG_t, uint8_t);
};

#endif
