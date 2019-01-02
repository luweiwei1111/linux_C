Python调用linux C的库函数lib_xx.so

python完美兼容C语言，有了ctypes 可以调用c相关代码 
如果是c++代码   编译的时候加上 extern "C" 就可以了 ，如果是一些复杂类型比如结构体，类，联合一些 可以考虑用boost.python

#!/usr/bin/python
from ctypes import *
import os

#加载C动态库
c_scan = cdll.LoadLibrary('./add.so')

c_scan.add.restype = c_int           #函数的返回值类型int
c_scan.add.argtypes = (c_int, c_int) #入参类型

#两个入参256和112
c_int_a = c_int(256)
c_int_b = c_int(112)

#设置返回值变量的类型
return_value = c_int()
#入参最后得出结果保存在return_value里面
return_value = c_scan.add(c_int_a, c_int_b)

print(return_value)

#编译过程
gcc -c -fpic exp.c
gcc -shared exp.o -o add.so
#执行
python add_c.py
结果：
368