#   Copyright (c) 2021, Xilinx, Inc.
#   All rights reserved.
# 
#   Redistribution and use in source and binary forms, with or without 
#   modification, are permitted provided that the following conditions are met:
#
#   1.  Redistributions of source code must retain the above copyright notice, 
#       this list of conditions and the following disclaimer.
#
#   2.  Redistributions in binary form must reproduce the above copyright 
#       notice, this list of conditions and the following disclaimer in the 
#       documentation and/or other materials provided with the distribution.
#
#   3.  Neither the name of the copyright holder nor the names of its 
#       contributors may be used to endorse or promote products derived from 
#       this software without specific prior written permission.
#
#   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
#   THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
#   PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
#   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
#   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
#   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#   OR BUSINESS INTERRUPTION). HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
#   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
#   OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
#   ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import os
import re
from pynq.lib import MicroblazeLibrary
from pynq.lib.pynqmicroblaze.compile import preprocess, checkmodule
from pynq.lib.pynqmicroblaze.bsp import add_module_path

class GroveAdapter:
    """This abstract class controls multiple Grove modules connected to a given adapter."""
    
    _module_re = re.compile('([^@]*)(?:@([a-fA-F0-9]+))?')
    def __init__(self, iop, port_prefix, **kwargs):
        """Create a new GroveAdapter object.

        Parameters
        ----------
        port_prefix : string
            One of "PMOD", "ARDUINO_SEEED", "ARDUINO_DIGILENT"
        args: list of ports

        """
        modules = set()
        for _, v in kwargs.items():
            if v:
                if type(v) is str:
                    modules.update(self._get_modules(v))
                else:
                    for spec in v:
                        modules.update(self._get_modules(spec))
        for mod in modules:
            if not checkmodule(mod, iop):
                raise RuntimeError(f"Module {mod} not found")
        if 'i2c' in modules or 'gpio' in modules or 'analog' in modules:
            modules.add('grove_interfaces')
        self._lib = MicroblazeLibrary(iop, modules)
        for k, v in kwargs.items():
            if v is None:
                continue
            port = getattr(self._lib, f'{port_prefix}_{k}')
            try:
                if type(v) is str:
                    setattr(self, k, self._instantiate_device(v, port))
                else:
                    setattr(self, k,
                           [self._instantiate_device(s, port) for s in v])
            except Exception as exc:
                raise RuntimeError(f"Failed to initialise port {k}") from exc
        
    def _module_basename(self, name):
        return self._module_re.match(name)[1]
        
    def _get_modules(self, spec):
        return [self._module_basename(p) for p in spec.split('.')]
    
    def _open_device(self, device, port):
        details = self._module_re.match(device)
        name = details[1]
        if details[2]:
            if not hasattr(self._lib, f'{name}_open_at_address'):
                raise RuntimeError(
                    f"Module {name} does not support opening at an address")
            return getattr(self._lib, f'{name}_open_at_address')(
                    port, int(details[2], 16))
        else:
            if name == 'i2c':
                return self._lib.i2c_open_grove(port)
            elif name == 'gpio':
                return self._lib.gpio_open_grove(port)
            elif name == 'analog':
                return self._lib.analog_open_grove(port)
            else:
                return getattr(self._lib, f'{name}_open')(port)
    
    def _instantiate_device(self, spec, port):
        chain = spec.split('.')
        if len(chain) == 1:
                return self._open_device(chain[0], port)
        else:
            if self._module_basename(chain[0]) == 'grove_adc':
                adc = self._open_device(chain[0], port)
                if not hasattr(self._lib, f'{chain[1]}_open_adc'):
                    raise RuntimeError(
                            f'{chain[1]} should not be connected to an ADC')
                return getattr(self._lib, f'{chain[1]}_open_adc')(adc)
            else:
                raise RuntimeError("Only grove_adc can be used in a chain")


class PmodGroveAdapter(GroveAdapter):
    """This class controls multiple Grove modules connected to a PMOD adapter."""

    def __init__(self, iop, G1=None, G2=None, G3=None, G4=None):
        """Create a new PmodGroveAdapter object.

        Parameters
        ----------
        iop : name of IOP for adapter
              e.g. base.PMODA, base.PMODB
        port args: ports by name either G1, G2, G3, G4
        
                   Available names of Grove modules for port args:
                   'grove_adc', 'grove_buzzer', 'grove_gesture', 'grove_imu',
                   'grove_joystick', 'grove_led_stick', 'grove_ledbar', 
                   'grove_lgcp', 'grove_light', 'grove_line_finder', 
                   'grove_oled', 'grove_pir', 'grove_potentiometer', 
                   'grove_relay', 'grove_servo', 'grove_temperature', 
                   'grove_usranger', 'grove_water_sensor'
        
        """        
        super().__init__(iop, 'PMOD', G1=G1, G2=G2, G3=G3, G4=G4)

class ArduinoSEEEDGroveAdapter(GroveAdapter):
    """This class controls multiple Grove modules connected to a ArduinoSEEED adapter."""

    def __init__(self, iop, UART=None, D2=None, D3=None, 
                 D4=None, D5=None, D6=None, D7=None,
                 D8=None, A0=None, A1=None, A2=None,
                 A3=None, I2C=None):
        """Create a new ArduinoSEEEDGroveAdapter object.

        Parameters
        ----------
        iop : name of IOP for adapter
              e.g. base.ARDUINO
        port args: ports by name either UART, D2, D3, D4, D6, D7, D8
                   A0, A1, A2, A3, I2C (can be a list)
                   
                   Available names of Grove modules for port args:
                   'grove_adc', 'grove_buzzer', 'grove_gesture', 'grove_imu',
                   'grove_joystick', 'grove_led_stick', 'grove_ledbar', 
                   'grove_lgcp', 'grove_light', 'grove_line_finder', 
                   'grove_oled', 'grove_pir', 'grove_potentiometer', 
                   'grove_relay', 'grove_servo', 'grove_temperature', 
                   'grove_usranger', 'grove_water_sensor'
                   
        """        

        super().__init__(iop, 'ARDUINO_SEEED', UART=UART, D2=D2, 
                         D3=D3, D4=D4, D5=D5, 
                         D6=D6, D7=D7, D8=D8,
                         A0=A0, A1=A1, A2=A2,
                         A3=A3, I2C=I2C)


class ArduinoDIGILENTGroveAdapter(GroveAdapter):
    """This class controls multiple Grove modules connected to a ArduinoDIGILENT adapter."""

    def __init__(self, iop, UART=None, G1=None, G2=None, 
                 G3=None, G4=None, G5=None, G6=None,
                 G7=None, A1=None, A2=None, A3=None,
                 A4=None, I2C=None):
        """Create a new ArduinoDIGILENTGroveAdapter object.

        Parameters
        ----------
        iop : name of IOP for adapter
              e.g. base.ARDUINO
        port args: ports by name either UART, G1, G2, G3, G4, G5, G6
                   A1, A2, A3, A4, I2C (can be a list)
                   
                   Available names of Grove modules for port args:
                   'grove_adc', 'grove_buzzer', 'grove_gesture', 'grove_imu',
                   'grove_joystick', 'grove_led_stick', 'grove_ledbar', 
                   'grove_lgcp', 'grove_light', 'grove_line_finder', 
                   'grove_oled', 'grove_pir', 'grove_potentiometer', 
                   'grove_relay', 'grove_servo', 'grove_temperature', 
                   'grove_usranger', 'grove_water_sensor'
                   
        """        
        super().__init__(iop, 'ARDUINO_DIGILENT', UART=UART, G1=G1, 
                         G2=G2, G3=G3, G4=G4, 
                         G5=G5, G6=G6, G7=G7,
                         A1=A1, A2=A2, A3=A3,
                         A4=A4, I2C=I2C)

class GroveG0Adapter(GroveAdapter):
    """This class controls multiple Grove modules connected to a Grove G0 (ZU) adapter."""

    def __init__(self, iop, G0=None):
        """Create a new GroveG0Adapter object.

        Parameters
        ----------
        iop : name of IOP for adapter
              e.g. base.ARDUINO
        port args: ports by name: G0
        """        

        super().__init__(iop, 'GROVE0', G0=G0)

class GroveG1Adapter(GroveAdapter):
    """This class controls multiple Grove modules connected to a Grove G1 (ZU) adapter."""

    def __init__(self, iop, G1=None):
        """Create a new GroveG1Adapter object.

        Parameters
        ----------
        iop : name of IOP for adapter
              e.g. base.ARDUINO
        port args: ports by name: G1
        """        

        super().__init__(iop, 'GROVE1', G1=G1)

moddir = os.path.dirname(os.path.realpath(__file__))

add_module_path(os.path.join(moddir, 'modules'))
