SparkFun IMU Interface
===
Arduino libraries that implement this interface are guaranteed to to work interchangeably in sketches. If a project is started with 6-DOF IMU, but one realizes that the project really needs a 9-DOF sensor, the change is simple.  Include the new header file, & change the type of the IMU object(s).  All of the code written to the interface will still function.  Here is a list of the functions provided by this interface:
* `status_t begin()` - initialize IMU to sane default values
* `float readGyroX()` - read value from the gyroscope about x-axis
* `float readGyroY()` - read value from the gyroscope about y-axis
* `float readGyroZ()` - read value from the gyroscope about z-axis
* `float readAccelX()` - read value from the accelerometer in x-axis
* `float readAccelY()` - read value from the accelerometer in y-axis
* `float readAccelZ()` - read value from the accelerometer in z-axis
* `float readMagX()` - read value from the magnetometer in x-axis
* `float readMagY()` - read value from the magnetometer in y-axis
* `float readMagZ()` - read value from the magnetometer in z-axis
* `float readTempC()` - read temperature of sensor in ˚C
* `float readTempF()` - read temperature of sensor in ˚F
* `status_t getStatus()` - read the status code of the previous operation

All functions other than `getStatus()` will update the status code depending on how their operation went.  Errors in the read functions can be detected by them returning `NAN` or calling `getStatus()`.

If an IMU doesn't support a set of functions, they will always return `NAN` and set the status to `IMU_NOT_SUPPORTED`.

Other, more advanced features may be available depending on the sensor.  See the documentation for the particular sensor for information on advanced feature support.

Using as a Template
===============

* Create your repo
* pull the contents of /IMU into it
* modify the library.properties to have real values (or arduino won't take it)
* "load .zip" and select your repo folder
