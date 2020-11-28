/**
 * connect GND to Arduino GND
 * connect VDD to 3.3V
 * SDA to Arduino SDA
 * SCL to Arduino SCL
 * imu.gx, imu.gy, imu.gz are raw angular velocity, use calcGyro(imu.gx) has shown in printGyrometer
 * imu.ax, imu.ay, imu.az are raw linear velocity, use calcAccel(imu.ax) has shown in printAccelerometer
 * imu.mx, imu.my, imu.mz are raw magnetic strength, use calMag(imu.mx) has shown in printMagnetometer
 * 
 * in the print functions it prints out the values you would want to grab
 */


/*****************************************************************
  LSM9DS1_Basic_I2C.ino
  SFE_LSM9DS1 Library Simple Example Code - I2C Interface
  Jim Lindblom @ SparkFun Electronics
  Original Creation Date: April 30, 2015
  https://github.com/sparkfun/LSM9DS1_Breakout

  The LSM9DS1 is a versatile 9DOF sensor. It has a built-in
  accelerometer, gyroscope, and magnetometer. Very cool! Plus it
  functions over either SPI or I2C.

  This Arduino sketch is a demo of the simple side of the
  SFE_LSM9DS1 library. It'll demo the following:
  How to create a LSM9DS1 object, using a constructor (global
  variables section).
  How to use the begin() function of the LSM9DS1 class.
  How to read the gyroscope, accelerometer, and magnetometer
  using the readGryo(), readAccel(), readMag() functions and
  the gx, gy, gz, ax, ay, az, mx, my, and mz variables.
  How to calculate actual acceleration, rotation speed,
  magnetic field strength using the calcAccel(), calcGyro()
  and calcMag() functions.
  How to use the data from the LSM9DS1 to calculate
  orientation and heading.

  Hardware setup: This library supports communicating with the
  LSM9DS1 over either I2C or SPI. This example demonstrates how
  to use I2C. The pin-out is as follows:
	LSM9DS1 --------- Arduino
	 SCL ---------- SCL (A5 on older 'Duinos')
	 SDA ---------- SDA (A4 on older 'Duinos')
	 VDD ------------- 3.3V
	 GND ------------- GND
  (CSG, CSXM, SDOG, and SDOXM should all be pulled high.
  Jumpers on the breakout board will do this for you.)

  The LSM9DS1 has a maximum voltage of 3.6V. Make sure you power it
  off the 3.3V rail! I2C pins are open-drain, so you'll be
  (mostly) safe connecting the LSM9DS1's SCL and SDA pins
  directly to the Arduino.

  Development environment specifics:
	IDE: Arduino 1.6.3
	Hardware Platform: SparkFun Redboard
	LSM9DS1 Breakout Version: 1.0

  This code is beerware. If you see me (or any other SparkFun
  employee) at the local, and you've found our code helpful,
  please buy us a round!

  Distributed as-is; no warranty is given.
*****************************************************************/

#include <Wire.h>
#include <SparkFunLSM9DS1.h>

LSM9DS1 imu;

///////////////////////
// Example I2C Setup //
///////////////////////
// SDO_XM and SDO_G are both pulled high, so our addresses are:
// #define LSM9DS1_M	0x1E // Would be 0x1C if SDO_M is LOW
// #define LSM9DS1_AG	0x6B // Would be 0x6A if SDO_AG is LOW

#define PRINT_CALCULATED

//#define PRINT_RAW
#define PRINT_SPEED 100 // ms, time between prints
static unsigned long lastPrint = 0;

// Earth's magnetic field varies by location. Add or subtract
// a declination to get a more accurate heading. Calculate
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION -5.05 // Declination (degrees) in Atlanta, GA.

void printGyrometer();
void printAccelerometer();
void printMagnetometer();
void printAttitude(float ax, float ay, float az, float mx, float my, float mz);

void setup() {
  Serial.begin(115200);
  Wire.begin();

  if (imu.begin() == false) { // with no arguments, this uses default addresses (AG:0x6B, M:0x1E) and i2c port (Wire).
    Serial.println("Failed to communicate with LSM9DS1.");
    while(1);
  }
}

void loop() {
  if (imu.gyroAvailable()) {
    imu.readGyro();
  }
  if (imu.accelAvailable()) {
    imu.readAccel();
  }
  if (imu.magAvailable()) {
    imu.readMag();
  }

  if ((lastPrint + PRINT_SPEED) < millis()) {
//    printGyrometer();
    printAccelerometer();
//    printMagnetometer();
    // The LSM9DS1's mag x and y axes are opposite to the accelerometer, so my, mx are substituted for each other.
//    printAttitude(imu.ax, imu.ay, imu.az, -imu.my, -imu.mx, imu.mz);
//    Serial.println();
    lastPrint = millis();
  }
}

void printGyrometer() {
  Serial.print("G: ");
  #ifdef PRINT_CALCULATED
    Serial.print(imu.calcGyro(imu.gx), 2);
    Serial.print(", ");
    Serial.print(imu.calcGyro(imu.gy), 2);
    Serial.print(", ");
    Serial.print(imu.calcGyro(imu.gz), 2);
    Serial.println(" deg/s");
  #elif defined PRINT_RAW
    Serial.print(imu.gx);
    Serial.print(", ");
    Serial.print(imu.gy);
    Serial.print(", ");
    Serial.println(imu.gz);
  #endif
}

void printAccelerometer() {
  Serial.print("A: ");
  #ifdef PRINT_CALCULATED
    Serial.print(imu.calcAccel(imu.ax), 2);
    Serial.print(", ");
    Serial.print(imu.calcAccel(imu.ay), 2);
    Serial.print(", ");
    Serial.print(imu.calcAccel(imu.az), 2);
    Serial.println(" g");
  #elif defined PRINT_RAW
    Serial.print(imu.ax);
    Serial.print(", ");
    Serial.print(imu.ay);
    Serial.print(", ");
    Serial.println(imu.az);
  #endif
}

void printMagnetometer() {
  Serial.print("M: ");
  #ifdef PRINT_CALCULATED
    Serial.print(imu.calcMag(imu.mx), 2);
    Serial.print(", ");
    Serial.print(imu.calcMag(imu.my), 2);
    Serial.print(", ");
    Serial.print(imu.calcMag(imu.mz), 2);
    Serial.println(" gauss");
  #elif defined PRINT_RAW
    Serial.print(imu.mx);
    Serial.print(", ");
    Serial.print(imu.my);
    Serial.print(", ");
    Serial.println(imu.mz);
  #endif
}

// Pitch/roll calculations take from this app note:
// http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
// Heading calculations taken from this app note:
// http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
void printAttitude(float ax, float ay, float az, float mx, float my, float mz) {
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));
  float roll = atan2(ay, az);

  float heading;
  if (my == 0) {
    heading = (mx < 0) ? PI : 0;
  } else {
    heading = atan2(mx, my);
  }
  heading -= DECLINATION * PI / 180;

  if (heading > PI) {
    heading -= (2 * PI);
  } else if (heading < -PI) {
    heading += (2 * PI);
  }

  // radians to degrees
  pitch *= 180.0 / PI;
  roll *= 180.0 / PI;
  heading *= 180.0 / PI;

  Serial.print("Pitch: ");
  Serial.print(pitch, 2);
  Serial.print(", Roll: ");
  Serial.println(roll, 2);
  Serial.print(", Heading: ");
  Serial.println(heading, 2);
}
