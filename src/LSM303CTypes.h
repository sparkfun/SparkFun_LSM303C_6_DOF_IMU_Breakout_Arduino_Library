// Data types used by the SparkFun LSM303C driver
#ifndef __LSM303C_TYPES_H__
#define __LSM303C_TYPES_H__

#define   ACC_I2C_ADDR    0x1D
#define   MAG_I2C_ADDR    0x1E

////////////////////////////////////
// LSM303C Magnetometer Registers //
////////////////////////////////////
#define   MAG_WHO_AM_I    0x0F
#define   MAG_CTRL_REG1   0x20
#define   MAG_CTRL_REG2   0x21
#define   MAG_CTRL_REG3   0x22
#define   MAG_CTRL_REG4   0x23
#define   MAG_CTRL_REG5   0x24
#define   MAG_STATUS_REG  0x27
#define   MAG_OUTX_L      0x28
#define   MAG_OUTX_H      0x29
#define   MAG_OUTY_L      0x2A
#define   MAG_OUTY_H      0x2B
#define   MAG_OUTZ_L      0x2C
#define   MAG_OUTZ_H      0x2D
#define   MAG_TEMP_OUT_L  0x2E
#define   MAG_TEMP_OUT_H  0x2F
#define   MAG_INT_CFG     0x30
#define   MAG_INT_SRC     0x31
#define   MAG_INT_THS_L   0x32
#define   MAG_INT_THS_H   0x33

/////////////////////////////////////
// LSM303C Accelerometer Registers //
/////////////////////////////////////
#define   ACC_TEMP_L        0x0B
#define   ACC_TEMP_H        0x0C
#define   ACC_ACT_TSH       0x1E
#define   ACC_ACT_DUR       0x1F
#define   ACC_WHO_AM_I      0x0F
#define   ACC_CTRL1         0x20
#define   ACC_CTRL2         0x21
#define   ACC_CTRL3         0x22
#define   ACC_CTRL4         0x23
#define   ACC_CTRL5         0x24
#define   ACC_CTRL6         0x25
#define   ACC_CTRL7         0x26
#define   ACC_STATUS        0x27
#define   ACC_OUT_X_L       0x28
#define   ACC_OUT_X_H       0x29
#define   ACC_OUT_Y_L       0x2A
#define   ACC_OUT_Y_H       0x2B
#define   ACC_OUT_Z_L       0x2C
#define   ACC_OUT_Z_H       0x2D
#define   ACC_FIFO_CTRL     0x2E
#define   ACC_FIFO_SRC      0x2F
#define   ACC_IG_CFG1       0x30
#define   ACC_IG_SRC1       0x31
#define   ACC_IG_THS_X1     0x32
#define   ACC_IG_THS_Y1     0x33
#define   ACC_IG_THS_Z1     0x34
#define   ACC_IG_DUR1       0x35
#define   ACC_IG_CFG2       0x36
#define   ACC_IG_SRC2       0x37
#define   ACC_IG_THS2       0x38
#define   ACC_IG_DUR2       0x39
#define   ACC_XL_REFERENCE  0x3A
#define   ACC_XH_REFERENCE  0x3B
#define   ACC_YL_REFERENCE  0x3C
#define   ACC_YH_REFERENCE  0x3D
#define   ACC_ZL_REFERENCE  0x3E
#define   ACC_ZH_REFERENCE  0x3F

typedef enum
{
  xAxis,
  yAxis,
  zAxis
} AXIS_t;

typedef enum
{ 
  MAG_DO_0_625_Hz = 0x00,
  MAG_DO_1_25_Hz  = 0x04,
  MAG_DO_2_5_Hz   = 0x08,
  MAG_DO_5_Hz     = 0x0C,
  MAG_DO_10_Hz    = 0x10,
  MAG_DO_20_hZ    = 0x14,
  MAG_DO_40_Hz    = 0x18,
  MAG_DO_80_Hz    = 0x1C
} MAG_DO_t;

typedef enum
{ 
  MAG_FS_4_Ga   =  0x00,
  MAG_FS_8_Ga   =  0x20,
  MAG_FS_12_Ga  =  0x40,
  MAG_FS_16_Ga  =  0x60
} MAG_FS_t;

typedef enum
{ 
  MAG_BDU_DISABLE = 0x00,
  MAG_BDU_ENABLE  = 0x40
} MAG_BDU_t;

typedef enum
{ 
  MAG_OMXY_LOW_POWER              = 0x00,
  MAG_OMXY_MEDIUM_PERFORMANCE     = 0x20,
  MAG_OMXY_HIGH_PERFORMANCE       = 0x40,
  MAG_OMXY_ULTRA_HIGH_PERFORMANCE = 0x60
} MAG_OMXY_t;

typedef enum
{ 
  MAG_OMZ_LOW_PW                  =  0x00,
  MAG_OMZ_MEDIUM_PERFORMANCE      =  0x04,
  MAG_OMZ_HIGH_PERFORMANCE        =  0x08,
  MAG_OMZ_ULTRA_HIGH_PERFORMANCE  =  0x0C
} MAG_OMZ_t;

typedef enum
{ 
  MAG_MD_CONTINUOUS   = 0x00,
  MAG_MD_SINGLE       = 0x01,
  MAG_MD_POWER_DOWN_1 = 0x02,
  MAG_MD_POWER_DOWN_2 = 0x03
} MAG_MD_t;

typedef enum
{ 
  ACC_FS_2g = 0x00,
  ACC_FS_4g = 0x20,
  ACC_FS_8g = 0x30
} ACC_FS_t;

typedef enum
{ 
  ACC_BDU_DISABLE = 0x00,
  ACC_BDU_ENABLE  = 0x08
} ACC_BDU_t;

typedef enum
{ 
  ACC_ODR_POWER_DOWN  = 0x00,
  ACC_ODR_10_Hz       = 0x10,
  ACC_ODR_50_Hz       = 0x20,
  ACC_ODR_100_Hz      = 0x30,
  ACC_ODR_200_Hz      = 0x40,
  ACC_ODR_400_Hz      = 0x50,
  ACC_ODR_800_Hz      = 0x60,
  ACC_ODR_MASK        = 0x60
} ACC_ODR_t;

typedef enum
{ 
  ACC_DISABLE_ALL = 0x00,
  ACC_X_ENABLE    = 0x01,
  ACC_Y_ENABLE    = 0x02,
  ACC_Z_ENABLE    = 0x04
} ACC_AXIS_EN_t;

typedef enum
{ 
  ACC_X_NEW_DATA_AVAILABLE    = 0x01,
  ACC_Y_NEW_DATA_AVAILABLE    = 0x02,
  ACC_Z_NEW_DATA_AVAILABLE    = 0x04,
  ACC_ZYX_NEW_DATA_AVAILABLE  = 0x08,
  ACC_X_OVERRUN               = 0x10,
  ACC_Y_OVERRUN               = 0x20,
  ACC_Z_OVERRUN               = 0x40,
  ACC_ZYX_OVERRUN             = 0x80
} ACC_STATUS_FLAGS_t;


#endif
