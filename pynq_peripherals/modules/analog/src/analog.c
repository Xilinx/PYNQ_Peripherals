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

#include <xparameters.h>

#ifdef XPAR_SYSMON_0_DEVICE_ID
#include <xsysmon.h>
static XSysMon SysMonInst;
static XSysMon_Config *SysMonConfigPtr;
static XSysMon *SysMonInstPtr = &SysMonInst;
static int sysmon_init = 0;
#endif


#include <analog.h>
#include <grove_adc.h>

#define XADC_TYPE 0l
#define GROVE_ADC_TYPE (1l << 24)
#define TYPE_MASK 0xF000000

analog analog_open_xadc(int pin_id) {
#ifdef XPAR_SYSMON_0_DEVICE_ID
    if (!sysmon_init) {
        SysMonConfigPtr = XSysMon_LookupConfig(XPAR_SYSMON_0_DEVICE_ID);
        XSysMon_CfgInitialize(SysMonInstPtr, SysMonConfigPtr,
            SysMonConfigPtr->BaseAddress);
        XSysMon_GetStatus(SysMonInstPtr);
        sysmon_init = 1;
    }
    return pin_id;
#else
    return -ENXIO;
#endif
}

analog analog_open_grove_adc(grove_adc adc) {
    return GROVE_ADC_TYPE | adc; 
}

void analog_close(analog dev_id) {

}

py_float analog_get_voltage(analog dev_id) {
   return analog_get_raw(dev_id) * analog_get_reference(dev_id) 
        / analog_get_max(dev_id);
}

py_int analog_get_raw(analog dev_id) {
    switch (dev_id & TYPE_MASK) {
    case XADC_TYPE:
#ifdef XPAR_SYSMON_0_DEVICE_ID
        while ((XSysMon_GetStatus(SysMonInstPtr) &
                        XSM_SR_EOS_MASK) != XSM_SR_EOS_MASK);
        return XSysMon_GetAdcData(SysMonInstPtr, XSM_CH_AUX_MIN+dev_id);
#else
        return -ENXIO;
#endif

    case GROVE_ADC_TYPE:
        return grove_adc_read_raw(dev_id ^ GROVE_ADC_TYPE);
    }
    return PY_SUCCESS;
}

py_float analog_get_reference(analog dev_id) {
    switch (dev_id & TYPE_MASK) {
    case XADC_TYPE:
        return 3.3f;
    case GROVE_ADC_TYPE:
        return 3.1f;
    }
    return PY_FLOAT_ERROR;
}

py_int analog_get_max(analog dev_id) {
    switch (dev_id & TYPE_MASK) {
    case XADC_TYPE:
        return 65536;
    case GROVE_ADC_TYPE:
        return 4096/2;  // Voltage divider 
    }
    return -ENXIO;
}

