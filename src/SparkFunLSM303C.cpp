#include "SparkFunLSM303C.h"
#include "stdint.h"

// Public methods
status_t LSM303C::begin()
{
  debug_println(EMPTY);
  return
  begin(// Default to I2C bus
        MODE_I2C,
        // Initialize magnetometer output data rate to 0.625 Hz (turn on device)
        MAG_DO_40_Hz,
        // Initialize magnetic field full scale to +/-16 gauss
        MAG_FS_16_Ga,
        // Enabling block data updating
        MAG_BDU_ENABLE,
        // Initialize magnetometer X/Y axes ouput data rate to high-perf mode
        MAG_OMXY_HIGH_PERFORMANCE,
        // Initialize magnetometer Z axis performance mode
        MAG_OMZ_HIGH_PERFORMANCE,
        // Initialize magnetometer run mode. Also enables I2C (bit 7 = 0)
        MAG_MD_CONTINUOUS,
        // Initialize acceleration full scale to +/-2g
        ACC_FS_2g,
        // Enable block data updating
        ACC_BDU_ENABLE,
        // Enable X, Y, and Z accelerometer axes
        ACC_X_ENABLE|ACC_Y_ENABLE|ACC_Z_ENABLE,
        // Initialize accelerometer output data rate to 100 Hz (turn on device)
        ACC_ODR_100_Hz
        );
}

status_t LSM303C::begin(InterfaceMode_t im, MAG_DO_t modr, MAG_FS_t mfs,
    MAG_BDU_t mbu, MAG_OMXY_t mxyodr, MAG_OMZ_t mzodr, MAG_MD_t mm,
    ACC_FS_t afs, ACC_BDU_t abu, uint8_t aea, ACC_ODR_t aodr)
{
  uint8_t successes = 0;
  // Select I2C or SPI
  interfaceMode = im;

  if (interfaceMode == MODE_SPI)
  {
    debug_println("Setting up SPI");
    // Setup pins for SPI
    // CS & CLK must be outputs DDRxn = 1
    bitSet(DIR_REG, CSBIT_MAG);
    bitSet(DIR_REG, CSBIT_XL);
    bitSet(DIR_REG, CLKBIT);
    // Deselect SPI chips
    bitSet(CSPORT_MAG, CSBIT_MAG);
    bitSet(CSPORT_XL, CSBIT_XL);
    // Clock polarity (CPOL) = 1
    bitSet(CLKPORT, CLKBIT);
    // SPI Serial Interface Mode (SIM) bits must be set
    SPI_WriteByte(ACC, ACC_CTRL4, 0b111);
    SPI_WriteByte(MAG, MAG_CTRL_REG3, _BV(2));
  }
  else
  {
    Wire.begin();
    Wire.setClock(400000L);
  }
  ////////// Initialize Magnetometer //////////
  // Initialize magnetometer output data rate
  successes += MAG_SetODR(modr);
  // Initialize magnetic field full scale
  successes += MAG_SetFullScale(mfs);
  // Enabling block data updating
  successes += MAG_BlockDataUpdate(mbu);
  // Initialize magnetometer X/Y axes ouput data rate
  successes += MAG_XY_AxOperativeMode(mxyodr);
  // Initialize magnetometer Z axis performance mode
  successes += MAG_Z_AxOperativeMode(mzodr);
  // Initialize magnetometer run mode.
  successes += MAG_SetMode(mm);

  ////////// Initialize Accelerometer //////////
  // Initialize acceleration full scale
  successes += ACC_SetFullScale(afs);
  // Enable block data updating
  successes += ACC_BlockDataUpdate(abu);
  // Enable X, Y, and Z accelerometer axes
  successes += ACC_EnableAxis(aea);
  // Initialize accelerometer output data rate
  successes += ACC_SetODR(aodr);

  return (successes == IMU_SUCCESS) ? IMU_SUCCESS : IMU_HW_ERROR;
}

float LSM303C::readMagX()
{
  return readMag(xAxis);
}

float LSM303C::readMagY()
{
  return readMag(yAxis);
}

float LSM303C::readMagZ()
{
  return readMag(zAxis);
}

float LSM303C::readAccelX()
{
  uint8_t flag_ACC_STATUS_FLAGS;
  status_t response = ACC_Status_Flags(flag_ACC_STATUS_FLAGS);
  
  if (response != IMU_SUCCESS)
  {
    debug_println(AERROR);
    return NAN;
  }
  
  // Check for new data in the status flags with a mask
  // If there isn't new data use the last data read.
  // There are valid cases for this, like reading faster than refresh rate.
  if (flag_ACC_STATUS_FLAGS & ACC_X_NEW_DATA_AVAILABLE)
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
  
    debug_println("Fresh raw data");

    //convert from LSB to mg
    return int16_t(( (valueH << 8) | valueL )) * SENSITIVITY_ACC;
  }

  // Should never get here
  debug_println("Returning NAN");
  return NAN;
}

float LSM303C::readAccelY()
{
  uint8_t flag_ACC_STATUS_FLAGS;
  status_t response = ACC_Status_Flags(flag_ACC_STATUS_FLAGS);
  
  if (response != IMU_SUCCESS)
  {
    debug_println(AERROR);
    return NAN;
  }
  
  // Check for new data in the status flags with a mask
  // If there isn't new data use the last data read.
  // There are valid cases for this, like reading faster than refresh rate.
  if (flag_ACC_STATUS_FLAGS & ACC_Y_NEW_DATA_AVAILABLE)
  {
    uint8_t valueL;
    uint8_t valueH;

    if ( ACC_ReadReg(ACC_OUT_Y_H, valueH) )
    {
	    return IMU_HW_ERROR;
    }
  
    if ( ACC_ReadReg(ACC_OUT_Y_L, valueL) )
    {
	    return IMU_HW_ERROR;
    }
  
    debug_println("Fresh raw data");

    //convert from LSB to mg
    return int16_t(( (valueH << 8) | valueL )) * SENSITIVITY_ACC;
  }

  // Should never get here
  debug_println("Returning NAN");
  return NAN;
}

float LSM303C::readAccelZ()
{
  uint8_t flag_ACC_STATUS_FLAGS;
  status_t response = ACC_Status_Flags(flag_ACC_STATUS_FLAGS);
  
  if (response != IMU_SUCCESS)
  {
    debug_println(AERROR);
    return NAN;
  }
  
  // Check for new data in the status flags with a mask
  // If there isn't new data use the last data read.
  // There are valid cases for this, like reading faster than refresh rate.
  if (flag_ACC_STATUS_FLAGS & ACC_Z_NEW_DATA_AVAILABLE)
  {
    uint8_t valueL;
    uint8_t valueH;

    if ( ACC_ReadReg(ACC_OUT_Z_H, valueH) )
    {
	    return IMU_HW_ERROR;
    }
  
    if ( ACC_ReadReg(ACC_OUT_Z_L, valueL) )
    {
	    return IMU_HW_ERROR;
    }
  
    debug_println("Fresh raw data");

    //convert from LSB to mg
    return(int16_t(( (valueH << 8) | valueL )) * SENSITIVITY_ACC);
  }

  // Should never get here
  debug_println("Returning NAN");
  return NAN;
}


float LSM303C::readTempC()
{
  uint8_t valueL;
  uint8_t valueH;
  float temperature;

  // Make sure temperature sensor is enabled
  if( MAG_TemperatureEN(MAG_TEMP_EN_ENABLE))
  {
    return NAN;
  }

	if( MAG_ReadReg(MAG_TEMP_OUT_L, valueL) )
  {
    return NAN;
  }

  if( MAG_ReadReg(MAG_TEMP_OUT_H, valueH) )
  {
    return NAN;
  }

  temperature = (float)( (valueH << 8) | valueL );
  temperature /= 8; // 8 digits/˚C
  temperature += 25;// Reads 0 @ 25˚C

  return temperature;  
}

float LSM303C::readTempF()
{
  return( (readTempC() * 9.0 / 5.0) + 32.0);
}



////////////////////////////////////////////////////////////////////////////////
////// Protected methods

float LSM303C::readAccel(AXIS_t dir)
{
  uint8_t flag_ACC_STATUS_FLAGS;
  status_t response = ACC_Status_Flags(flag_ACC_STATUS_FLAGS);
  
  if (response != IMU_SUCCESS)
  {
    debug_println(AERROR);
    return NAN;
  }
  
  // Check for new data in the status flags with a mask
  // If there isn't new data use the last data read.
  // There are valid cases for this, like reading faster than refresh rate.
  if (flag_ACC_STATUS_FLAGS & ACC_ZYX_NEW_DATA_AVAILABLE)
  {
    response = ACC_GetAccRaw(accelData);
    debug_println("Fresh raw data");
  }
  //convert from LSB to mg
  switch (dir)
  {
  case xAxis:
    return accelData.xAxis * SENSITIVITY_ACC;
    break;
  case yAxis:
    return accelData.yAxis * SENSITIVITY_ACC;
    break;
  case zAxis:
    return accelData.zAxis * SENSITIVITY_ACC;
    break;
  default:
    return NAN;
  }

  // Should never get here
  debug_println("Returning NAN");
  return NAN;
}

float LSM303C::readMag(AXIS_t dir)
{
  MAG_XYZDA_t flag_MAG_XYZDA;
  status_t response = MAG_XYZ_AxDataAvailable(flag_MAG_XYZDA);
  
  if (response != IMU_SUCCESS)
  {
    debug_println(MERROR);
    return NAN;
  }
  
  // Check for new data in the status flags with a mask
  if (flag_MAG_XYZDA & MAG_XYZDA_YES)
  {
    response = MAG_GetMagRaw(magData);
    debug_println("Fresh raw data");
  }
  //convert from LSB to Gauss
  switch (dir)
  {
  case xAxis:
    return magData.xAxis * SENSITIVITY_MAG;
    break;
  case yAxis:
    return magData.yAxis * SENSITIVITY_MAG;
    break;
  case zAxis:
    return magData.zAxis * SENSITIVITY_MAG;
    break;
  default:
    return NAN;
  }

  // Should never get here
  debug_println("Returning NAN");
  return NAN;
}

status_t LSM303C::MAG_GetMagRaw(AxesRaw_t& buff)
{
  debug_print(EMPTY);
  uint8_t valueL;
  uint8_t valueH;
  
  debug_println("& was false");
  if( MAG_ReadReg(MAG_OUTX_L, valueL) )
  {
    return IMU_HW_ERROR;
  }

  if( MAG_ReadReg(MAG_OUTX_H, valueH) )
  {
    return IMU_HW_ERROR;
  }

  buff.xAxis = (int16_t)( (valueH << 8) | valueL );
  
  if( MAG_ReadReg(MAG_OUTY_L, valueL) )
  {
    return IMU_HW_ERROR;
  }

  if( MAG_ReadReg(MAG_OUTY_H, valueH) )
  {
    return IMU_HW_ERROR;
  }

  buff.yAxis = (int16_t)( (valueH << 8) | valueL );
  
  if( MAG_ReadReg(MAG_OUTZ_L, valueL) )
  {
    return IMU_HW_ERROR;
  }

  if( MAG_ReadReg(MAG_OUTZ_H, valueH) )
  {
    return IMU_HW_ERROR;
  }

  buff.zAxis = (int16_t)( (valueH << 8) | valueL );

  return IMU_SUCCESS;
}

// Methods required to get device up and running
status_t LSM303C::MAG_SetODR(MAG_DO_t val)
{
  debug_print(EMPTY);
  uint8_t value;

  if(MAG_ReadReg(MAG_CTRL_REG1, value))
  {
    debug_printlns("Failed Read from MAG_CTRL_REG1");
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
  debug_print(EMPTY);
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
  debug_print(EMPTY);
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

status_t LSM303C::MAG_XYZ_AxDataAvailable(MAG_XYZDA_t& value)
{
  if ( MAG_ReadReg(MAG_STATUS_REG, (uint8_t&)value) )
  {
    return IMU_HW_ERROR;
  }

  value = (MAG_XYZDA_t)((int8_t)value & (int8_t)MAG_XYZDA_YES);

  return IMU_SUCCESS;
}

status_t LSM303C::MAG_XY_AxOperativeMode(MAG_OMXY_t val)
{
  debug_print(EMPTY);

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
  debug_print(EMPTY);
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
  debug_print(EMPTY);
  uint8_t value;

  if ( MAG_ReadReg(MAG_CTRL_REG3, value) )
  {
    debug_print("Failed to read MAG_CTRL_REG3. 'Read': 0x");
    debug_printlns(value, HEX);
    return IMU_HW_ERROR;
  }

  value &= ~MAG_MD_POWER_DOWN_2;
  value |= val;		

  if ( MAG_WriteReg(MAG_CTRL_REG3, value) )
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;
}

status_t LSM303C::ACC_SetFullScale(ACC_FS_t val)
{
  debug_print(EMPTY);
  uint8_t value;

  if ( ACC_ReadReg(ACC_CTRL4, value) )
  {
    debug_printlns("Failed ACC read");
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
  debug_print(EMPTY);
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
  debug_print(EMPTY);
  uint8_t value;

  if ( ACC_ReadReg(ACC_CTRL1, value) )
  {
    debug_println(AERROR);
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
  debug_print(EMPTY);
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

status_t LSM303C::MAG_TemperatureEN(MAG_TEMP_EN_t val){
  uint8_t value;

  if( MAG_ReadReg(MAG_CTRL_REG1, value) )
  {
    return IMU_HW_ERROR;
  }

  value &= ~MAG_TEMP_EN_ENABLE; //mask
  value |= val;	

  if( MAG_WriteReg(MAG_CTRL_REG1, value) )
  {
    return IMU_HW_ERROR;
  }

  return IMU_SUCCESS;
}

status_t LSM303C::MAG_ReadReg(MAG_REG_t reg, uint8_t& data)
{
  debug_print("Reading register 0x");
  debug_printlns(reg, HEX);
  status_t ret = IMU_GENERIC_ERROR;
    
  if (interfaceMode == MODE_I2C)
  {
    ret = I2C_ByteRead(MAG_I2C_ADDR, reg, data);
  }
  else if (interfaceMode == MODE_SPI)
  {
    data = SPI_ReadByte(MAG, reg);
    ret = IMU_SUCCESS;
  }
  else
  {
    ret = IMU_GENERIC_ERROR; // Shouldn't get here
  }

  return ret;
}

uint8_t  LSM303C::MAG_WriteReg(MAG_REG_t reg, uint8_t data)
{
  debug_print(EMPTY);
  uint8_t ret;
    
  if (interfaceMode == MODE_I2C)
  {
    ret = I2C_ByteWrite(MAG_I2C_ADDR, reg, data);
  }
  else if (interfaceMode == MODE_SPI)
  {
    ret = SPI_WriteByte(MAG, reg, data);
  }
  else
  {
    ret = IMU_GENERIC_ERROR;
  }

  return ret;
}

status_t LSM303C::ACC_ReadReg(ACC_REG_t reg, uint8_t& data)
{
  debug_print("Reading address 0x");
  debug_printlns(reg, HEX);
  status_t ret;
    
  if (interfaceMode == MODE_I2C)
  {
    ret = I2C_ByteRead(ACC_I2C_ADDR, reg, data);
  }
  else if (interfaceMode == MODE_SPI)
  {
    data = SPI_ReadByte(ACC, reg);
    ret = IMU_SUCCESS;
  }
  else
  {
    ret = IMU_HW_ERROR;
  }

  return ret;
}





uint8_t  LSM303C::ACC_WriteReg(ACC_REG_t reg, uint8_t data)
{
  debug_print(EMPTY);
  uint8_t ret;
    
  if (interfaceMode == MODE_I2C)
  {
    ret = I2C_ByteWrite(ACC_I2C_ADDR, reg, data);
  }
  else if (interfaceMode == MODE_SPI)
  {
    ret = SPI_WriteByte(ACC, reg, data);
  }
  else
  {
    ret = IMU_GENERIC_ERROR;
  }

  return ret;
}

// This function uses bit manibulation for higher speed & smaller code
uint8_t LSM303C::SPI_ReadByte(CHIP_t chip, uint8_t data)
{
  debug_print("Reading register 0x");
  debug_printlns(data, HEX);
  uint8_t counter;

  // Set the read/write bit (bit 7) to do a read
  data |= _BV(7);

  // Set data pin to output
  bitSet(DIR_REG, DATABIT);
 
  noInterrupts();

  // Select the chip & deselect the other
  switch (chip)
  {
  case MAG:
    bitClear(CSPORT_MAG, CSBIT_MAG);
    bitSet(CSPORT_XL, CSBIT_XL);
    break;
  case ACC:
    bitClear(CSPORT_XL, CSBIT_XL);
    bitSet(CSPORT_MAG, CSBIT_MAG);
    break;
  }

  // Shift out 8-bit address
  for(counter = 8; counter; counter--)
  {
    bitWrite(DATAPORTO, DATABIT, data & 0x80);
    // Data is setup, so drop clock edge
    bitClear(CLKPORT, CLKBIT);
    bitSet(CLKPORT, CLKBIT);
    // Shift off sent bit
    data <<= 1;
  }
  
  // Switch data pin to input (0 = INPUT)
  bitClear(DIR_REG, DATABIT);

  // Shift in register data from address
  for(counter = 8; counter; counter--)
  {
    // Shift data to the left.  Remains 0 after first shift
    data <<= 1;

    bitClear(CLKPORT, CLKBIT);
    // Sample on rising egde
    bitSet(CLKPORT, CLKBIT);
    if (bitRead(DATAPORTI, DATABIT))
    {
      data |= 0x01;
    }
  }

  // Unselect chip
  switch (chip)
  {
  case MAG:
    bitSet(CSPORT_MAG, CSBIT_MAG);
    break;
  case ACC:
    bitSet(CSPORT_XL, CSBIT_XL);
    break;
  }

  interrupts();

  return(data);
}





// This function uses bit manibulation for higher speed & smaller code
status_t LSM303C::SPI_WriteByte(CHIP_t chip, uint8_t reg, uint8_t data)
{
  debug_print("Writing 0x");
  debug_prints(data, HEX);
  debug_prints(" to register 0x");
  debug_printlns(reg, HEX);

  uint8_t counter;
  uint16_t twoBytes;

  // Clear the read/write bit (bit 7) to do a write
  reg &= ~_BV(7);
  twoBytes = reg << 8 | data;

  // Set data pin to output
  bitSet(DIR_REG, DATABIT);
 
  noInterrupts();

  // Select the chip & deselect the other
  switch (chip)
  {
  case MAG:
    bitClear(CSPORT_MAG, CSBIT_MAG);
    bitSet(CSPORT_XL, CSBIT_XL);
    break;
  case ACC:
    bitClear(CSPORT_XL, CSBIT_XL);
    bitSet(CSPORT_MAG, CSBIT_MAG);
    break;
  }

  // Shift out 8-bit address & 8-bit data
  for(counter = 16; counter; counter--)
  {
    bitWrite(DATAPORTO, DATABIT, twoBytes & 0x8000);
    
    // Data is setup, so drop clock edge
    bitClear(CLKPORT, CLKBIT);
    bitSet(CLKPORT, CLKBIT);
    // Shift off sent bit
    twoBytes <<= 1;
  }
  
  // Unselect chip
  switch (chip)
  {
  case MAG:
    bitSet(CSPORT_MAG, CSBIT_MAG);
    break;
  case ACC:
    bitSet(CSPORT_XL, CSBIT_XL);
    break;
  }
 
  interrupts();

  // Set data pin to input
  bitClear(DIR_REG, DATABIT);

  // Is there a way to verify true success?
  return IMU_SUCCESS;
}




uint8_t  LSM303C::I2C_ByteWrite(I2C_ADDR_t slaveAddress, uint8_t reg,
    uint8_t data)
{
  uint8_t ret = IMU_GENERIC_ERROR;
  Wire.beginTransmission(slaveAddress);  // Initialize the Tx buffer
  // returns num bytes written
  if (Wire.write(reg))
  {
    ret = Wire.write(data);
    if (ret)
    {
      debug_print("Wrote: 0x");
      debug_printlns(data, HEX);
      switch (Wire.endTransmission())
      {
      case 0:
        ret = IMU_SUCCESS;
        break;
      case 1: // Data too long to fit in transmit buffer
      case 2: // Received NACK on transmit of address
      case 3: // Received NACK on transmit of data
      case 4: // Other Error
      default:
        ret = IMU_HW_ERROR;
      }
    }
    else
    {
      ret = IMU_HW_ERROR;
    }
  }
  else
  {
    ret = IMU_HW_ERROR;
  }
  return ret;
}

status_t LSM303C::I2C_ByteRead(I2C_ADDR_t slaveAddress, uint8_t reg,
    uint8_t& data)
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
  debug_println("Getting accel status");
  if( ACC_ReadReg(ACC_STATUS, val) )
  {
    debug_println(AERROR);
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
