#!/bin/bash

HIGH=`i2cget -y 1 0x68 0x43 | sed -e 's/^0x//g' | tr a-z A-Z`
LOW=`i2cget -y 1 0x68 0x44 | sed -e 's/^0x//g' | tr a-z A-Z`
TEMP=$HIGH$LOW 
echo $TEMP
echo "ibase=16; scale=9; $TEMP/131" | bc

HIGH=`i2cget -y 1 0x68 0x45 | sed -e 's/^0x//g' | tr a-z A-Z`
LOW=`i2cget -y 1 0x68 0x46 | sed -e 's/^0x//g' | tr a-z A-Z`
TEMP=$HIGH$LOW
echo $TEMP
echo "ibase=16; scale=9; $TEMP/131" | bc

HIGH=`i2cget -y 1 0x68 0x47 | sed -e 's/^0x//g' | tr a-z A-Z`
LOW=`i2cget -y 1 0x68 0x48 | sed -e 's/^0x//g' | tr a-z A-Z`
TEMP=$HIGH$LOW
echo $TEMP
echo "ibase=16; scale=9; $TEMP/131" | bc
