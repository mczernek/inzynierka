/*
 * MeasurmentsHandler.h
 *
 *  Created on: 27 lis 2013
 *      Author: mis3k
 */

#ifndef MEASURMENTSHANDLER_H_
#define MEASURMENTSHANDLER_H_

#include "mpu6050.h"
#include "Measurement.h"
#include <sys/time.h>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>

class MeasurmentsHandler {

public:

	MeasurmentsHandler(mpu6050& sensor, const char* filename = "plik", int filterStrength = 10);
	virtual ~MeasurmentsHandler();

	void measure(int timeInSecs = 2);
	void measureUntilKeyPressed(char keyPressed);
	void addResults(long currTime);
	void plotResultsOnOneChart();
	void plotResultsInSeparateCharts();

	void storeValuesToFile();

	void getCurrentTime(timeval& toSet);
	long getCurrentTimeInMilis();

private:

	const int period;

	std::ofstream of;
	mpu6050& sensor;
	long starttime;
	const char* filename;

	volatile bool givenKeyWasPressed;

	FILE* openGnuplot();
	void waitForKeyPressed(char keyPressed);
	void loopStoring();

	std::list<inz::Measurement> templist;
	std::list<inz::Measurement> resultsForConvolution;
	std::list<inz::Measurement> convolutionResults;
	std::vector<double> convolutionFactors;

	long lastResultTime;

	void storeResult(long currTime);
	void addToTempList(long currTime, double x, double y, double z);
	void addToResultsForConvolution();
	void addToConvultionResults();


};

#endif /* MEASURMENTSHANDLER_H_ */
