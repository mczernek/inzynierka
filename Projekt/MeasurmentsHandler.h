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

	MeasurmentsHandler(mpu6050& sensor, const char* filename = "plik", int filterStrength = 10, int range = 10);
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

	int range;

	std::ofstream of1;
	std::ofstream of2;
	std::ofstream of3;
	std::ofstream of4;
	std::ofstream of5;
	mpu6050& sensor;
	long starttime;
	const char* filename;

	volatile bool givenKeyWasPressed;

	FILE* openGnuplot();
	void waitForKeyPressed(char keyPressed);
	void loopStoring();

	std::list<inz::Measurement> templist1;
	std::list<inz::Measurement> resultsForConvolution1;
	std::list<inz::Measurement> convolutionResults1;
	std::vector<double> convolutionFactors1;

	std::list<inz::Measurement> templist2;
	std::list<inz::Measurement> resultsForConvolution2;
	std::list<inz::Measurement> convolutionResults2;
	std::vector<double> convolutionFactors2;


	std::list<inz::Measurement> templist3;
	std::list<inz::Measurement> resultsForConvolution3;
	std::list<inz::Measurement> convolutionResults3;
	std::vector<double> convolutionFactors3;

	std::list<inz::Measurement> templist4;
	std::list<inz::Measurement> resultsForConvolution4;
	std::list<inz::Measurement> convolutionResults4;
	std::vector<double> convolutionFactors4;

	std::list<inz::Measurement> templist5;
	std::list<inz::Measurement> resultsForConvolution5;
	std::list<inz::Measurement> convolutionResults5;
	std::vector<double> convolutionFactors5;

	long lastResultTime;

	void storeResult(long currTime);
	void addToTempList(long currTime, double x, double y, double z);
	void addToResultsForConvolution();
	void addToConvultionResults();

	std::string getRange();


};

#endif /* MEASURMENTSHANDLER_H_ */
