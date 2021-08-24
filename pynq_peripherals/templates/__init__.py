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

from simple_term_menu import TerminalMenu
import jinja2
import os

GROVE_HEADER = r"""#pragma once

#include <grove_constants.h>
{%- if adc_open %}
#include <grove_adc.h>
{%- endif %}

// Device typedef
typedef int {{ peripheral_name }};

// Device lifetime functions
{{ peripheral_name }} {{ peripheral_name }}_open(int grove_id);
{%- if i2c_use_address %}
{{ peripheral_name }} {{ peripheral_name }}_open_at_address(int grove_id, int address);
{%- endif %}
{%- if adc_open %}
{{ peripheral_name }} {{ peripheral_name }}_open_adc(grove_adc adc);
{%- endif %}
void {{ peripheral_name }}_close({{ peripheral_name }} p);

// Device functions
void {{ peripheral_name }}_example({{ peripheral_name }} p{{example_args}});

"""

GROVE_SOURCE = r"""#include <grove_interfaces.h>
#include <{{ peripheral_name }}.h>

{% if i2c_default_address -%}
#define I2C_ADDRESS {{i2c_default_address}}
{% endif -%}
{% if has_data_struct -%}
#define DEVICE_MAX 4

struct {{peripheral_name}}_info {
{{ data_contents }}
    int count;
};

static struct {{peripheral_name}}_info info[DEVICE_MAX];

static int {{peripheral_name}}_next_index() {
    for (int i = 0; i < DEVICE_MAX; ++i) {
        if (info[i].count == 0) return i;
    }
    return -1;
}
{% endif -%}

{% if adc_open -%}
{%- if has_data_struct %}
static {{ peripheral_name }} {{ peripheral_name }}_open_analog(analog pin) {
    {{peripheral_name}} dev_id = {{peripheral_name}}_next_index();
    info[dev_id].count++;
    info[dev_id].{{device_name}} = pin;
{%- if has_data %}
    info[dev_id].data = 0; // Static data initialisation
{%- endif %}
    return dev_id;
}
{%- endif -%}

{{ peripheral_name }} {{ peripheral_name }}_open(int grove_id) {
{%- if has_data_struct %}
    return {{ peripheral_name }}_open_analog(analog_open_grove(grove_id));
{%- else %}
    return analog_open_grove(grove_id);
{%- endif %}
}

{{ peripheral_name }} {{ peripheral_name }}_open_adc(grove_adc adc) {
{%- if has_data_struct %}
    return {{ peripheral_name }}_open_analog(analog_open_adc(adc));
{%- else %}
    return analog_open_adc(adc);
{%- endif %}
}
{%- else -%}
{{ peripheral_name }} {{ peripheral_name }}_open(int grove_id) {
{%- if i2c_use_address %}
    return {{peripheral_name}}_open_at_address(grove_id, I2C_ADDRESS);
{%- else %}
{%- if has_data_struct %}
    {{peripheral_name}} dev_id = {{peripheral_name}}_next_index();
    info[dev_id].count++;
{%- if twowire %}
    info[dev_id].{{device_name_a}} = {{device_type}}_open_grove_a(grove_id);
    info[dev_id].{{device_name_b}} = {{device_type}}_open_grove_b(grove_id);
{%- else %}
    info[dev_id].{{device_name}} = {{device_type}}_open_grove(grove_id);
{%- endif %}
{%- if has_data %}
    info[dev_id].data = 0; // Static data initialisation
{%- endif %}
    return dev_id;
{%- else %}
    {{device_type}} {{device_name}} = {{device_type}}_open_grove(grove_id);
    return {{device_name}};
{%- endif %}
{%- endif %}
}
{%- endif %}

{% if i2c_use_address -%}
{{peripheral_name}} {{peripheral_name}}_open_at_address(int grove_id, int address) {
    {{peripheral_name}} dev_id = {{peripheral_name}}_next_index();
    info[dev_id].count++;
    info[dev_id].{{device_name}} = {{device_type}}_open_grove(grove_id);
    info[dev_id].address = address;
{%- if has_data %}
    info[dev_id].data = 0; // Static data initialisation
{%- endif %}
    return dev_id;
}
{% endif -%}

void {{ peripheral_name }}_close({{peripheral_name}} p) {
{%- if has_data_struct %}
    if (--info[p].count != 0) return;
{%- if twowire %}
    {{device_type}} {{device_name_a}} = info[p].{{device_name_a}};
    {{device_type}} {{device_name_b}} = info[p].{{device_name_b}};
{%- else %}
    {{device_type}} {{device_name}} = info[p].{{device_name}};
{%- endif %}
{%- else %}
    {{device_type}} {{device_name}} = p;
{%- endif %}
{%- if twowire %}
    {{device_type}}_close({{device_name_a}});
    {{device_type}}_close({{device_name_b}});
{%- else %}
    {{device_type}}_close({{device_name}});
{%- endif %}
}

void {{peripheral_name}}_example({{peripheral_name}} p{{example_args}}) {
{%- if has_data_struct %}
{%- if twowire %}
    {{device_type}} {{device_name_a}} = info[p].{{device_name_a}};
    {{device_type}} {{device_name_b}} = info[p].{{device_name_b}};
{%- else %}
    {{device_type}} {{device_name}} = info[p].{{device_name}};
{%- endif %}
{%- else %}
    {{device_type}} {{device_name}} = p;
{%- endif %}
    {{ example_source }}
}

"""

class PeripheralTemplate:
    def __init__(self, name):
        self.name = name
        self.config = {}

    def run(self, peripheral_name, peripheral_dir):
        self.config['peripheral_name'] = peripheral_name
        os.mkdir(peripheral_dir)
        os.mkdir(os.path.join(peripheral_dir, 'include'))
        os.mkdir(os.path.join(peripheral_dir, 'notebooks'))
        os.mkdir(os.path.join(peripheral_dir, 'src'))
        header_template = jinja2.Template(self.header_template)
        with open(os.path.join(peripheral_dir, 'include', peripheral_name + ".h"), 'w') as f:
            f.write(header_template.render(self.config))
        source_template = jinja2.Template(self.source_template)
        with open(os.path.join(peripheral_dir, 'src', peripheral_name + ".c"), 'w') as f:
            f.write(source_template.render(self.config))
        print(f"Template written in {peripheral_dir}")


class GroveTemplate(PeripheralTemplate):
    def __init__(self, name):
        super().__init__(name)
        self.data = []
        self.header_template = GROVE_HEADER
        self.source_template = GROVE_SOURCE

    def run(self, peripheral_name, peripheral_dir):
        print("Add additional per-instance data?")
        if TerminalMenu(["Yes", "No"]).show() == 0:
            self.data.append("int data")
            self.config['has_data'] = True
        if len(self.data) > 1:
            self.config['has_data_struct'] = True
            self.config['data_contents'] = '\n'.join([f'    {l};' for l in self.data])
        super().run(peripheral_name, peripheral_dir)

class WireChoiceTemplate(GroveTemplate):
    def run(self, peripheral_name, peripheral_dir):
        print("Number of signal wires in the peripheral")
        if TerminalMenu(["1 wire", "2 wires"]).show() == 0:
            self.data.append(self.pinname)
            if self.config['device_type'] == 'analog':
                self.config['adc_open'] = True
        else:
            self.config['device_name_a'] = input("Name of pin A/0: ")
            self.config['device_name_b'] = input("Name of pin B/1: ")
            self.data.append(f"{self.config['device_type']} {self.config['device_name_a']}")
            self.data.append(f"{self.config['device_type']} {self.config['device_name_b']}")
            self.config['device_name'] = self.config['device_name_a']
            self.config['twowire'] = True
        super().run(peripheral_name, peripheral_dir)

class GroveAnalogTemplate(WireChoiceTemplate):
    def __init__(self):
        super().__init__("Grove Analog")
        self.pinname = "analog pin"
        self.config['device_type'] = 'analog'
        self.config['device_name'] = 'pin'
        self.config['device_name_a'] = 'pin_a'
        self.config['device_name_b'] = 'pin_b'
        self.config['example_source'] = f"{self.config['device_type']}_get_voltage({self.config['device_name']});"


class GroveGPIOTemplate(WireChoiceTemplate):
    def __init__(self):
        super().__init__("Grove GPIO")
        self.pinname = "gpio pin"
        self.config['device_type'] = 'gpio'
        self.config['device_name'] = 'pin'
        self.config['device_name_a'] = 'pin_a'
        self.config['device_name_b'] = 'pin_b'
        self.config['example_source'] = f"{self.config['device_type']}_read({self.config['device_name']});"

class GroveUARTTemplate(GroveTemplate):
    def __init__(self):
        super().__init__("Grove UART")
        self.data.append("uart uart_dev")
        self.config['device_type'] = 'uart'
        self.config['device_name'] = 'uart_dev'
        self.config['example_args'] = ", const char* buffer, int length"
        self.config['example_source'] = "uart_write(uart_dev, buffer, length);"

class GroveI2CTemplate(GroveTemplate):
    def __init__(self):
        super().__init__("Grove I2C")
        self.data.append("i2c i2c_dev")
        self.config['device_type'] = 'i2c'
        self.config['device_name'] = 'i2c_dev'
        self.config['example_args'] = ", const char* buffer, int length"

    def run(self, peripheral_name, peripheral_dir):
        address = input("Default I2C address: ")
        self.config['i2c_default_address'] = address
        print("Support non-default addresses?")
        if TerminalMenu(['Yes', 'No']).show() == 0:
            self.data.append("unsigned char address")
            self.config['i2c_use_address'] = True
            self.config['example_source'] = "i2c_write(i2c_dev, info[p].address, buffer, length);"
        else:
            self.config['example_source'] = "i2c_write(i2c_dev, I2C_ADDRESS, buffer, length);"
        super().run(peripheral_name, peripheral_dir)


TEMPLATES = [
        GroveAnalogTemplate(),
        GroveGPIOTemplate(),
        GroveI2CTemplate(),
        GroveUARTTemplate()
]
