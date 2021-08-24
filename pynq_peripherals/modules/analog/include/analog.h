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

#include <pytypes.h>
#include <sys/errno.h>

typedef py_int analog;

// Device lifetime functions
/* Open a device providing an anlog signal to the System Monitor of 
 * the Zynq chip 
 * 
 * Parameters
 * ----------
 * pin_id: int
 *     Valid port ids for this device:
 *     ARDUINO_SEEED_A0, ARDUINO_SEEED_A1, ARDUINO_SEEED_A2, ARDUINO_SEEED_A3
 * 
 * Returns
 * -------
 * analog
 *     The device object if successful
 *     -ENXIO no such device (raises exception)
 * 
 */
analog analog_open_xadc(int pin_id);

/* Open a Grove ADC module  
 * 
 * Parameters
 * ----------
 * adc: int
 *     Valid port ids for this device:
 *     PMOD_G3, PMOD_G4, ARDUINO_SEEED_I2C 
 * 
 * Returns
 * -------
 * analog
 *     The device object if successful
 *     -EIO device not present (raises exception)
 * 
 */
analog analog_open_grove_adc(int adc);

/* Release the connected analog source by closing the device 
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
void analog_close(analog dev_id);

// Device functions
/* Read the analog signal and return voltage
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *   float:
 *     Voltage value 
 *     NAN  general operation error (raises exception)
 *
 */
py_float analog_get_voltage(analog dev_id);

/* Read the analog signal and return a 12-bit integer raw value
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *   integer:
 *     A 12-bit value from Grove ADC, 16-bit value from 
 *         System  Monittor if successful
 *     -EIO otherwise to raise exception
 */
py_int analog_get_raw(analog dev_id);

/* Read the reference voltage
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *   float:
 *     3.1V if Grove ADC, 3.3V if System Monitor
 *     NAN  general operation error (raises exception)
 *
 */
py_float analog_get_reference(analog dev_id);

/* Get the maximum value that the selected device can return
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *     A 12-bit or 16-bit integer value 
 *     -PY_INT_ERROR otherwise to raise exception
 */
py_int analog_get_max(analog dev_id);
