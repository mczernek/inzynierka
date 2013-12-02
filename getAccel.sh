#!/bin/bash

HIGH=`i2cget -y 1 0x68 0x3B | sed -e 's/^0x//g' | tr a-z A-Z`
LOW=`i2cget -y 1 0x68 0x3C | sed -e 's/^0x//g' | tr a-z A-Z`
TEMP=$HIGH$LOW 
echo $TEMP
echo "ibase=16; scale=9; $TEMP/4000" | bc

HIGH=`i2cget -y 1 0x68 0x3D | sed -e 's/^0x//g' | tr a-z A-Z`
LOW=`i2cget -y 1 0x68 0x3E | sed -e 's/^0x//g' | tr a-z A-Z`
TEMP=$HIGH$LOW
echo $TEMP
echo "ibase=16; scale=9; $TEMP/4000" | bc

HIGH=`i2cget -y 1 0x68 0x3F | sed -e 's/^0x//g' | tr a-z A-Z`
LOW=`i2cget -y 1 0x68 0x40 | sed -e 's/^0x//g' | tr a-z A-Z`
TEMP=$HIGH$LOW
echo $TEMP
echo "ibase=16; scale=9; $TEMP/4000" | bc
