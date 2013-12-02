#!/bin/bash

HIGH=`i2cget -y 1 0x68 0x41 | sed -e 's/^0x//g' | tr a-z A-Z`
LOW=`i2cget -y 1 0x68 0x42 | sed -e 's/^0x//g' | tr a-z A-Z`
TEMP=$HIGH$LOW 
echo $TEMP
echo "ibase=16; scale=3; ((-((FFFF-$TEMP)+1))/154)+24.87" | bc 
