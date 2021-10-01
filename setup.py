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

from distutils.dir_util import mkpath, copy_tree
from distutils.file_util import copy_file
from setuptools import setup, find_packages
from setuptools.command.build_py import build_py as _build_py
import glob
import os


class build_py(_build_py):
    def run(self):
        super().run()
        build_dir = self.data_files[0][2]
        notebook_mod_directories = glob.glob(build_dir + '/modules/*/notebooks')
        notebook_app_directories = glob.glob(build_dir + '/apps/*/notebooks')
        notebook_directories = notebook_mod_directories+notebook_app_directories
        mkpath(os.path.join(build_dir, 'notebooks'))
        for d in notebook_directories:
            module_name = os.path.basename(os.path.dirname(d))
            copy_tree(d, os.path.join(build_dir, 'notebooks', module_name))


setup(
    name = "pynq_peripherals",
    version = "0.1.0",
    license = "BSD",
    author = "PYNQ Team",
    author_email = "pynq_support@xilinx.com",
    include_package_data = True,
    install_requires = ['pynq>=2.6.2', 'simple_term_menu'],
    entry_points = {
        'pynq.lib': ['pynq_peripherals = pynq_peripherals'],
        'pynq.notebooks': ['pynq_peripherals = pynq_peripherals.notebooks'],
        'console_scripts': ['pynq-new-peripheral = pynq_peripherals.cli:main']
    },
    packages=find_packages(),
    cmdclass = {
        'build_py': build_py
    },
    description = "Driver packages for PYNQ peripherals"
)
