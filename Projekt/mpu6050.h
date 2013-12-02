#ifndef MPU6050_H_
#define MPU6050_H_

#include "i2c8Bit.h"

#define TEMP_HIGH_BIT 0x41
#define TEMP_LOW_BIT 0x42

#define ACCX_HIGH_BIT 0x3B
#define ACCX_LOW_BIT 0x3C

#define ACCY_HIGH_BIT 0x3D
#define ACCY_LOW_BIT 0x3E

#define ACCZ_HIGH_BIT 0x3F
#define ACCZ_LOW_BIT 0x40

#define GYRX_HIGH_BIT 0x43
#define GYRX_LOW_BIT 0x44

#define GYRY_HIGH_BIT 0x45
#define GYRY_LOW_BIT 0x46

#define GYRZ_HIGH_BIT 0x47
#define GYRZ_LOW_BIT 0x48

#define MPU6050_CHIP_ADDRESS 0x68
#define MPU6050_FILE_PATH "/dev/i2c-1"

#define MOTION_INT_MASK 0x40
#define DATA_INT_MASK 0x01

class mpu6050{
public:

	mpu6050();

	double getTemp();
	double getAccelX();
	double getAccelY();
	double getAccelZ();
	double getGyroX();
	double getGyroY();
	double getGyroZ();

	bool isDataInterrput();
	bool isMotionInterrupt();

	void refreshInterrupt();

	virtual ~mpu6050();

private:

	i2c8Bit sensor;

	unsigned char interruptReg;

	void initializeSensor();
	inline double processAccelRegister(int reg);
	long readSignedSensor(unsigned char high_bit, unsigned char low_bit);
};

#endif


