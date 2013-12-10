#include "i2c8Bit.h"
#include "mpu6050.h"
#include <iostream>
#include <stdint.h>
#include <math.h>

mpu6050::mpu6050(int range) :
		sensor(MPU6050_CHIP_ADDRESS, MPU6050_FILE_PATH) {
	interruptReg = 0x00;
	initializeSensor(range);
}

void mpu6050::initializeSensor(int range) {

	resetDevice();
	wakeUpDevice();
	setRange(range);

}

double mpu6050::getTemp() {
	int read = readSignedSensor(TEMP_HIGH_BIT, TEMP_LOW_BIT);

	double temp;

	std::cout << read << std::endl;

	temp = ((double) read) / 340.0 + 36.53;

	return temp;
}

void mpu6050::resetDevice() {

	unsigned char h;

	sensor.writeReg(0x6b, 0x80);
	sensor.readReg(0x6b, h);
	std::cout << "Reseting finished! Register 0x6b set to: " << (int) h
			<< std::endl;
	sensor.readReg(0x6b, h);
	std::cout << "And then back to: " << (int) h << std::endl;
}

void mpu6050::wakeUpDevice() {
	unsigned char h;

	sensor.writeReg(0x6b, 0x00);
	sensor.readReg(0x6b, h);
	std::cout << "Device woken up, register 0x6b has value: " << (int) h
			<< std::endl;
}

int mpu6050::normalizeRange(int new_range) {

	unsigned char result;

	switch (new_range) {
	case 0:
	case 1:
	case 2:
	case 3:
		result = new_range;
		break;
	default:
		result = 0;
		break;
	}
	return result;
}

void mpu6050::initMultiplier() {
	switch (range) {
	case 1:
		rangeMultiplier = 2;
		break;
	case 2:
		rangeMultiplier = 4;
		break;
	case 3:
		rangeMultiplier = 8;
		break;
	case 0:
	default:
		rangeMultiplier = 1;
		break;
	}
}

void mpu6050::setRange(int new_range) {
	unsigned char toSet = normalizeRange(new_range);
	range = (int) toSet;
	initMultiplier();

	unsigned char h;
	toSet = toSet << 3;

	sensor.readReg(0x1c, h);

	std::cout << (int) h << std::endl;
	std::cout << (int) toSet << std::endl;
	std::cout << (int) toSet << std::endl;
	std::cout << (int) (231 & h) << std::endl;
	std::cout << (int) ((h & 231) | toSet) << std::endl;

	std::cout << "Register 0x1c will be set for " << (int) ((h & 231) | toSet)
			<< std::endl;
	sensor.writeReg(0x1c, (h & 231) | toSet);
	sensor.readReg(0x1c, h);
	std::cout << "Register 0x1c is set for: " << (int) h << std::endl;

}

double mpu6050::getAccelX() {
	int read = readSignedSensor(ACCX_HIGH_BIT, ACCX_LOW_BIT);
	return processAccelRegister(read);
}

double mpu6050::getAccelY() {
	int read = readSignedSensor(ACCY_HIGH_BIT, ACCY_LOW_BIT);
	return processAccelRegister(read);
}

double mpu6050::getAccelZ() {
	int read = readSignedSensor(ACCZ_HIGH_BIT, ACCZ_LOW_BIT);
	return processAccelRegister(read);
}

inline double mpu6050::processAccelRegister(int reg) {
	return (double) reg / (16384 / rangeMultiplier);
}

long mpu6050::readSignedSensor(unsigned char high_bit, unsigned char low_bit) {
	unsigned char high;
	sensor.readReg(high_bit, high);
	unsigned char low;
	sensor.readReg(low_bit, low);

	uint16_t unsign = high;

	unsign = unsign << 8;
	unsign += low;

	return (int16_t) unsign;
}

void mpu6050::refreshInterrupt() {
	sensor.readReg(0x3A, interruptReg);
}

bool mpu6050::isDataInterrput() {
	return interruptReg & DATA_INT_MASK;
}

bool mpu6050::isMotionInterrupt() {
	return interruptReg & MOTION_INT_MASK;
}

mpu6050::~mpu6050() {
	resetDevice();
}
