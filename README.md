# PYNQ Hackathon Peripheral Modules
![python](https://github.com/Xilinx/PYNQ/workflows/Python/badge.svg)

---
This repository contains drivers for Arduino, PMOD, Grove and Raspberry PI peripherals connected to the recommended [**PYNQ-Z2**](http://www.pynq.io/board.html) board. Each peripheral driver comes with Jupyter notebooks which show how to use it. Application notebooks can be developed in pure Python using the Python API provided for each driver.
It also contains command line interface tool to generate peripheral driver templates for new peripehrals. Peripheral drivers are developed in Standard C

<p align="center">
  <img src="./images/pynq_hackathon_hw.png">
</p>

All notebooks in this repository are listed below:

__Grove__

| Name | Link to notebook |  Link to Purchase |
|:----------:|:------------------:|:------------------:|
| Grove I2C ADC | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_adc/notebooks/grove_adc.ipynb) | [Buy](https://www.seeedstudio.com/Grove-I2C-ADC.html) |

__Arduino__

| Name | Link to notebook |  Link to Purchase |
|:----------:|:------------------:|:------------------:|
| Grove I2C ADC | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_adc/notebooks/grove_adc.ipynb) | [Buy](https://www.seeedstudio.com/Grove-I2C-ADC.html) |

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

| Name | Link to notebook |  Link to Purchase |
|:----------:|:------------------:|:------------------:|
|Grove I2C ADC | [Notebook](https://github.com/Xilinx/PYNQ_peripherals/blob/main/pynq_peripherals/modules/grove_adc/notebooks/grove_adc.ipynb) | [Buy](https://www.seeedstudio.com/Grove-I2C-ADC.html) |


## Contributing

Contributions to this repository are welcomed. Please refer to [CONTRIBUTING.md](https://github.com/Xilinx/PYNQ_peripherals/blob/main/CONTRIBUTING.md) for details on how to contribute new peripheral
drivers.

## License

Copyright (C) 2021 Xilinx, Inc

SPDX-License-Identifier: BSD-3-Clause
