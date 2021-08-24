# Creating a new peripheral driver

To develop a new peripheral driver we recommend the following flow:

 * Clone this repository to your PYNQ board
 * Install the repository in editable mode - `sudo pip3 install -e ...`
 * Run `pynq new-peripheral` and follow the prompt

Once the prompt has been completed there will be a new driver in the
`pynq_peripheral/modules` folder which you can add your code to. You can use
the existing Jupyter environment to test your code and develop your notebooks.
Once you have finished with the driver, copy your notebooks into the driver's
`notebooks` folder and you are ready to commit and share your new peripheral
driver.

# Coding guidelines for new peripherals

To keep some consistency across all of the PYNQ drivers and to make best use of
the PYNQ microblaze subsystem we have created a set of guidelines to keep in
mind when developing your driver. If you've used the `pynq new-peripheral`
command to create the template a lot of these will have been done for you.

## General points

*Driver Name*

Your driver should be of the form `{interface}_{peripheral}`, all in lower-case
and words separated by underscores.

*Consistent APIs*

All drivers should contain the following:

 * A typedef from a basic C type to the name of the driver - this is generally an integer.
 * All functions in a drivers API except for `_open` should take a driver object as the first parameter
 * A `_close` function with no arguments other than the driver object should be provided

*Header and source files*

The main API for your driver should be in a `${driver_name}.h` file in your
driver's include directory. This file constitutes the API as exported to
Python and should meet the following:

 * Be plain C
 * Contain an include guard
 * Include no other header files where possible
 * Contain the driver typedef and all function for your API
 * Have no functions that don't conform to the rest of this docuemnt.

All other header files should go in `include` and all source files in `src`.
Source files should compile as both C and C++ but do not need to conform to the
API specification.

*Argument and return types*

All functions in your API should conform to the following restrictions

 * Return void or primitive types
 * Take primitive, typedefs to primitives or arrays to primitives as arguments

In addition consider the following restrictions and semantics

 * All pointer types except void are passed on the stack and so should be limited in size
 * const pointers should be used where possible - in particular python strings can only be passed as const char pointers
 * const char pointers from a string will be NULL-terminated
 * void* should be used for any data larger than 50 bytes - buffers passed as void* pointers must be allocated with pynq.allocate
 * Non-const pointers will copy data back into the python object so only writeable python objects can be used

*Error handling and debugging*

 * Use the `pyexception` function to signal error conditions. This will abort
   the current function and throw an exception in python.
 * Use `pyprintf` to print debug information while developing or under error
   conditions - your driver should not print anything in normal operation.

*Notebooks*

Your driver should include one or more notebooks in its `notebooks` folder
which _fully document_ the API you have implemented. Notebooks should contain
both code and explanatory text introducing your API. It is important that every
function in your public API be exercised by your included notebooks.

We would rather have a slightly less functional but fully documented API than a
more complete but undocumented one.

Notebooks must include examples of all of your functions in Python but may
optionally include Microblaze C code showing common patterns that users may
wish to implement in C to improve the performance or functionality of the
peripheral.

*Including 3rd party code*

Any third-party code should be clearly label, ensured that it is released under
a compatible license and references to the origin and license added to the
THIRD\_PARTY.md file

## Grove-specific guidelines

For grove peripherals there are a few additional points to consider given that
they are often used together through and adapter or shield.

 * Include `grove_constants.h` in your API header - this is an exception to the
   no-include rule to make using your library easier.
 * Your `_open` function should take only a single `int grove_id` argument
   which can be passed to the functions in `grove_interfaces.h` to open
   devices.
 * I2C peripherals can have an `_open_at_address` function which takes
   `grove_id` and `i2c_address` arguments. This is not required and the default
   should be the address the grove peripheral is shipped with.
 * You should expect that multiple instances of your device are used at the same
   time. Any state that is maintained on a per-device instance should be kept in
   a static array of at least 4 entries. This will be created automatically if you
   specify device-specific data when generating the template.

Copyright (C) 2021 Xilinx, Inc

SPDX-License-Identifier: BSD-3-Clause