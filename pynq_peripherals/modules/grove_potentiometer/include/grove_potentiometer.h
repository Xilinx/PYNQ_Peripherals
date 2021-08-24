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

// Device typedef

/* Potentiometer Class
 *
 * Available Methods:
 *      open, close
 *
 *      get_position
 */

typedef int grove_potentiometer;

// Device lifetime functions

/* Open a Potentiometer module connected directly to one of Arduino analog ports
 * or via a Grove ADC module (at default address) to an I2C port 
 * 
 * Parameters
 * ----------
 * grove_id: int
 *     Valid port ids for this device:
 *     ARDUINO_SEEED_A0-A3 (analog)
 *     PMOD_G3, PMOD_G4, ARDUINO_SEEED_I2C (via I2C ADC)
 * 
 * Returns
 * -------
 * grove_potentiometer
 *      The device object  
 *      -EIO device not present or IO error (raises exception)
 *      -ENOMEM memory allocation error (raises exception)
 * 
 */
grove_potentiometer grove_potentiometer_open(int grove_id);

/* Open a Potentiometer module connected to the specified port with the default 
 * I2C address
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
 * grove_potentiometer
 *      The device object  
 *      -EIO device not present or IO error (raises exception)
 *      -ENOMEM memory allocation error (raises exception)
 * 
 */
grove_potentiometer grove_potentiometer_open_at_address(int grove_id, int address);

/* Release the Potentiometer by closing the device 
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
void grove_potentiometer_close(grove_potentiometer potentiometer);


// Device functions

/* Read the ADC and return the linear or angular position as fraction of full displacement
 *
 * Parameters
 * ----------
 *     None
 *
 * Returns
 * -------
 *   float:
 *     Linear or angular position as fraction of full displacement (between 0.0 and 1.0)
 *     NAN  general operation error (raises exception)
 *
 */
py_float grove_potentiometer_get_position(grove_potentiometer potentiometer);
