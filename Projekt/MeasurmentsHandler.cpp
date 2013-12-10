/*
 * MeasurmentsHandler.cpp
 *
 *  Created on: 27 lis 2013
 *      Author: mis3k
 */

#include "MeasurmentsHandler.h"
#include "Measurement.h"
#include <iostream>
#include <thread>
#include <sstream>

using namespace std;

MeasurmentsHandler::MeasurmentsHandler(mpu6050& sensor, const char* filename,
		int filterStrength, int range) :
		of1("file1"), of2("file2"), of3("file3"), of4("file4"), of5("file5"), sensor(sensor), filename(filename), period(10), lastResultTime(
				0), range(range) {
	starttime = 0;
	givenKeyWasPressed = false;

	/*if (filterStrength != 0) {
		for (int i = 0; i < filterStrength; i++) {
			convolutionFactors1.push_back((double) 1 / filterStrength);
		}
	} else {*/
		convolutionFactors1.push_back(0.08);
		convolutionFactors1.push_back(0.22);
		convolutionFactors1.push_back(0.4);
		convolutionFactors1.push_back(0.22);
		convolutionFactors1.push_back(0.08);
	//}

	for (int i = 0; i < 1; i++) {
		convolutionFactors2.push_back((double) 1 / 1);
	}

	for (int i = 0; i < 5; i++) {
		convolutionFactors3.push_back((double) 1 / 5);
	}

	for (int i = 0; i < 8; i++) {
		convolutionFactors4.push_back((double) 1 / 8);
	}

	for (int i = 0; i < 15; i++) {
		convolutionFactors5.push_back((double) 1 / 15);
	}

	std::cout << "Initialization finished" << std::endl;

}

void MeasurmentsHandler::measure(int timeInSecs) {
	//std::cout << "Started measuring for given time" << std::endl;
	starttime = getCurrentTimeInMilis();
	long currTime;
	while ((currTime = getCurrentTimeInMilis() - starttime) < timeInSecs * 1000) {
		//std::cout << "Trying to add results for given time: " << currTime << std::endl;
		addResults(currTime);
	}
}

void MeasurmentsHandler::waitForKeyPressed(char keyPressed) {
	char c;
	std::cin >> c;
	std::cout << c << keyPressed;
	while (c != keyPressed) {
		std::cin >> c;
	}
	givenKeyWasPressed = true;
}

void MeasurmentsHandler::measureUntilKeyPressed(char keyPressed) {
	//std::cout << "Started measuring until key pressed" << std::endl;
	starttime = getCurrentTimeInMilis();

	std::thread a(&MeasurmentsHandler::waitForKeyPressed, this, keyPressed);

	long currTime;
	while (!givenKeyWasPressed) {
		currTime = getCurrentTimeInMilis() - starttime;
		addResults(currTime);
	}
	a.join();

}

void MeasurmentsHandler::addResults(long currTime) {
	//std::cout << "Results adding" << std::endl;
	sensor.refreshInterrupt();
	if (sensor.isMotionInterrupt()) {
		of1 << "Interrupt caught!" << std::endl;
	}
	if (sensor.isDataInterrput()) {
		storeResult(currTime);
	}
}

void MeasurmentsHandler::storeValuesToFile() {
	//std::cout << convolutionResults.size() << std::endl;
	for (std::list<inz::Measurement>::iterator it = convolutionResults1.begin();
			it != convolutionResults1.end(); it++) {
		of1 << it->toPrint();
	}
	for (std::list<inz::Measurement>::iterator it = convolutionResults2.begin();
			it != convolutionResults2.end(); it++) {
		of2 << it->toPrint();
	}
	for (std::list<inz::Measurement>::iterator it = convolutionResults3.begin();
			it != convolutionResults3.end(); it++) {
		of3 << it->toPrint();
	}
	for (std::list<inz::Measurement>::iterator it = convolutionResults4.begin();
			it != convolutionResults4.end(); it++) {
		of4 << it->toPrint();
	}
	for (std::list<inz::Measurement>::iterator it = convolutionResults5.begin();
			it != convolutionResults5.end(); it++) {
		of5 << it->toPrint();
	}
}

void MeasurmentsHandler::plotResultsOnOneChart() {
	FILE* gnuplotPipe = openGnuplot();

	fprintf(gnuplotPipe, "set term x11 1\n");
	fprintf(gnuplotPipe, "plot [] %s \"%s\" using 1:2 title \"X\",",
			getRange().c_str(), "file1");
	fprintf(gnuplotPipe, "\"%s\" using 1:3 title \"Y\",", "file1");
	fprintf(gnuplotPipe, "\"%s\" using 1:4 title \"Z\"\n", "file1");

	fprintf(gnuplotPipe, "\n");

	fprintf(gnuplotPipe, "set term x11 2\n");
	fprintf(gnuplotPipe, "plot [] %s \"%s\" using 1:2 title \"X\",",
			getRange().c_str(), "file2");
	fprintf(gnuplotPipe, "\"%s\" using 1:3 title \"Y\",", "file2");
	fprintf(gnuplotPipe, "\"%s\" using 1:4 title \"Z\"\n", "file2");

	fprintf(gnuplotPipe, "\n");

	fprintf(gnuplotPipe, "set term x11 3\n");
	fprintf(gnuplotPipe, "plot [] %s \"%s\" using 1:2 title \"X\",",
			getRange().c_str(), "file3");
	fprintf(gnuplotPipe, "\"%s\" using 1:3 title \"Y\",", "file3");
	fprintf(gnuplotPipe, "\"%s\" using 1:4 title \"Z\"\n", "file3");

	fprintf(gnuplotPipe, "\n");

	fprintf(gnuplotPipe, "set term x11 4\n");
	fprintf(gnuplotPipe, "plot [] %s \"%s\" using 1:2 title \"X\",",
			getRange().c_str(), "file4");
	fprintf(gnuplotPipe, "\"%s\" using 1:3 title \"Y\",", "file4");
	fprintf(gnuplotPipe, "\"%s\" using 1:4 title \"Z\"\n", "file4");

	fprintf(gnuplotPipe, "\n");

	fprintf(gnuplotPipe, "set term x11 5\n");
	fprintf(gnuplotPipe, "plot [] %s \"%s\" using 1:2 title \"X\",",
			getRange().c_str(), "file5");
	fprintf(gnuplotPipe, "\"%s\" using 1:3 title \"Y\",", "file5");
	fprintf(gnuplotPipe, "\"%s\" using 1:4 title \"Z\"\n", "file5");

	fprintf(gnuplotPipe, "q\n");
}

void MeasurmentsHandler::plotResultsInSeparateCharts() {
	FILE* gnuplotPipe = openGnuplot();

	fprintf(gnuplotPipe, "plot [] %s \"%s\" using 1:2 title \"X\"\n",
			getRange().c_str(), filename);
	fprintf(gnuplotPipe, "set term x11 2\n");
	fprintf(gnuplotPipe, "plot %s [-2:2] \"%s\" using 1:3 title \"Y\"\n",
			getRange().c_str(), filename);
	fprintf(gnuplotPipe, "set term x11 3\n");
	fprintf(gnuplotPipe, "plot [] %s \"%s\" using 1:4 title \"Z\"\n",
			getRange().c_str(), filename);

	fprintf(gnuplotPipe, "q\n");
}

std::string MeasurmentsHandler::getRange(){
	std::stringstream ss;
	ss << "[-" << sensor.getMultiplier() * 2 << ":" << sensor.getMultiplier()*2 << "]";
	return ss.str();
}

void MeasurmentsHandler::getCurrentTime(timeval& toSet) {
	gettimeofday(&toSet, NULL);
}

long MeasurmentsHandler::getCurrentTimeInMilis() {
	timeval temp;
	gettimeofday(&temp, NULL);
	return temp.tv_sec * 1000 + temp.tv_usec / 1000;
}

FILE* MeasurmentsHandler::openGnuplot() {
	FILE* result = popen("gnuplot -persistent", "w");
	fprintf(result, "set style data lines\n");
	fprintf(result, "set terminal x11\n");
	fflush(result);
	return result;
}

void MeasurmentsHandler::storeResult(long currTime) {
	double x = sensor.getAccelX();
	double y = sensor.getAccelY();
	double z = sensor.getAccelZ();
	addToTempList(currTime, x, y, z);
}

void MeasurmentsHandler::addToTempList(long currTime, double x, double y,
		double z) {
	//std::cout << "Adding to tempList" << std::endl;
	//std::cout << "addToTempList" << std::endl;
	//std::cout << currTime << ": " << x << std::endl;
	templist1.push_back(inz::Measurement(currTime, x, y, z));
	if (templist1.size() > 2)
		templist1.pop_front();
	templist2.push_back(inz::Measurement(currTime, x, y, z));
	if (templist2.size() > 2)
		templist2.pop_front();
	templist3.push_back(inz::Measurement(currTime, x, y, z));
	if (templist3.size() > 2)
		templist3.pop_front();
	templist4.push_back(inz::Measurement(currTime, x, y, z));
	if (templist4.size() > 2)
		templist4.pop_front();
	templist5.push_back(inz::Measurement(currTime, x, y, z));
	if (templist5.size() > 2)
		templist5.pop_front();


	if (currTime >= lastResultTime + period) {
		lastResultTime += period;
		addToResultsForConvolution();
	}
}

void MeasurmentsHandler::addToResultsForConvolution() {
	//std::cout << "Adding to ResultsForConvolution" << std::endl;
	std::list<inz::Measurement>::iterator it_begin = templist1.begin();
	std::list<inz::Measurement>::iterator it_end = templist1.end();
	int factor_end = lastResultTime - it_begin->time;
	int factor_begin = it_end->time - lastResultTime;
	int delta = it_end->time - it_begin->time;
	double x = ((double) factor_begin * it_begin->accelX
			+ (double) factor_end * it_end->accelX) / delta;
	double y = ((double) factor_begin * it_begin->accelY
			+ (double) factor_end * it_end->accelY) / delta;
	double z = ((double) factor_begin * it_begin->accelZ
			+ (double) factor_end * it_end->accelZ) / delta;
	//std::cout << "addToResultsForConvolution" << std::endl;
	//std::cout << it_begin->time << ": " << it_begin->accelX << std::endl;
	//std::cout << it_end->time << ": " << it_end->accelX << std::endl;
	//std::cout << lastResultTime << ": " << x << std::endl;
	resultsForConvolution1.push_back(inz::Measurement(lastResultTime, x, y, z));
	if (resultsForConvolution1.size() > convolutionFactors1.size()) {
		resultsForConvolution1.pop_front();
		//addToConvultionResults();
	}


	//std::cout << "Adding to ResultsForConvolution" << std::endl;
	it_begin = templist2.begin();
	it_end = templist2.end();
	factor_end = lastResultTime - it_begin->time;
	factor_begin = it_end->time - lastResultTime;
	delta = it_end->time - it_begin->time;
	 x = ((double) factor_begin * it_begin->accelX
			+ (double) factor_end * it_end->accelX) / delta;
	 y = ((double) factor_begin * it_begin->accelY
			+ (double) factor_end * it_end->accelY) / delta;
	 z = ((double) factor_begin * it_begin->accelZ
			+ (double) factor_end * it_end->accelZ) / delta;
	//std::cout << "addToResultsForConvolution" << std::endl;
	//std::cout << it_begin->time << ": " << it_begin->accelX << std::endl;
	//std::cout << it_end->time << ": " << it_end->accelX << std::endl;
	//std::cout << lastResultTime << ": " << x << std::endl;
	resultsForConvolution2.push_back(inz::Measurement(lastResultTime, x, y, z));
	if (resultsForConvolution2.size() > convolutionFactors2.size()) {
		resultsForConvolution2.pop_front();
		//addToConvultionResults();
	}

	//std::cout << "Adding to ResultsForConvolution" << std::endl;
	it_begin = templist3.begin();
	it_end = templist3.end();
	 factor_end = lastResultTime - it_begin->time;
	 factor_begin = it_end->time - lastResultTime;
	 delta = it_end->time - it_begin->time;
	 x = ((double) factor_begin * it_begin->accelX
			+ (double) factor_end * it_end->accelX) / delta;
	 y = ((double) factor_begin * it_begin->accelY
			+ (double) factor_end * it_end->accelY) / delta;
	 z = ((double) factor_begin * it_begin->accelZ
			+ (double) factor_end * it_end->accelZ) / delta;
	//std::cout << "addToResultsForConvolution" << std::endl;
	//std::cout << it_begin->time << ": " << it_begin->accelX << std::endl;
	//std::cout << it_end->time << ": " << it_end->accelX << std::endl;
	//std::cout << lastResultTime << ": " << x << std::endl;
	resultsForConvolution3.push_back(inz::Measurement(lastResultTime, x, y, z));
	if (resultsForConvolution3.size() > convolutionFactors3.size()) {
		resultsForConvolution3.pop_front();
		//addToConvultionResults();
	}

	//std::cout << "Adding to ResultsForConvolution" << std::endl;
	it_begin = templist4.begin();
	it_end = templist4.end();
	 factor_end = lastResultTime - it_begin->time;
	 factor_begin = it_end->time - lastResultTime;
	 delta = it_end->time - it_begin->time;
	 x = ((double) factor_begin * it_begin->accelX
			+ (double) factor_end * it_end->accelX) / delta;
	 y = ((double) factor_begin * it_begin->accelY
			+ (double) factor_end * it_end->accelY) / delta;
	 z = ((double) factor_begin * it_begin->accelZ
			+ (double) factor_end * it_end->accelZ) / delta;
	//std::cout << "addToResultsForConvolution" << std::endl;
	//std::cout << it_begin->time << ": " << it_begin->accelX << std::endl;
	//std::cout << it_end->time << ": " << it_end->accelX << std::endl;
	//std::cout << lastResultTime << ": " << x << std::endl;
	resultsForConvolution4.push_back(inz::Measurement(lastResultTime, x, y, z));
	if (resultsForConvolution4.size() > convolutionFactors4.size()) {
		resultsForConvolution4.pop_front();
		//addToConvultionResults();
	}

	//std::cout << "Adding to ResultsForConvolution" << std::endl;
	it_begin = templist5.begin();
	it_end = templist5.end();
	 factor_end = lastResultTime - it_begin->time;
	 factor_begin = it_end->time - lastResultTime;
	 delta = it_end->time - it_begin->time;
	 x = ((double) factor_begin * it_begin->accelX
			+ (double) factor_end * it_end->accelX) / delta;
	 y = ((double) factor_begin * it_begin->accelY
			+ (double) factor_end * it_end->accelY) / delta;
	 z = ((double) factor_begin * it_begin->accelZ
			+ (double) factor_end * it_end->accelZ) / delta;
	//std::cout << "addToResultsForConvolution" << std::endl;
	//std::cout << it_begin->time << ": " << it_begin->accelX << std::endl;
	//std::cout << it_end->time << ": " << it_end->accelX << std::endl;
	//std::cout << lastResultTime << ": " << x << std::endl;
	resultsForConvolution5.push_back(inz::Measurement(lastResultTime, x, y, z));
	if (resultsForConvolution5.size() > convolutionFactors5.size()) {
		resultsForConvolution5.pop_front();
		addToConvultionResults();
	}

}

void MeasurmentsHandler::addToConvultionResults() {
	//std::cout << "Adding to Convolution results" << std::endl;
	double x, y, z;
	std::list<inz::Measurement>::iterator listIt;
	std::vector<double>::iterator factorIt;
	for (listIt = resultsForConvolution1.begin(), factorIt =
			convolutionFactors1.begin(); listIt != resultsForConvolution1.end();
			listIt++, factorIt++) {
		x += listIt->accelX * (*factorIt);
		y += listIt->accelY * (*factorIt);
		z += listIt->accelZ * (*factorIt);
	}
	convolutionResults1.push_back(inz::Measurement(lastResultTime, x, y, z));

	x=y=z=0.0;
	for (listIt = resultsForConvolution2.begin(), factorIt =
			convolutionFactors2.begin(); listIt != resultsForConvolution2.end();
			listIt++, factorIt++) {
		x += listIt->accelX * (*factorIt);
		y += listIt->accelY * (*factorIt);
		z += listIt->accelZ * (*factorIt);
	}
	convolutionResults2.push_back(inz::Measurement(lastResultTime, x, y, z));
	x=y=z=0.0;
	for (listIt = resultsForConvolution3.begin(), factorIt =
			convolutionFactors3.begin(); listIt != resultsForConvolution3.end();
			listIt++, factorIt++) {
		x += listIt->accelX * (*factorIt);
		y += listIt->accelY * (*factorIt);
		z += listIt->accelZ * (*factorIt);
	}
	convolutionResults3.push_back(inz::Measurement(lastResultTime, x, y, z));
	x=y=z=0.0;
	for (listIt = resultsForConvolution4.begin(), factorIt =
			convolutionFactors4.begin(); listIt != resultsForConvolution4.end();
			listIt++, factorIt++) {
		x += listIt->accelX * (*factorIt);
		y += listIt->accelY * (*factorIt);
		z += listIt->accelZ * (*factorIt);
	}
	convolutionResults4.push_back(inz::Measurement(lastResultTime, x, y, z));
	x=y=z=0.0;
	for (listIt = resultsForConvolution5.begin(), factorIt =
			convolutionFactors5.begin(); listIt != resultsForConvolution5.end();
			listIt++, factorIt++) {
		x += listIt->accelX * (*factorIt);
		y += listIt->accelY * (*factorIt);
		z += listIt->accelZ * (*factorIt);
	}
	convolutionResults5.push_back(inz::Measurement(lastResultTime, x, y, z));
}

MeasurmentsHandler::~MeasurmentsHandler() {
	of1.close();
	of2.close();
	of3.close();
	of4.close();
	of5.close();
}

