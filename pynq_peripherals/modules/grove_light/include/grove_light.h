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
#include <grove_adc.h>
#include <pytypes.h>
#include <sys/errno.h>

// Device typedef

/* Light class
 *
 * Available Methods:
 *    open, close, read
 *       read returns light intensity in percentage
 *    
 */
typedef py_int grove_light;

// Device lifetime functions
/* Open a Light module connected directly to one of Arduino analog ports
 * or via a Grove ADC module (at default address) to an I2C port 
 * 
 * Parameters
 * ----------
 * grove_id: int
 *     Valid port ids for this device:
 *     ARDUINO_SEEED_A0, ARDUINO_SEEED_A1, ARDUINO_SEEED_A2, ARDUINO_SEEED_A3
 * 
 * Returns
 * -------
 * grove_temperature
 *     The device object if successful
 *     -EIO device not present (raises exception)
 *     -ENOMEM memory allocation error (raises exception)
 * 
 */
grove_light grove_light_open(int grove_id);

/* Open a light module connected to Grove_ADC module 
 * 
 * Parameters
 * ----------
 * grove_id: int
 *     Valid port ids for this device:
 *     PMOD_G3, PMOD_G4, ARDUINO_SEEED_I2C 
 * address: int
 *     I2C address of the ADC
 * 
 * Returns
 * -------
 * grove_temperature
 *     The device object if successful
 *     -EIO device not present (raises exception)
 *     -ENOMEM memory allocation error (raises exception)
 * 
 */
grove_light grove_light_open_at_address(int grove_id, int address);

/* Release the light module by closing the device 
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
void grove_light_close(grove_light light);

// Device functions
/* Read the connected device and return the light intensity in percentage 
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *   float:
 *     light intensity in percentage
 *     NAN  general operation error (raises exception)
 *
 */
py_float grove_light_get_intensity(grove_light light);
