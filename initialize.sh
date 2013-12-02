#!/bin/bash
sudo modprobe i2c-dev
sudo chmod o+rw /dev/i2c*
sudo i2cset -y 1 0x68 0x6b 0
