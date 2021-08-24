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
#include <pytypes.h>
#include <sys/errno.h>

// Device typedef
/* Servo class
 *
 * Available Methods:
 *    open, close
 *    set_speed, forward, backward
 *    
 */
typedef py_int geared_motor;

// Device lifetime functions
/* Open a motor device connected to the specified port 
 * 
 * Parameters
 * ----------
 * grove_id: int
 *     Valid port ids for this device:
 *     PMOD_G1, PMOD_G2, PMOD_G3, PMOD_G4
 * 
 * Returns
 * -------
 * geared_motor
 *     The device object  
 * 
 */
geared_motor geared_motor_open(int grove_id);

/* Release the motor by closing the device 
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
void geared_motor_close(geared_motor p);

// Device functions
/* Set the speed of the motor
 * 
 * Parameters
 * ----------
 * value: int
 *     The speed percentage of the motor from 0 - 100
 * 
 * Returns
 * -------
 *     None
 *
 */
py_void geared_motor_set_speed(geared_motor p, int value);


// Device functions
/* Set the driection of the motor to forward
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
void geared_motor_forward(geared_motor p);

// Device functions
/* Set the driection of the motor to backward
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
void geared_motor_backward(geared_motor p);