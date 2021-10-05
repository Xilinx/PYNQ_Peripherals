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
#include <grove_constants.h>
#include <pytypes.h>
#include <sys/errno.h>

// Device typedef 
/* grove_barometer class 
 *
 * Available Methods:
 *    open, open_at_address,close,configure,
 *    start_conversion, set oversample rate, set mode, read temperature raw value
 *    read pressure raw value, read temerature, read pressure, read registers
 *    
 */
typedef py_int grove_barometer;

// Device lifetime functions
/* Open a grove barometer sensor device connected to the specified port 
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
grove_barometer grove_barometer_open(py_int grove_id);

/* Open a grove barometer sensor device connected to the specified I2C address 
 * 
 * Parameters
 * ----------
 * grove_id: int
 *     Valid port ids for this device:
 *     PMOD_G1, PMOD_G2, PMOD_G3, PMOD_G4, ARDUINO_SEEED_D2,
 *     ARDUINO_SEEED_D3,ARDUINO_SEEED_D4, ARDUINO_SEEED_D5,
 *     ARDUINO_SEEED_D6, ARDUINO_SEEED_D7, ARDUINO_SEEED_D8
 * address: int
 *     valid i2c adress for the device
 * 
 * Returns
 * -------
 * grove_water
 *     The device object
 *     -ENOMEM if memory error
 * 
 */
grove_barometer grove_barometer_open_at_address(py_int grove_id, py_int address);

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
void grove_barometer_close(grove_barometer p);

// Device functions
/* Configure grove barometer sensor
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     0 if configured
 *     ERROR Code if configuration fails 
 *
 */
py_int grove_barometer_configure(grove_barometer p);

/* Start conversion mesurement for grove barometer sensor
 * 
 * Parameters
 * ----------
 *     measurement_type: int
 *     specify the measurment type to perform:
 *        1 = pressure, 2 = temperature, 5 = continuous pressure
 *        6 = continuous temprature, 7 = continuous pressure and temperature 
 * 
 * Returns
 * -------
 *     0 for no error 
 *
 */
py_int grove_barometer_start_conversion(grove_barometer p, int measurement_type);


/* Set oversample rate for pressure readings to be used by grove barometer sensor
 * 
 * Parameters
 * ----------
 *     value: int
 *     oversample rate value
 * 
 * Returns
 * -------
 *     None 
 *
 */
py_void grove_barometer_pressure_oversample_rate(grove_barometer p, py_int value);

/* Set oversample rate for temperature readings to be used by grove barometer sensor
 * 
 * Parameters
 * ----------
 *     value: int
 *     oversample rate value
 * 
 * Returns
 * -------
 *     None 
 *
 */
py_void grove_barometer_temperature_oversample_rate(grove_barometer p, py_int value);

/* Set measurement rate for pressure readings to be used by grove barometer sensor
 * 
 * Parameters
 * ----------
 *     value: int
 *     measurement rate value
 * 
 * Returns
 * -------
 *     None 
 *
 */
py_void grove_barometer_pressure_measurement_rate(grove_barometer p, py_int value);

/* Set measurement rate for temperature readings to be used by grove barometer sensor
 * 
 * Parameters
 * ----------
 *     value: int
 *     measurement rate value
 * 
 * Returns
 * -------
 *     None 
 *
 */
py_void grove_barometer_temperature_measurement_rate(grove_barometer p, py_int value);

/* Set measurement mode for grove barometer sensor
 * 
 * Parameters
 * ----------
 *     value: int
 *     mesurement mode
 *        1 = pressure, 2 = temperature, 5 = continuous pressure
 *        6 = continuous temprature, 7 = continuous pressure and temperature 
 * 
 * Returns
 * -------
 *     None 
 *
 */
py_void grove_barometer_mode(grove_barometer p, py_int value);

/* Read raw temerature value from grove barometer sensor
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     raw temperature value : int 
 *
 */
py_int grove_barometer_temperature_raw(grove_barometer p);

/* Read raw pressure value from grove barometer sensor
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     raw pressure value : int 
 *
 */
py_int grove_barometer_pressure_raw(grove_barometer p);

/* Read coverted and normalized temperature value from grove barometer sensor
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     converted temperature value : int 
 *
 */
py_float grove_barometer_temperature(grove_barometer p);

/* Read coverted and normalized pressure value from grove barometer sensor
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     converted pressure value : int 
 *
 */
py_float grove_barometer_pressure(grove_barometer p);

/* Read mesurement fifo empty status from grove barometer sensor
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     empty flag : int
 *     0 = not empty
 *     1 = empty 
 *
 */
py_int grove_barometer_fifo_empty(grove_barometer p);

/* Read mesurement fifo full status from grove barometer sensor
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     full flag : int
 *     0 = not full
 *     1 = full 
 *
 */
py_int grove_barometer_fifo_full(grove_barometer p);

/* Enable internal fifo to store measurement values from grove barometer sensor
 * 
 * Parameters
 * ----------
 *     value: int
 *     1 = enable fifo, 0 = disable fifo 
 * 
 * Returns
 * -------
 *     None 
 *
 */
py_int grove_barometer_enable_fifo(grove_barometer p, py_int value);

/* Read top measurment avlue from internal of grove barometer sensor
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     value : int 
 *
 */
py_int grove_barometer_read_fifo(grove_barometer p);

/* Reset grove barometer sensor
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
py_int grove_barometer_reset(grove_barometer p);

/* Read grove barometer sensor status register
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *     status register value 
 *
 */
py_int grove_barometer_status(grove_barometer p);

/* Read register at given address for barometer sensor
 * 
 * Parameters
 * ----------
 *     addr: char
 *     address if register to read
 * 
 * Returns
 * -------
 *     status register value 
 *
 */
py_int grove_barometer_reg_read(grove_barometer p, unsigned char addr);

/* Write register at given address for barometer sensor
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
py_int grove_barometer_reg_write(grove_barometer p, unsigned char addr, unsigned char val);

/* Calculate pressure from a given raw temperature and raw pressure value
 * 
 * Parameters
 * ----------
 *     raw_temp: int
 *     raw temperature value to use
 *     raw_press: int
 *     raw temperature value to use
 * 
 * Returns
 * -------
 *     pressure value: float
 *
 */
py_float grove_barometer_calculate_pressure(grove_barometer p, int raw_temp, int raw_press);

/* Calculate temperature from a given raw temperature and raw pressure value
 * 
 * Parameters
 * ----------
 *     raw_temp: int
 *     raw temperature value to use
 *     raw_press: int
 *     raw temperature value to use
 * 
 * Returns
 * -------
 *     temperature value: float
 *
 */
py_float grove_barometer_calculate_temperature(grove_barometer p, int raw_temp, int raw_press);
