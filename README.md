# PYNQ Peripherals
![python](https://github.com/Xilinx/PYNQ/workflows/Python/badge.svg)

This repository contains drivers for Arduino, PMOD, Grove and Raspberry Pi peripherals supported on the recommended [**PYNQ-Z2**](http://www.pynq.io/board.html) board. Each peripheral driver comes with Jupyter notebooks which show how to use it. Application notebooks can be developed solely in Python using the Python API provided for each driver.
It also contains a command line interface tool to generate peripheral driver templates for new peripherals. Peripheral drivers are developed in Standard C

<p align="center">
  <img src="./images/pynq_hackathon_hw.png">
</p>

All notebooks in this repository are listed below:

__Grove Modules__

| Name | Link to notebook |  Link to module |
|:----------:|:------------------:|:------------------:|
| Grove I2C ADC | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_adc/notebooks/grove_adc.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-I2C-ADC.html) |
| Grove I2C Barometer | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_barometer/notebooks/grove_barometer.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-High-Precision-Barometer-Sensor-DPS310-p-4397.html) |
| Grove buzzer | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_buzzer/notebooks/grove_buzzer.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-Buzzer.html) |
| Grove I2C Gesture | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_gesture/notebooks/grove_gesture.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-Gesture-PAJ7620U2.html) |
| Grove I2C IMU | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_imu/notebooks/grove_imu.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-IMU-10DOF-v2-0.html) |
| Grove Thumb Joystick | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_joystick/notebooks/grove_joystick.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-Thumb-Joystick.html) |
| Grove RGB LED Stick | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_led_stick/notebooks/grove_led_stick.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-RGB-LED-Stick-10-WS2813-Mini.html) |
| Grove LED Bar| [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_ledbar/notebooks/grove_ledbar.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-LED-Bar-v2-0.html) |
| Grove Light-Gesture-Color-Proximity sensor | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_lgcp/notebooks/grove_lgcp.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-Light-Color-Proximity-Sensor-TMG39931-p-2879.html) |
| Grove Light sensor | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_light/notebooks/grove_light.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-Light-Sensor-v1-2-LS06-S-phototransistor.html) |
| Grove Line Finder | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_line_finder/notebooks/grove_line_finder.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-Line-Finder-v1-1.html) |
| Grove I2C OLED | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_oled/notebooks/grove_oled.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-OLED-Display-0-96.html) |
| Grove PIR | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_pir/notebooks/grove_pir.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-PIR-Motion-Sensor.html) |
| Grove Slide Potentiometer | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_potentiometer/notebooks/grove_potentiometer.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-Slide-Potentiometer.html) |
| Grove Relay | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_relay/notebooks/grove_relay.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-Relay.html) |
| Grove Servo | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_servo/notebooks/grove_servo.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-Servo.html) |
| Grove Temperature sensor| [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_temperature/notebooks/grove_temperature.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-Temperature-Sensor.html) |
| Grove Ultrasonic Ranger | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_usranger/notebooks/grove_usranger.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-Ultrasonic-Distance-Sensor.html) |
| Grove Water_sensor | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_water_sensor/notebooks/grove_water_sensor.ipynb) | [Click Here](https://www.seeedstudio.com/Grove-Water-Sensor.html) |

__Non-Grove Modules__

| Name | Link to notebook |  Link to module |
|:----------:|:------------------:|:------------------:|
| Geared Motor Module | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/geared_motor/notebooks/geared_motor.ipynb) | [Click Here 1](https://www.dfrobot.com/product-1705.html) [Click Here 2](https://www.dfrobot.com/product-100.html) |
| Sparkfun Environment Sensor | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_envsensor/notebooks/grove_envsensor.ipynb) | [Click Here](https://www.sparkfun.com/products/16466) |

## Installation

To install the notebooks run

```console
sudo pip install git+https://github.com/Xilinx/PYNQ_peripherals.git
```
```console
sudo pynq get-notebooks pynq_peripherals -p $PYNQ_JUPYTER_NOTEBOOKS
```

in your jupyter-notebooks folder.

## Example Hacks

| Name | Link to notebook |
|:----------:|:------------------:|
| Plant Monitoring System | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/apps/app0_plant_monitoring_system/notebooks/plant_monitoring_system.ipynb) |
| Room Control System | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/apps/app1_room_control_system/notebooks/room_control_system.ipynb) |
| PYNQ Pattern Generator LED string | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/apps/app2_led_strings/notebooks/led_strings.ipynb) |
| Voice Enabled Room Control System | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/apps/app3_voice_enabled_room_control_system/notebooks/voice_enabled_room_control_system.ipynb) |
| Automatic Door Control and Motion Logger | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/apps/app4_automatic_door_control_and_motion_logger/notebooks/automatic_door_control_and_motion_logger.ipynb) |
| PYNQ Car using DFROBOT Turtle | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/apps/app5_pynq_car/notebooks/pynq_car.ipynb) |

## Contributing

Contributions to this repository are welcomed. Please refer to [CONTRIBUTING.md](https://github.com/Xilinx/PYNQ_peripherals/blob/main/CONTRIBUTING.md) for details on how to contribute new peripheral
drivers.

## License

Copyright (C) 2021 Xilinx, Inc

SPDX-License-Identifier: BSD-3-Clause
