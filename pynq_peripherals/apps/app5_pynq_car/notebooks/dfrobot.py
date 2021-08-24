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

class PynqCar():
    def __init__(self, left_motor, right_motor):
        self.left_motor = left_motor
        self.right_motor = right_motor
        self.speed = 0
        self.speed_offset = 0
        self.speed_diff = 0
        self.stop()
        
    def set_speed(self, speed):
        if (abs(speed)+abs(self.speed_offset)+abs(self.speed_diff) < 100):
            self.speed = speed
            left_speed = self.speed+self.speed_offset+self.speed_diff
            right_speed = self.speed-self.speed_offset-self.speed_diff
            if left_speed < 0:
                self.left_motor.backward()
                self.left_motor.set_speed(0-left_speed)
            else:
                self.left_motor.forward()
                self.left_motor.set_speed(left_speed)
            if right_speed < 0:
                self.right_motor.backward()
                self.right_motor.set_speed(0-right_speed)
            else:
                self.right_motor.forward()
                self.right_motor.set_speed(right_speed)
        else:
            print("Error: Speed must be -100 to 100")
            
    def stop(self):
        self.right_motor.set_speed(0)
        self.left_motor.set_speed(0)
        
    def steering(self, diff):
        self.speed_diff = diff
        self.set_speed(self.speed)