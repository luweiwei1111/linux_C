#!/usr/bin/python
from ctypes import *
import os

c_scan = cdll.LoadLibrary('./add.so')

c_scan.add.restype = c_int
c_scan.add.argtypes = (c_int, c_int)

c_int_a = c_int(256)
c_int_b = c_int(112)
return_value = c_int()
return_value = c_scan.add(c_int_a, c_int_b)

print(return_value)