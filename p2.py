#!/bin/python
import os

for i in range(100000):
    os.read(0, 500)
    os.write(1,b"Hello"*100)
os.write(2, b"P2\n")