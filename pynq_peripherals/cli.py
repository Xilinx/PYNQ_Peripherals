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

import argparse
import os
import shutil
from .templates import TEMPLATES
from simple_term_menu import TerminalMenu

THIS_DIR = os.path.dirname(os.path.realpath(__file__))
DEFAULT_DIR = os.path.join(THIS_DIR, "modules")

def _template_option_parser():
    parser = argparse.ArgumentParser(description="Create new PYNQ peripheral")
    parser.add_argument("-f", "--force", help="Replace existing peripheral if present",
                        action="store_true")
    parser.add_argument("-d", "--directory", help="Directory to create the new peripheral",
                        default=DEFAULT_DIR)
    return parser

def main():
    parser = _template_option_parser()
    args = parser.parse_args()
    peripheral_name = input("Name for peripheral: ")
    peripheral_dir = os.path.join(args.directory, peripheral_name)
    if os.path.exists(peripheral_dir):
        if args.force:
            shutil.rmtree(peripheral_dir)
        else:
            raise RuntimeError("Peripheral already exists")
    template_index = TerminalMenu([t.name for t in TEMPLATES]).show()
    TEMPLATES[template_index].run(peripheral_name, peripheral_dir)
