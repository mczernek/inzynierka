#include "i2c8Bit.h"
#include "mpu6050.h"
#include <iostream>
#include <stdint.h>
#include <math.h>

mpu6050::mpu6050(): sensor(MPU6050_CHIP_ADDRESS, MPU6050_FILE_PATH){
	interruptReg = 0x00;
	initializeSensor();
}

void mpu6050::initializeSensor(){

    sensor.writeReg(0x38, 0x40);
    sensor.writeReg(0x1f, 0x01);
    sensor.writeReg(0x68, 0x07);
    sensor.writeReg(0x6b, 0x00);

    unsigned char a, b;
    
    sensor.readReg(0x6b, a);
    sensor.readReg(0x38, b);

    
    std::cout << (int)a << " " << (int)b << std::endl;
}

double mpu6050::getTemp(){
    int read = readSignedSensor(TEMP_HIGH_BIT, TEMP_LOW_BIT);
    
    double temp; 
    
    std::cout << read << std::endl;
    
    temp = ((double)read)/340.0+36.53;
    
    return temp;
}

double mpu6050::getAccelX(){
    int read = readSignedSensor(ACCX_HIGH_BIT, ACCX_LOW_BIT);
    return processAccelRegister(read);
}

double mpu6050::getAccelY(){
    int read = readSignedSensor(ACCY_HIGH_BIT, ACCY_LOW_BIT);
    return processAccelRegister(read);
}

double mpu6050::getAccelZ(){
    int read = readSignedSensor(ACCZ_HIGH_BIT, ACCZ_LOW_BIT);
    return processAccelRegister(read);
}

inline double mpu6050::processAccelRegister(int reg){
    return (double)reg/16384;
}

long mpu6050::readSignedSensor(unsigned char high_bit, unsigned char low_bit){
    unsigned char high;
    sensor.readReg(high_bit, high);
    unsigned char low;
    sensor.readReg(low_bit, low);
    
    uint16_t unsign = high;
    
    unsign = unsign << 8;
    unsign += low;

    return (int16_t)unsign;
}

void mpu6050::refreshInterrupt(){
	sensor.readReg(0x3A, interruptReg);
}

bool mpu6050::isDataInterrput(){
	return interruptReg & DATA_INT_MASK;
}

bool mpu6050::isMotionInterrupt(){
	return interruptReg & MOTION_INT_MASK;
}

mpu6050::~mpu6050(){
	//Nothing to do here so far
}
