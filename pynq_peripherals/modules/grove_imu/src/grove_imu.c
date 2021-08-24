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
#include <grove_imu.h>
#include "circular_buffer.h"
#include "timer.h"
#include "i2c.h"
#include <stdbool.h>

#include "grove_imu_hw.h"

struct grove_imu_info {
    i2c i2c_dev;
    int count;
    int16_t ax, ay, az, gx, gy, gz, mx, my, mz;
    int t_fine;
    uint16_t dig_T1, dig_P1;
    int16_t dig_T2, dig_T3, dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;  
};

static uint8_t mpuAddr, bmpAddr;
static uint8_t buffer[14];

static struct grove_imu_info info[DEVICE_MAX];

static int next_index() {
    for (int i = 0; i < DEVICE_MAX; ++i) {
        if (info[i].count == 0) return i;
    }
    return -ENOMEM;
}

static int set_default_mpu_config(grove_imu imu);
static int set_default_bmp_config(grove_imu imu);

/*
 * Documentation for public functions is provided as part of the external 
 * public header file.
 * Documentation for static/private functions is provided above each
 * function in this file.
 */ 

grove_imu grove_imu_open(int grove_id) { 
    grove_imu dev_id = next_index();
    if (dev_id == -ENOMEM)
        return -ENOMEM;
    grove_imu lcl_err;
    info[dev_id].count++;
    info[dev_id].i2c_dev = i2c_open_grove(grove_id);
    
    if ((lcl_err = set_default_mpu_config(dev_id)) < PY_SUCCESS) {
        info[dev_id].count--;
        i2c_close(info[dev_id].i2c_dev);
        return lcl_err;
    }
    if ((lcl_err = set_default_bmp_config(dev_id)) < PY_SUCCESS) {
        info[dev_id].count--;
        i2c_close(info[dev_id].i2c_dev);
        return lcl_err;
    }
    return dev_id;
}

void grove_imu_close(grove_imu imu) {
    grove_imu_reset(imu);
    if (--info[imu].count != 0) return;
    i2c i2c_dev = info[imu].i2c_dev;
    i2c_close(i2c_dev);
}

/* Read data in bytes form register
 * 
 * Parameters
 * ----------
 * devAddr: uint8_t
 *     Device address.
 * regAddr: uint8_t
 *     Register address to read.
 * length: uint8_t
 *     Number of bytes to read.
 * data: uint8_t*
 *     Array pointer to store the return value.
 * 
 * Return
 * ------
 * int
 *     The length of read data in bytes. Return -1 if errors on read.
 * 
 */
static int i2c_readBytes(grove_imu imu, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data){
    i2c i2c_dev = info[imu].i2c_dev;
    if (i2c_write(i2c_dev, devAddr, &regAddr, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, devAddr, data, length) != length) return -EIO;
    return length;
}

/* Read a byte from register
 * 
 * Parameters
 * ----------
 * devAddr: uint8_t
 *     Device address.
 * regAddr: uint8_t
 *     Register address to read.
 * data: uint8_t*
 *     Array pointer to store the return value.
 * 
 * Return
 * ------
 * int
 *     The length of read data in bytes. Return -1 if errors on read.
 * 
 */
static int i2c_readByte(grove_imu imu, uint8_t devAddr, uint8_t regAddr, uint8_t *data){
    i2c i2c_dev = info[imu].i2c_dev;
    if (i2c_write(i2c_dev, devAddr, &regAddr, 1) != 1) return -EIO;
    if (i2c_read(i2c_dev, devAddr, data, 1) != 1) return -EIO;
    return 1;
}

/* Write data in bytes to register
 * 
 * Parameters
 * ----------
 * devAddr: uint8_t
 *     Device address.
 * regAddr: uint8_t
 *     Register address to write.
 * length: uint8_t
 *     Number of bytes to write.
 * data: uint8_t*
 *     Array pointer that stores the write value.
 * 
*/
static int i2c_writeBytes(grove_imu imu, uint8_t devAddr, uint8_t regAddr, uint8_t length, uint8_t *data){
    i2c i2c_dev = info[imu].i2c_dev;
    int i;
    int len_total = (int)length+1;
    uint8_t temp[len_total];
    temp[0] = regAddr;
    for (i=1;i<len_total;i++){
        temp[i]=data[i-1];
    }
    if (i2c_write(i2c_dev, devAddr, temp, len_total) != len_total) return -EIO;
    return PY_SUCCESS;
}

/* Write a byte to register
 * 
 * Parameters
 * ----------
 * devAddr: uint8_t
 *     Device address.
 * regAddr: uint8_t
 *     Register address to write.
 * data: uint8_t*
 *     Array pointer that stores the write value.
 * 
 */
static int i2c_writeByte(grove_imu imu, uint8_t devAddr, uint8_t regAddr, uint8_t *data){
    i2c i2c_dev = info[imu].i2c_dev;
    uint8_t temp[2];
    temp[0] = regAddr;
    temp[1] = *data;
    if (i2c_write(i2c_dev, devAddr, temp, 2) != 2) return -EIO;
    return PY_SUCCESS;
}

/* Read bits of data from register
 * 
 * Return specified bits with specified position and length
 * 
 * Parameters
 * ----------
 * devAddr: uint8_t
 *     Device address.
 * regAddr: uint8_t 
 *     register address to read.
 * bitStart: uint8_t
 *     Start index of bits to read.
 * width: uint8_t 
 *     Width of bits to read.
 * data: uint8_t*
 *     Array pointer to store the return value.
 * 
 * Return
 * ------
 * int
 *     Specified bits.
 * 
 */
static int i2c_readBits(grove_imu imu, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t width, uint8_t *data) {
    i2c i2c_dev = info[imu].i2c_dev;
    uint8_t b;
    uint8_t mask;
    int count = i2c_readBytes(imu, devAddr, regAddr, 1, &b);
    if (count == -1) {
        return -EIO;
    }
    if (count != 0) {
        mask = ((1 << width) - 1) << (bitStart - width + 1);
        b &= mask;
        b >>= (bitStart - width + 1);
        *data = b;
    }
    return count;
}

/* Read one Specified bit from register
 * 
 * Parameters
 * ----------
 * devAddr: uint8_t
 *     Device address.
 * regAddr: uint8_t
 *     Register address to read.
 * bitStart: uint8_t
 *     Index to read.
 * data: uint8_t*
 *     Array pointer to store the return value.
 * 
 * Return
 * ------
 * int
 *     The specified bit
 * 
 */
static int i2c_readBit(grove_imu imu, uint8_t devAddr, uint8_t regAddr, 
                   uint8_t bitStart, uint8_t *data) {
    return i2c_readBits(imu, devAddr, regAddr, bitStart, (uint8_t) 1, data);
}
    
/* Write specified bits to register
 * 
 * Write bits to register with specified positioin and length.
 * 
 * Parameters
 * ----------
 * devAddr: uint8_t
 *     Device address.
 * regAddr: uint8_t
 *     Register address to write.
 * bitStart: uint8_t
 *     Start index of bits to write.
 * width: uint8_t
 *     Width of bits to write.
 * data: uint8_t*
 *     Array pointer that stores the write value.
 * 
 */
static int i2c_writeBits(grove_imu imu, uint8_t devAddr, uint8_t regAddr, 
                     uint8_t bitStart, uint8_t width, uint8_t *data) {
    uint8_t b, temp;
    temp = *data;
    int count = i2c_readBytes(imu, devAddr, regAddr, 1, &b);
    if (count == -1) {
        return -EIO;
    }
    if (count != 0) {
        uint8_t mask = ((1 << width) - 1) << (bitStart - width + 1);
        // shift data into correct position
        temp <<= (bitStart - width + 1);
        // zero all non-important bits in data
        temp &= mask;
        // zero all important bits in existing byte
        b &= ~(mask);
        // combine data with existing byte
        b |= temp;
        if (i2c_writeByte(imu, devAddr, regAddr, &b) == -1) return -EIO;
    }
    return PY_SUCCESS;
}

/* Write one bit to register
 * 
 * Parameters
 * ----------
 * devAddr: uint8_t
 *     Device address.
 * regAddr: uint8_t
 *     Register address to write.
 * bitStart: uint8_t
 *     Index of bits to write.
 * data: uint8_t*
 *     Array pointer that stores the write value.
 * 
 */
static int i2c_writeBit(grove_imu imu, uint8_t devAddr, uint8_t regAddr, uint8_t bitStart, uint8_t *data) {
    return i2c_writeBits(imu, devAddr, regAddr, bitStart, (uint8_t) 1, data);
}

// MPU9250 driver functions

/* Set defalut configuration of IMU MPU
 * Selecting the gyros as the clock source, set gyroscope full scale range
 * as ±250°/sec, set accelerometer full scale range as ±2g, and disable 
 * sleep mode.
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      0 if default configuration set successfully
 *      -EIO device not present or IO error (raises exception)
 */
static int set_default_mpu_config(grove_imu imu) {
    mpuAddr = MPU9250_DEFAULT_ADDRESS;
    bmpAddr = BMP280_ADDRESS;
    if (grove_imu_set_clock_source(imu, MPU9250_CLOCK_PLL_XGYRO) == -EIO) return -EIO;
    if (grove_imu_set_full_scale_gyro_range(imu, MPU9250_GYRO_FS_250) == -EIO) return -EIO;
    if (grove_imu_set_full_scale_accel_range(imu, MPU9250_ACCEL_FS_2) == -EIO) return -EIO;
    if (grove_imu_set_sleep_mode(imu, false) == -EIO) return -EIO;
    return PY_SUCCESS;
}

py_void grove_imu_set_clock_source(grove_imu imu, uint8_t source) {
    return i2c_writeBits(imu, mpuAddr, MPU9250_RA_PWR_MGMT_1, MPU9250_PWR1_CLKSEL_BIT, 
                    MPU9250_PWR1_CLKSEL_LENGTH, &source);
}

py_void grove_imu_set_full_scale_gyro_range(grove_imu imu, uint8_t range) {
    return i2c_writeBits(imu, mpuAddr, MPU9250_RA_GYRO_CONFIG, MPU9250_GCONFIG_FS_SEL_BIT, 
                    MPU9250_GCONFIG_FS_SEL_LENGTH, &range);
}

py_void grove_imu_set_full_scale_accel_range(grove_imu imu, uint8_t range) {
    return i2c_writeBits(imu, mpuAddr, MPU9250_RA_ACCEL_CONFIG, 
                    MPU9250_ACONFIG_AFS_SEL_BIT, 
                    MPU9250_ACONFIG_AFS_SEL_LENGTH, &range);
}

py_void grove_imu_reset(grove_imu imu) {
    uint8_t data = 0x01;
    return i2c_writeBit(imu, mpuAddr, MPU9250_RA_PWR_MGMT_1, 
                    MPU9250_PWR1_DEVICE_RESET_BIT, &data);
}

py_void grove_imu_set_sleep_mode(grove_imu imu, uint8_t enabled) {
    return i2c_writeBit(imu, mpuAddr, MPU9250_RA_PWR_MGMT_1, 
                    MPU9250_PWR1_SLEEP_BIT, &enabled);
}

py_void grove_imu_fetch_motion9(grove_imu imu) {
    //get accel and gyro
    if (i2c_readBytes(imu, mpuAddr, MPU9250_RA_ACCEL_XOUT_H, 14, buffer) == -EIO) return -EIO;
    delay_ms(60);
    info[imu].ax = (((int16_t)buffer[0]) << 8) | buffer[1];
    info[imu].ay = (((int16_t)buffer[2]) << 8) | buffer[3];
    info[imu].az = (((int16_t)buffer[4]) << 8) | buffer[5];
    info[imu].gx = (((int16_t)buffer[8]) << 8) | buffer[9];
    info[imu].gy = (((int16_t)buffer[10]) << 8) | buffer[11];
    info[imu].gz = (((int16_t)buffer[12]) << 8) | buffer[13];
    
    //read mag
    uint8_t data;
    data = 0x02;
    //set i2c bypass enable pin to access magnetometer
    if (i2c_writeByte(imu, mpuAddr, MPU9250_RA_INT_PIN_CFG, &data) == -EIO) return -EIO;
    delay_ms(10);
    data = 0x01;
    //enable the magnetometer
    if (i2c_writeByte(imu, MPU9150_RA_MAG_ADDRESS, 0x0A, &data) == -EIO) return -EIO;
    delay_ms(10);
    if (i2c_readBytes(imu, MPU9150_RA_MAG_ADDRESS, MPU9150_RA_MAG_XOUT_L, 6, buffer) == -EIO) return -EIO;
    delay_ms(60);
    info[imu].mx = (((int16_t)buffer[1]) << 8) | buffer[0];
    info[imu].my = (((int16_t)buffer[3]) << 8) | buffer[2];
    info[imu].mz = (((int16_t)buffer[5]) << 8) | buffer[4];
    return PY_SUCCESS;
}

py_float grove_imu_get_accel_x(grove_imu imu) {
    float v = (float)info[imu].ax/16384;
    return v;
}

py_float grove_imu_get_accel_y(grove_imu imu) {
    float v = (float)info[imu].ay/16384;
    return v;
}

py_float grove_imu_get_accel_z(grove_imu imu) {
    float v = (float)info[imu].az/16384;
    return v;
}

py_float grove_imu_get_gyro_x(grove_imu imu) {
    float v = (float)info[imu].gx*250/32768;
    return v;
}

py_float grove_imu_get_gyro_y(grove_imu imu) {
    float v = (float)info[imu].gy*250/32768;
    return v;
}

py_float grove_imu_get_gyro_z(grove_imu imu) {
    float v = (float)info[imu].gz*250/32768;
    return v;
}

py_float grove_imu_get_magneto_x(grove_imu imu) {
    float v = (float)info[imu].mx*1200/4096;
    return v;
}

py_float grove_imu_get_magneto_y(grove_imu imu) {
    float v = (float)info[imu].my*1200/4096;
    return v;
}

py_float grove_imu_get_magneto_z(grove_imu imu) {
    float v = (float)info[imu].mz*1200/4096;
    return v;
}

// BMP280 functions

/* Read a 16-bit unsigned value from BMP280
 * 
 * Parameters
 * ---------
 * regAddr: uint8_t
 *     Register address to read.
 * 
 * Return
 * ------
 * int
 *     16-bit unsigned value.
 * 
 */
static int bmp_read_u16(grove_imu imu, uint8_t regAddr) {
    uint8_t lsb, msb;
    if (i2c_readByte(imu, BMP280_ADDRESS, regAddr, &lsb) == -EIO) return -EIO;
    if (i2c_readByte(imu, BMP280_ADDRESS, regAddr + 1, &msb) == -EIO) return -EIO;
    return (int)(((uint16_t)msb << 8) | (uint16_t)lsb);
}

/* Read a 16-bit signed value from BMP280
 * 
 * Parameters
 * ---------
 * regAddr: uint8_t
 *     Register address to read.
 * 
 * Return
 * ------
 * int
 *     16-bit signed value.
 * 
 */
static int bmp_read_s16(grove_imu imu, uint8_t regAddr) {
    uint8_t lsb, msb;
    if (i2c_readByte(imu, BMP280_ADDRESS, regAddr, &lsb) == -EIO) return -EIO;
    if (i2c_readByte(imu, BMP280_ADDRESS, regAddr + 1, &msb) == -EIO) return -EIO;
    return (int)((int16_t)(((uint16_t)msb << 8) | (uint16_t)lsb));
}

/* Set defalut configuration of IMU BMP
 * Read the compensation value and enable the divice. 
 *
 * Parameters
 * ----------
 *      None
 *
 * Returns
 * -------
 *      0 if default configuration set successfully
 *      -EIO device not present or IO error (raises exception)
 */
static int set_default_bmp_config(grove_imu imu) {
    i2c i2c_dev = info[imu].i2c_dev;
    uint8_t data_chipid;
    if (i2c_readByte(imu, BMP280_ADDRESS, BMP280_REG_CHIPID, &data_chipid) == -EIO) return -EIO;
    if(data_chipid != 0x58) {
        return -EIO;
    }

    info[imu].dig_T1 = (uint16_t)bmp_read_u16(imu, BMP280_REG_DIG_T1);
    info[imu].dig_T2 = (int16_t)bmp_read_s16(imu, BMP280_REG_DIG_T2);
    info[imu].dig_T3 = (int16_t)bmp_read_s16(imu, BMP280_REG_DIG_T3);

    info[imu].dig_P1 = (uint16_t)bmp_read_u16(imu, BMP280_REG_DIG_P1);
    info[imu].dig_P2 = (int16_t)bmp_read_s16(imu, BMP280_REG_DIG_P2);
    info[imu].dig_P3 = (int16_t)bmp_read_s16(imu, BMP280_REG_DIG_P3);
    info[imu].dig_P4 = (int16_t)bmp_read_s16(imu, BMP280_REG_DIG_P4);
    info[imu].dig_P5 = (int16_t)bmp_read_s16(imu, BMP280_REG_DIG_P5);
    info[imu].dig_P6 = (int16_t)bmp_read_s16(imu, BMP280_REG_DIG_P6);
    info[imu].dig_P7 = (int16_t)bmp_read_s16(imu, BMP280_REG_DIG_P7);
    info[imu].dig_P8 = (int16_t)bmp_read_s16(imu, BMP280_REG_DIG_P8);
    info[imu].dig_P9 = (int16_t)bmp_read_s16(imu, BMP280_REG_DIG_P9);

    uint8_t ctrl_data = 0xFF;
    if (i2c_writeByte(imu, BMP280_ADDRESS, BMP280_REG_CONTROL, &ctrl_data) == -EIO) return -EIO;

    delay_ms(100);
    return PY_SUCCESS;
}

py_float grove_imu_get_temperature(grove_imu imu) {
    int var1, var2;
    int adc_T;
    uint8_t msb, lsb, xlsb;
    if (i2c_readByte(imu, BMP280_ADDRESS, BMP280_REG_TEMPDATA, &msb) == -EIO) return PY_FLOAT_ERROR;
    if (i2c_readByte(imu, BMP280_ADDRESS, BMP280_REG_TEMPDATA + 1, &lsb) == -EIO) return PY_FLOAT_ERROR;
    if (i2c_readByte(imu, BMP280_ADDRESS, BMP280_REG_TEMPDATA + 2, &xlsb) == -EIO) return -PY_FLOAT_ERROR;
    adc_T = (int)(((uint32_t)msb << 16) | ((uint32_t)lsb << 8) | (uint32_t)xlsb);
    adc_T >>= 4;
    var1 = (((adc_T >> 3) - ((int)(info[imu].dig_T1 << 1))) * ((int)info[imu].dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int)info[imu].dig_T1)) * ((adc_T >> 4) - ((int)info[imu].dig_T1))) >> 12) * ((int)info[imu].dig_T3)) >> 14;
    info[imu].t_fine = var1 + var2;
    float T = (info[imu].t_fine * 5 + 128) >> 8;
    return T/100;
}

py_int grove_imu_get_pressure(grove_imu imu) {
    int64_t var1, var2, pressure;

    grove_imu_get_temperature(imu);

    int adc_P;
    uint8_t msb, lsb, xlsb;
    if (i2c_readByte(imu, BMP280_ADDRESS, BMP280_REG_PRESSUREDATA, &msb) == -EIO) return -EIO;
    if (i2c_readByte(imu, BMP280_ADDRESS, BMP280_REG_PRESSUREDATA + 1, &lsb) == -EIO) return -EIO;
    if (i2c_readByte(imu, BMP280_ADDRESS, BMP280_REG_PRESSUREDATA + 2, &xlsb) == -EIO) return -EIO;
    adc_P = (int)(((uint32_t)msb << 16) | ((uint32_t)lsb << 8) | (uint32_t)xlsb);
    adc_P >>= 4;
    var1 = ((int64_t)info[imu].t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)info[imu].dig_P6;
    var2 = var2 + ((var1*(int64_t)info[imu].dig_P5)<<17);
    var2 = var2 + (((int64_t)info[imu].dig_P4)<<35);
    var1 = ((var1 * var1 * (int64_t)info[imu].dig_P3)>>8) + ((var1 * (int64_t)info[imu].dig_P2)<<12);
    var1 = (((((int64_t)1)<<47)+var1))*((int64_t)info[imu].dig_P1)>>33;
    if (var1 == 0) {
        return 0; // avoid exception caused by division by zero
    }
    pressure = 1048576-adc_P;
    pressure = (((pressure<<31)-var2)*3125)/var1;
    var1 = (((int64_t)info[imu].dig_P9) * (pressure>>13) * (pressure>>13)) >> 25;
    var2 = (((int64_t)info[imu].dig_P8) * pressure) >> 19;
    pressure = ((pressure + var1 + var2) >> 8) + (((int64_t)info[imu].dig_P7)<<4);
    return (int)pressure / 256;
}
