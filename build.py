#!/usr/bin/python3

print ('Call build.py') 

builds = [
    "iostream", 
    "fstream", 
    "iomanip", 
    "cassert", 
    "ios", 
    "array", 
    "bit", 
    "source_location", 
]

import os 

for b in builds: 
    s = 'g++-12 -std=c++23 -fmodules-ts -xc++-system-header ' + b 
    print (s)
    os.system(s) 

print ('done')