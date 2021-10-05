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
 *  OR BUSINESS py_intERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 *  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 *  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 *  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *****************************************************************************/
 
#pragma once

#include <grove_constants.h>
#include <pytypes.h>

// Device typedef
/* grove_envsensor class 
 *
 * Available Methods:
 *    open, open_at_address,close,init,
 *    reg_write, read_data, read_temperature, read_pressure
 *    read_humidity
 *    
 */
typedef int grove_envsensor;

// Device lifetime functions
/* Open a grove environment sensor device connected to the specified port 
 * 
 * Parameters
 * ----------
 * grove_id: int
 *     Valid port ids for this device:
 *     PMOD_G1, PMOD_G2, PMOD_G3, PMOD_G4, ARDUINO_SEEED_D2,
 *     ARDUINO_SEEED_D3,ARDUINO_SEEED_D4, ARDUINO_SEEED_D5,
 *     ARDUINO_SEEED_D6, ARDUINO_SEEED_D7, ARDUINO_SEEED_D8
 * 
 * Returns
 * -------
 * grove_water
 *     The device object
 *     -ENOMEM if memory error
 * 
 */
grove_envsensor grove_envsensor_open(int grove_id);

/* Open a grove environment sensor device at a specified I2C address 
 * 
 * Parameters
 * ----------
 * grove_id: int
 *     Valid port ids for this device:
 *     PMOD_G1, PMOD_G2, PMOD_G3, PMOD_G4, ARDUINO_SEEED_D2,
 *     ARDUINO_SEEED_D3,ARDUINO_SEEED_D4, ARDUINO_SEEED_D5,
 *     ARDUINO_SEEED_D6, ARDUINO_SEEED_D7, ARDUINO_SEEED_D8
 * address: int
*      i2c address of the grove environemnt sensor
 * 
 * Returns
 * -------
 * grove_water
 *     The device object
 *     -ENOMEM if memory error
 * 
 */
grove_envsensor grove_envsensor_open_at_address(int grove_id, int address);

/* Release the grove barometer sensor by closing the device 
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     None
 *
 */
void grove_envsensor_close(grove_envsensor p);

// Device functions

/* Write register at given address for environement sensor
 * 
 * Parameters
 * ----------
 *     addr: char
 *     address if register to write
 *     val: char
 *     value to write to regiter
 * 
 * Returns
 * -------
 *     None
 *
 */
py_int grove_envsensor_reg_write(grove_envsensor p, unsigned char addr, unsigned char val);

/* Read all raw data from the sensor
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     0 = All data read into internal buffers
 *     1 = Failed to setup sensor
 *     2 = Failed to set sensor mode
 *     3 = Failed to read sensor data
 *
 */
py_int grove_envsensor_read_data(grove_envsensor p);

/* Read temperature value
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     temperature value: float
 *
 */
py_float grove_envsensor_read_temperature(grove_envsensor p);

/* Read pressure value
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     pressure value: float
 *
 */
py_float grove_envsensor_read_pressure(grove_envsensor p);

/* Read humidity value
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     humidity value: float
 *
 */
py_float grove_envsensor_read_humidity(grove_envsensor p);
py_float grove_envsensor_read_gas(grove_envsensor p);

/* Initilize grve environement sensor
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     1 = Sensor init done
 *     0 = Sensor init failed
 *
 */
py_int grove_envsensor_init(grove_envsensor p);



