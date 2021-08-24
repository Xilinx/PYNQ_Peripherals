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
#include <grove_adc.h>
#include <math.h>

#include <xil_types.h>

// VRef = Va measured on the board
#define V_REF 3.14

// ADC Registers
#define REG_ADDR_RESULT        0x00
#define REG_ADDR_CONFIG        0x02

/*
 * Documentation for public functions is provided as part of the external 
 * public header file.
 * Documentation for static/private functions is provided above each
 * function in this file.
 */ 

/* Read converted result
 * 
 * Parameters
 * ----------
 *
 * address: u8
 *     Device address.
 * reg: u8
 *     Register address to read.
 * 
 * Return
 * ------
 * int
 *     The read data. Return -1 (EIO) if errors on read.
 * 
 */
static int read_adc(i2c device, u8 address, u8 reg){
    u8 data_buffer[2];
    u32 sample;
    u8 num_bytes;

    data_buffer[0] = reg; // Set the address pointer register
    if(i2c_write(device, address, data_buffer, 1) != 1) return -EIO;

    if(i2c_read(device, address, data_buffer, 2) != 2) return -EIO;
    sample = ((data_buffer[0]&0x0f) << 8) | data_buffer[1];
    return sample;
}

/* Write command to a register
 * Maximum of 2 data bytes can be written in one transaction
 *
 * Parameters
 * ----------
 *
 * address: u8
 *     Device address.
 * reg: u8
 *     Register address to read.
 * data: u32
 *     Data to be written.
 * bytes: u8
 *     Number of bytes to be written.
 * 
 * Return
 * ------
 * int
 *     The number of bytes written. Return -1 (EIO) if errors on write.
 * 
 */
static int write_adc(i2c device, u8 address, u8 reg, u32 data, u8 bytes){
    u8 data_buffer[3];
    data_buffer[0] = reg;
    if(bytes ==2){
        data_buffer[1] = (data >> 8) & 0x0f; // Bits 11:8
        data_buffer[2] = data & 0xff; // Bits 7:0
    }else{
        data_buffer[1] = data & 0xff; // Bits 7:0
    }   
    if(i2c_write(device, address, data_buffer, bytes+1) != (bytes+1)) return -EIO;
    else return (bytes+1);

}

#define I2C_ADDRESS 0x50
#define DEVICE_MAX 4

struct info {
    i2c i2c_dev;
    unsigned char address;
    int count;
};

static struct info info[DEVICE_MAX];

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

grove_adc grove_adc_open(int grove_id) {
    return grove_adc_open_at_address(grove_id, I2C_ADDRESS);
}

grove_adc grove_adc_open_at_address(int grove_id, int address) {
    grove_adc dev_id = next_index();
    if (dev_id >=0 ) {
        info[dev_id].count++;
        info[dev_id].i2c_dev = i2c_open_grove(grove_id);
        info[dev_id].address = address;
        if(write_adc(info[dev_id].i2c_dev,info[dev_id].address, REG_ADDR_CONFIG,0x20,1) == -EIO){
            info[dev_id].count--;
            i2c_close(info[dev_id].i2c_dev);
            return -EIO;
        } 
    }
    return dev_id;
}

void grove_adc_close(grove_adc adc) {
    if (--info[adc].count != 0) return;
    i2c i2c_dev = info[adc].i2c_dev;
    i2c_close(i2c_dev);
}

py_int grove_adc_read_raw(grove_adc adc) {
    return read_adc(info[adc].i2c_dev, info[adc].address, REG_ADDR_RESULT);
}

py_float grove_adc_read(grove_adc adc) {
    int raw = grove_adc_read_raw(adc);
    if (raw == -1) return PY_FLOAT_ERROR; 
    return (raw * V_REF) * 2 / 4096; // Voltage divider
}