// Test derrived class for base class SparkFunIMU
#ifndef __SPARKFUN_LSM303C_H__
#define __SPARKFUN_LSM303C_H__

#include "Wire.h"
#include "SparkFunIMU.h"
#include "LSM303CTypes.h"
#include "DebugMacros.h"
//#include "stdint.h"

#define SENSITIVITY_ACC     -0.06103515625      //LSB/mg
#define SENSITIVITY_MAG      0.00048828125      //LSB/Ga

class LSM303C : public SparkFunIMU
{
  protected:
    typedef struct
    {
      int16_t xAxis;
      int16_t yAxis;
      int16_t zAxis;
    } AxesRaw_t;

    AxesRaw_t accelData = {NAN, NAN, NAN};

    // The LSM303C functions over both I2C or SPI. This library supports both.
    // Interface mode used must be set!
    typedef enum
    {
      MODE_SPI,
      MODE_I2C,
    } interface_mode;
    interface_mode interfaceMode = MODE_I2C;  // Set a default...

    // These two funcs are only for demo
    uint8_t readAccelWhoAmI() { return 0x41; }
    uint8_t readMagWhoAmI()   { return 0x3E; }

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
    float    readAccel(AXIS_t);

    status_t MAG_ReadReg(uint8_t, uint8_t&);
    uint8_t  MAG_WriteReg(uint8_t, uint8_t);
    status_t ACC_ReadReg(uint8_t, uint8_t&);
    uint8_t  ACC_WriteReg(uint8_t, uint8_t);
    uint8_t  I2C_ByteWrite(uint8_t, uint8_t, uint8_t);  
    status_t I2C_ByteRead(uint8_t, uint8_t, uint8_t&);

  public:
    LSM303C() {}
    ~LSM303C() = default;
    status_t begin(void);
    float readAccelX(void);
    float readAccelY(void);
    float readAccelZ(void);
    float readMagX(void);
    float readMagY(void);
    float readMagZ(void);
    float readTempC(void);
    float readTempF(void);
};

//extern LSM303C lsm303c;

#endif  // End of __EXAMPLEIMU_H__ definition check
