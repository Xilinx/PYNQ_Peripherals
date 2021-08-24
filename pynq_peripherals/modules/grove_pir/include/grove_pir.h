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
#include <gpio.h>
#include <pytypes.h>

// Device typedef

/* Motion sensor Class
 *
 * Available Methods:
 *    open, close, motion
 *    
 */
typedef int grove_pir;

// Device lifetime functions

/* Open a Motion sensor device connected to the specified port 
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
 * grove_pir
 *      The device object  
 *      -ENOMEM memory allocation error (raises exception)
 * 
 */
grove_pir grove_pir_open(int grove_id);

/* Release the Motion sensor by closing the device 
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
void grove_pir_close(grove_pir pir);


// Device functions

/* Sense if motion is present
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Returns
 * -------
 *    True, if motion detected
 *    False, if motion not detected
 *
 */
py_bool grove_pir_motion_detected(grove_pir pir);
