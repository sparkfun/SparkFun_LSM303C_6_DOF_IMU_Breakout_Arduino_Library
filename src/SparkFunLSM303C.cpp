#include "SparkFunLSM303C.h"
#include "stdint.h"


// Public methods
status_t LSM303C::begin()
{
  uint8_t successes = 0;
  ////////// Initialize Magnetometer //////////
  // Initialize magnetometer output data rate to 0.625 Hz (turn on device)
  successes += MAG_SetODR(MAG_DO_40_Hz);
  // Initialize magnetic field full scale to +/-16 gauss
  successes += MAG_SetFullScale(MAG_FS_16_Ga);
  // Enabling block data updating
  successes += MAG_BlockDataUpdate(MAG_BDU_ENABLE);
  // Initialize magnetometer X/Y axes ouput data rate to high-performance mode
  successes += MAG_XY_AxOperativeMode(MAG_OMXY_HIGH_PERFORMANCE);
  // Initialize magnetometer Z axis performance mode
  successes += MAG_Z_AxOperativeMode(MAG_OMZ_HIGH_PERFORMANCE);
  // Initialize magnetometer run mode. Also enables I2C (bit 7 = 0)
  successes += MAG_SetMode(MAG_MD_CONTINUOUS);

  ////////// Initialize Accelerometer //////////
  // Initialize acceleration full scale to +/-2g
  successes += ACC_SetFullScale(ACC_FS_2g);
  // Enable block data updating
  successes += ACC_BlockDataUpdate(ACC_BDU_ENABLE);
  // Enable X, Y, and Z accelerometer axes
  successes += ACC_EnableAxis(ACC_X_ENABLE|ACC_Y_ENABLE|ACC_Z_ENABLE);
  // Initialize accelerometer output data rate to 100 Hz (turn on device)
  successes += ACC_SetODR(ACC_ODR_100_Hz);

  if (successes == IMU_SUCCESS)
  {
    return IMU_SUCCESS;
  }

  return IMU_HW_ERROR;
}

float LSM303C::readMagX()
{
  return 12.345;
}

float LSM303C::readMagY()
{
  return -1.0;
}

float LSM303C::readMagZ()
{
  return -1.0;
}

float LSM303C::readAccel(AXIS_t dir)
{
  AxesRaw_t data;
  uint8_t flag_ACC_STATUS_FLAGS;
  status_t response = ACC_Status_Flags(flag_ACC_STATUS_FLAGS);
  float ret = NAN;
  
  if (response != IMU_SUCCESS)
  {
    Serial.println("\nError: Accelerometer isn't working!!!");
    ret = NAN;
  }
  // Check for new data in the status flags with a mask
  else if (flag_ACC_STATUS_FLAGS & ACC_ZYX_NEW_DATA_AVAILABLE)
  {
    response = ACC_GetAccRaw(data);
    debug_println("Got raw data");
    //convert from LSB to mg
    switch (dir)
    {
    case xAxis:
      debug_println("Reading accel in x");
      ret = data.xAxis * SENSITIVITY_ACC;
      break;
    case yAxis:
      debug_println("Reading accel in y");
      ret = data.yAxis * SENSITIVITY_ACC;
      break;
    case zAxis:
      debug_println("Reading accel in z");
      ret = data.zAxis * SENSITIVITY_ACC;
      break;
    default:
      Serial.println("Fell through...");
      ret = NAN;
    }
  }
  debug_println("Returning NAN");
  return ret;
}

float LSM303C::readAccelX()
{
  return readAccel(xAxis);
}

float LSM303C::readAccelY()
{
  return readAccel(yAxis);
}

float LSM303C::readAccelZ()
{
  return readAccel(zAxis);
}

float LSM303C::readTempC()
{
  return 25.45;
}

float LSM303C::readTempF()
{
  return 25.45;
}

////// Protected methods

// Methods required to get device up and running
status_t LSM303C::MAG_SetODR(MAG_DO_t val)
{
  debug_println("Setting mag ODR");
  uint8_t value;

  if(MAG_ReadReg(MAG_CTRL_REG1, value))
  {
    return IMU_HW_ERROR;
  }

  // Mask and only change DO0 bits (4:2) of MAG_CTRL_REG1
  value &= ~MAG_DO_80_Hz;
  value |= val;

  if(MAG_WriteReg(MAG_CTRL_REG1, value))
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;
}

status_t LSM303C::MAG_SetFullScale(MAG_FS_t val)
{
  debug_println("Setting mag full scale");
  uint8_t value;

  if ( MAG_ReadReg(MAG_CTRL_REG2, value) )
  {
    return IMU_HW_ERROR;
  }

  value &= ~MAG_FS_16_Ga; //mask
  value |= val;	

  if ( MAG_WriteReg(MAG_CTRL_REG2, value) )
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;
}

status_t LSM303C::MAG_BlockDataUpdate(MAG_BDU_t val)
{
  debug_println("Setting mag block data update");
  uint8_t value;

  if ( MAG_ReadReg(MAG_CTRL_REG5, value) )
  {
    return IMU_HW_ERROR;
  }


  value &= ~MAG_BDU_ENABLE; //mask
  value |= val;		

  if ( MAG_WriteReg(MAG_CTRL_REG5, value) )
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;
}

status_t LSM303C::MAG_XY_AxOperativeMode(MAG_OMXY_t val)
{
  debug_println("Setting mag XY axes operating mode");

  uint8_t value;

  if ( MAG_ReadReg(MAG_CTRL_REG1, value) )
  {
    return IMU_HW_ERROR;
  }
	
  value &= ~MAG_OMXY_ULTRA_HIGH_PERFORMANCE; //mask
  value |= val;	

  if ( MAG_WriteReg(MAG_CTRL_REG1, value) )
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;
}

status_t LSM303C::MAG_Z_AxOperativeMode(MAG_OMZ_t val)
{
  debug_println("Setting mag Z axis operating mode");
  uint8_t value;

  if ( MAG_ReadReg(MAG_CTRL_REG4, value) )
  {
    return IMU_HW_ERROR;
  }

  value &= ~MAG_OMZ_ULTRA_HIGH_PERFORMANCE; //mask
  value |= val;	

  if ( MAG_WriteReg(MAG_CTRL_REG4, value) )
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;
}

status_t LSM303C::MAG_SetMode(MAG_MD_t val)
{
  debug_println("Setting mag mode");
  uint8_t value;

  if ( MAG_ReadReg(MAG_CTRL_REG3, value) )
  {
    debug_print("Failed to read MAG_CTRL_REG3. 'Read': 0x");
    debug_printlns(value, HEX);
    return IMU_HW_ERROR;
  }

  value &= ~MAG_MD_POWER_DOWN_2;
  value |= val;		

  debug_println("About to call MAG_WriteReg()");
  if ( MAG_WriteReg(MAG_CTRL_REG3, value) )
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;
}

status_t LSM303C::ACC_SetFullScale(ACC_FS_t val)
{
  debug_println("Setting acc setting full scale");
  uint8_t value;

  if ( ACC_ReadReg(ACC_CTRL4, value) )
  {
    return IMU_HW_ERROR;
  }

  value &= ~ACC_FS_8g;
  value |= val;	


  if ( ACC_WriteReg(ACC_CTRL4, value) )
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;
}

status_t LSM303C::ACC_BlockDataUpdate(ACC_BDU_t val)
{
  debug_println("Setting acc block data update");
  uint8_t value;

  if ( ACC_ReadReg(ACC_CTRL1, value) )
  {
    return IMU_HW_ERROR;
  }

  value &= ~ACC_BDU_ENABLE;
  value |= val;	

  if ( ACC_WriteReg(ACC_CTRL1, value) )
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;
}

status_t LSM303C::ACC_EnableAxis(uint8_t val)
{
  debug_println("Setting acc enabling axes");
  uint8_t value;

  if ( ACC_ReadReg(ACC_CTRL1, value) )
  {
    return IMU_HW_ERROR;
  }

  value &= ~0x07;
  value |= val;	

  if ( ACC_WriteReg(ACC_CTRL1, value) )
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;

}

status_t LSM303C::ACC_SetODR(ACC_ODR_t val)
{
  debug_println("Setting acc setting ODR");
  uint8_t value;

  if ( ACC_ReadReg(ACC_CTRL1, value) )
  {
    return IMU_HW_ERROR;
  }

  value &= ~ACC_ODR_MASK;
  value |= val;	
	
  if ( ACC_WriteReg(ACC_CTRL1, value) )
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;
}

status_t LSM303C::MAG_ReadReg(uint8_t reg, uint8_t& data)
{
  debug_print("Reading register 0x");
  debug_printlns(reg, HEX);
  status_t ret;
    
  if (interfaceMode == MODE_I2C)
  {
    ret = I2C_ByteRead(MAG_I2C_ADDR, reg, data);
    #ifdef DEBUG
      if (ret != IMU_SUCCESS)
    #endif
      debug_println("I2C read failed");
  }
  else if (interfaceMode == MODE_SPI)
  {
    // TODO: fix this
    //ret = SPIreadByte(gAddress, reg);
    ret = IMU_NOT_SUPPORTED; // Not implemented yet
  }
  else
  {
    // TODO: Finish
    ret = IMU_GENERIC_ERROR; // interfaceMode not set valid value (not worried)
  }

  return ret;
}

uint8_t  LSM303C::MAG_WriteReg(uint8_t reg, uint8_t data)
{
  uint8_t ret;
    
  if (interfaceMode == MODE_I2C)
  {
    ret = I2C_ByteWrite(MAG_I2C_ADDR, reg, data);
  }
  else if (interfaceMode == MODE_SPI)
  {
    // TODO: fix this
    //ret = SPIreadByte(gAddress, reg);
    ret = IMU_NOT_SUPPORTED;
  }
  else
  {
    ret = IMU_GENERIC_ERROR;
  }

  return ret;
}

status_t LSM303C::ACC_ReadReg(uint8_t reg, uint8_t& data)
{
  debug_print("Reading address 0x");
  debug_printlns(reg, HEX);
  status_t ret;
    
  if (interfaceMode == MODE_I2C)
  {
    // TODO: looks like this call is failing
    ret = I2C_ByteRead(ACC_I2C_ADDR, reg, data);
    debug_print("Read: 0x");
    debug_printlns(ret, HEX);
  }
  else if (interfaceMode == MODE_SPI)
  {
    // TODO: fix this
    //ret = SPIreadByte(gAddress, reg);
    ret = IMU_NOT_SUPPORTED;
  }
  else
  {
    ret = IMU_HW_ERROR;
  }

  return ret;
}

uint8_t  LSM303C::ACC_WriteReg(uint8_t reg, uint8_t data)
{
  uint8_t ret;
    
  if (interfaceMode == MODE_I2C)
  {
    ret = I2C_ByteWrite(ACC_I2C_ADDR, reg, data);
  }
  else if (interfaceMode == MODE_SPI)
  {
    // TODO: fix this
    //ret = SPIreadByte(gAddress, reg);
    ret = IMU_NOT_SUPPORTED;
  }
  else
  {
    ret = IMU_GENERIC_ERROR;
  }

  return ret;
}

uint8_t  LSM303C::I2C_ByteWrite(uint8_t slaveAddress, uint8_t reg, uint8_t data)
{
  uint8_t ret;
  Wire.beginTransmission(slaveAddress);  // Initialize the Tx buffer
  // returns num bytes written
  if (Wire.write(reg))
  {
    ret = Wire.write(data);
    if (ret)
    {
      debug_print("Wrote: 0x");
      debug_printlns(data, HEX);
      if (Wire.endTransmission())
      {
        ret = 0;
      }
      else
      {
        // TODO: fix return types
        ret = 0;
      }
    }
    else
    {
      ret = 0;
    }
  }
  else
  {
    ret = 0;
  }
  return ret;
}

status_t LSM303C::I2C_ByteRead(uint8_t slaveAddress, uint8_t reg, uint8_t& data)
{
  status_t ret = IMU_GENERIC_ERROR;
  debug_print("Reading from I2C address: 0x");
  debug_prints(slaveAddress, HEX);
  debug_prints(", register 0x");
  debug_printlns(reg, HEX);
  Wire.beginTransmission(slaveAddress); // Initialize the Tx buffer
  if (Wire.write(reg))  // Put slave register address in Tx buff
  {
    if (Wire.endTransmission(false))  // Send Tx, send restart to keep alive
    {
      debug_println("Error: I2C buffer didn't get sent!");
      debug_print("Slave address: 0x");
      debug_printlns(slaveAddress, HEX);
      debug_print("Register: 0x");
      debug_printlns(reg, HEX);

      ret = IMU_HW_ERROR;
    }
    else if (Wire.requestFrom(slaveAddress, 1))
    {
      debug_print("Data before read: 0x");
      debug_printlns(data, HEX);
      data = Wire.read();
      debug_print("Read: 0x");
      debug_printlns(data, HEX);
      ret = IMU_SUCCESS;
    }
    else
    {
      debug_println("IMU_HW_ERROR");
      ret = IMU_HW_ERROR;
    }
  }
  else
  {
    debug_println("Error: couldn't send slave register address");
  }
  return ret;
}

status_t LSM303C::ACC_Status_Flags(uint8_t& val)
{
  debug_println("Getting acc status");
  if( ACC_ReadReg(ACC_STATUS, val) )
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;
}

status_t LSM303C::ACC_GetAccRaw(AxesRaw_t& buff)
{
  uint8_t valueL;
  uint8_t valueH;

  if ( ACC_ReadReg(ACC_OUT_X_H, valueH) )
  {
	  return IMU_HW_ERROR;
  }
  
  if ( ACC_ReadReg(ACC_OUT_X_L, valueL) )
  {
	  return IMU_HW_ERROR;
  }
  
  buff.xAxis = (int16_t)( (valueH << 8) | valueL );
  
  if ( ACC_ReadReg(ACC_OUT_Y_H, valueH) )
  {
	  return IMU_HW_ERROR;
  }
  
  if ( ACC_ReadReg(ACC_OUT_Y_L, valueL) )
  {
	  return IMU_HW_ERROR;
  }
  
  buff.yAxis = (int16_t)( (valueH << 8) | valueL );
  
  if ( ACC_ReadReg(ACC_OUT_Z_H, valueH) )
  {
	  return IMU_HW_ERROR;
  }
  
  if ( ACC_ReadReg(ACC_OUT_Z_L, valueL) )
  {
	  return IMU_HW_ERROR;
  }
  
  buff.zAxis = (int16_t)( (valueH << 8) | valueL ); 

  return IMU_SUCCESS;
}

// Preinstantiate Objects //////////////////////////////////////////////////////

//LSM303C lsm303c;
