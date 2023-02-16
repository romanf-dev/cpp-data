#!/bin/bash

g++ -include node.hpp -pedantic -std=c++17 -c -o cppdata.o $1
ld --gc-sections -T gnu.ld -o cppdata.elf cppdata.o
objcopy -O binary --only-section .blob cppdata.elf cppdata.bin

