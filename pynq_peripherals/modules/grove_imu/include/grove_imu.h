/******************************************************************************
 *  Copyright (c) 2021, Xilinx, Inc.
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  1.  Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2.  Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *
 *  3.  Neither the name of the copyright holder nor the names of its
 *      contributors may be used to endorse or promote products derived from
 *      this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 *  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 *  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 *  OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/

#pragma once

#include <grove_constants.h>
#include <grove_imu_constants.h>
#include <pytypes.h>
#include <stdint.h>
#include <sys/errno.h>

// Device typedef
/* IMU Object
 *
 * Available Methods:
 *      open, close
 *
 *      set_clock_source
 *      set_full_scale_gyro_range
 *      set_full_scale_accel_range
 *      reset
 *      set_sleep_mode
 *      fetch_motion9
 *      get_accel_ax, get_accel_y, get_accel_z
 *      get_gyro_x, get_gyro_y, get_gyro_z
 *      get_magneto_x, get_magneto_y, get_magneto_z
 *
 *      get_temperature
 *      get_pressure
 */
typedef py_int grove_imu;

// Device lifetime functions

/* Open an IMU module connected to the specified port with the default 
 * I2C address
 * 
 * Parameters
 * ----------
 * grove_id: int
 *     Valid port ids for this device:
 *     PMOD_G3, PMOD_G4, ARDUINO_SEEED_I2C 
 * 
 * Returns
 * -------
 * grove_imu
 *      The device object  
 *      -EIO device not present or IO error (raises exception)
 *      -ENOMEM memory allocation error (raises exception)
 * 
 */
grove_imu grove_imu_open(int grove_id);

/* Open an IMU module connected to the specified port and a non-default
 * I2C address
 *
 * Unique I2C address is needed when two modules of same type are connected
 * to the same I2C channel. The two modules should have unique I2C
 * address
 *
 * Parameters
 * ----------
 * grove_id: int
 *     Valid port ids for this device:
 *     PMOD_G3, PMOD_G4, ARDUINO_SEEED_I2C 
 * 
 * Returns
 * -------
 * grove_imu
 *     The device object  
 */
/*
 * grove_imu grove_imu_open_at_address(int grove_id, int address);
*/

/* Release the IMU by closing the device 
 *
 * Parameters
 * ----------
 * grove_id : int
 *     The opened device object
 *
 * Returns
 * -------
 *     None
 *
 */
void grove_imu_close(grove_imu imu);

// Device functions

/*  Set the IMU clock source 
 *
 * Parameters
 * ----------
 * source : uint8
 *      Valid sources are:
 *      CLOCK_INTERNAL, 
 *      CLOCK_PLL_XGYRO, CLOCK_PLL_YGRYO,  CLOCK_PLL_ZGYRO
 *      CLOCK_PLL_EXT32K, CLOCK_EXT19M, CLOCK_KEEP_RESET
 *
 * Returns
 * -------
 *      None
 *
 */
py_void grove_imu_set_clock_source(grove_imu imu, uint8_t source);

/* Set the IMU full-scale gyro range 
 *
 * Parameters
 * ----------
 * source : enum
 *      Valid sources are:
 *      GYRO_FS_250, GYRO_FS_500, GYRO_FS_1000, GYRO_FS_2000
 *
 * Returns
 * -------
 *      None
 *
 */
py_void grove_imu_set_full_scale_gyro_range(grove_imu imu, uint8_t range);

/* Set the IMU full-scale accelearion range 
 *
 * Parameters
 * ----------
 * source : enum
 *      Valid sources are:
 *      ACCEL_FS_2, ACCEL_FS_4, ACCEL_FS_8, ACCEL_FS_16
 *
 * Returns
 * -------
 *      None
 *
 */
py_void grove_imu_set_full_scale_accel_range(grove_imu imu, uint8_t range);

/* Reset the internal registers and restores the default settings
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      None
 *
 */
py_void grove_imu_reset(grove_imu imu);

/* Enable or Disable sleep mode 
 *
 * Parameters
 * ----------
 *      mode: enum
 *      Valid sources are:
 *      SLEEP_ENABLED, SLEEP_DISABLED
 *
 * Returns
 * -------
 *      None
 *
 */
py_void grove_imu_set_sleep_mode(grove_imu imu, uint8_t mode);

/* Fetch the IMU accelerometer and gyroscope values
 * Fetch the value from IMU and store into this object
 * To get these stored values call the correspondiing 
 * get_accel_, get_gyro_ or get_magneto_ methods
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      0 if clock set successfully
 *      -EIO device not present or IO error (raises exception)
 */
py_void grove_imu_fetch_motion9(grove_imu imu);

/* Acceleration in direction X
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      float:
 *          Acceleration value 
 *          NAN general operation error (raises exception)
 */
py_float grove_imu_get_accel_x(grove_imu imu);

/* Acceleration in direction Y
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      float:
 *          Acceleration value 
 *          NAN general operation error (raises exception)
 */
py_float grove_imu_get_accel_y(grove_imu imu);

/* Acceleration in direction Z
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      float:
 *          Acceleration value 
 *          NAN general operation error (raises exception)
 */
py_float grove_imu_get_accel_z(grove_imu imu);

/* Angular velocity of axis X
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      float:
 *          Angular velocity value 
 *          NAN general operation error (raises exception)
 */
py_float grove_imu_get_gyro_x(grove_imu imu);

/* Angular velocity of axis Y
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      float:
 *          Angular velocity value 
 *          NAN general operation error (raises exception)
 */
py_float grove_imu_get_gyro_y(grove_imu imu);

/* Angular velocity of axis Z
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      float:
 *          Angular velocity value 
 *          NAN general operation error (raises exception)
 */
py_float grove_imu_get_gyro_z(grove_imu imu);

/* X-axis Magnetometer value
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      float:
 *          Magnetometer value 
 *          NAN general operation error (raises exception)
 */
py_float grove_imu_get_magneto_x(grove_imu imu);

/* Y-axis Magnetometer value
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      float:
 *          Magnetometer value 
 *          NAN general operation error (raises exception)
 */
py_float grove_imu_get_magneto_y(grove_imu imu);

/* Z-axis Magnetometer value
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      float:
 *          Magnetometer value 
 *          NAN general operation error (raises exception)
 */
py_float grove_imu_get_magneto_z(grove_imu imu);

/* Read the IMU temperature value
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      float:
 *          Temperature value in degree Celsius
 *          NAN general operation error (raises exception)
 */
py_float grove_imu_get_temperature(grove_imu imu);

/* Read the IMU pressure value
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      int:
 *          Presssure value in Pa
 *          -EIO device not present or IO error (raises exception)
 */
py_int grove_imu_get_pressure(grove_imu imu);
