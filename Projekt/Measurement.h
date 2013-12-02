/*
 * Measurement.h
 *
 *  Created on: 2 gru 2013
 *      Author: mis3k
 */

#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

#include <string>
#include <sstream>

namespace inz{

class Measurement {

public:

	long time;
	double accelX;
	double accelY;
	double accelZ;

	Measurement(long time, double X, double Y, double Z): time(time), accelX(X), accelY(Y), accelZ(Z){
	}

	const std::string toPrint(){
		std::stringstream stream;
		stream << time << "\t" << accelX << "\t" << accelY << "\t" << accelZ << "\n";
		return stream.str();
	}

	virtual ~Measurement();
};

} /* namespace mpu6050 */

#endif /* MEASUREMENT_H_ */
