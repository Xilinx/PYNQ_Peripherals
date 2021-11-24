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

#include <grove_interfaces.h>
#include <grove_water_level.h>
#include <math.h>
#include <pyprintf.h>
#include <xil_types.h>

#define I2C_ADDRESS_LO       0x77
#define I2C_ADDRESS_HI       0x78
#define THRESHOLD            100
#define DEVICE_MAX           4

struct info {
    i2c i2c_dev;
    unsigned char address;
    int count;
};

static struct info info[DEVICE_MAX];

/*
 * Documentation for public functions is provided as part of the external 
 * public header file.
 * Documentation for static/private functions is provided above each
 * function in this file.
 */ 

/* Get next instance space
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Return
 * ------
 * int
 *     The next index. Return -12 (ENOMEM, no memory) if errors on read.
 * 
 */
static int next_index() {
    for (int i = 0; i < DEVICE_MAX; ++i) {
        if (info[i].count == 0) return i;
    }
    return -ENOMEM; 
}

/* Get the water level value
 * 
 * Parameters
 * ----------
 *     None
 * 
 * Return
 * ------
 * float
 *     The water level value in centimeter (the resolution is 0.5cm)
 * 
 */
static float read_water_level(i2c device){
    u8 data_buffer_lo[8];
    u8 data_buffer_hi[12];
    
    data_buffer_lo[0] = 0;
    if(i2c_write(device, I2C_ADDRESS_LO, data_buffer_lo, 1) != 1) return -EIO;
    if(i2c_read(device, I2C_ADDRESS_LO, data_buffer_lo, 8) != 8) return -EIO;
    
    data_buffer_hi[0] = 0;
    if(i2c_write(device, I2C_ADDRESS_HI, data_buffer_hi, 1) != 1) return -EIO;
    if(i2c_read(device, I2C_ADDRESS_HI, data_buffer_hi, 12) != 12) return -EIO;
    
    int i;
    for (i = 0; i < 8; i ++)
    {
        if (data_buffer_lo[i] < THRESHOLD)
        {
            return i * 0.5;
        }
    }
    for (i = 0; i < 12; i ++)
    {
        if (data_buffer_hi[i] < THRESHOLD)
        {
            return (i + 8) * 0.5;
        }
    }
    return 20 * 0.5;
}
        
grove_water_level grove_water_level_open(int grove_id) {
    return grove_water_level_open_at_address(grove_id, I2C_ADDRESS_LO);
}

grove_water_level grove_water_level_open_at_address(int grove_id, int address) {
    grove_water_level dev_id = next_index();
    if (dev_id >=0 ) {
        info[dev_id].count++;
        info[dev_id].i2c_dev = i2c_open_grove(grove_id);
        info[dev_id].address = address;
    }
    return dev_id;
}

void grove_water_level_close(grove_water_level water_level) {
    if (--info[water_level].count != 0) return;
    i2c i2c_dev = info[water_level].i2c_dev;
    i2c_close(i2c_dev);
}

py_float grove_water_level_get_level(grove_water_level water_level) {
    return read_water_level(info[water_level].i2c_dev);
}
