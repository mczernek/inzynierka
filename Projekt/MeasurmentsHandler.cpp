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

using namespace std;

MeasurmentsHandler::MeasurmentsHandler(mpu6050& sensor, const char* filename, int filterStrength) :
		of(filename), sensor(sensor), filename(filename), period(10), lastResultTime(0) {
	starttime = 0;
	givenKeyWasPressed = false;

	for(int i=0; i<filterStrength; i++){
		convolutionFactors.push_back((double)1/filterStrength);
	}

	/*for(int i=0; i<100; ++i){
		convolutionFactors.push_back(1.0-((double)i)*0.00020202);
	}*/

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
		of << "Interrupt caught!" << std::endl;
	}
	if (sensor.isDataInterrput()) {
		storeResult(currTime);
	}
}

void MeasurmentsHandler::storeValuesToFile(){
	std::cout << convolutionResults.size() << std::endl;
	for(std::list<inz::Measurement>::iterator it = convolutionResults.begin(); it != convolutionResults.end(); it++){
		of << it->toPrint();
	}
}

void MeasurmentsHandler::plotResultsOnOneChart() {
	FILE* gnuplotPipe = openGnuplot();

	fprintf(gnuplotPipe, "plot [] [-2:2] \"%s\" using 1:2 title \"X\",",
			filename);
	fprintf(gnuplotPipe, "\"%s\" using 1:3 title \"Y\",", filename);
	fprintf(gnuplotPipe, "\"%s\" using 1:4 title \"Z\"\n", filename);

	fprintf(gnuplotPipe, "q\n");
}

void MeasurmentsHandler::plotResultsInSeparateCharts() {
	FILE* gnuplotPipe = openGnuplot();

	fprintf(gnuplotPipe, "plot [] [-2:2] \"%s\" using 1:2 title \"X\"\n",
			filename);
	fprintf(gnuplotPipe, "set term x11 2\n");
	fprintf(gnuplotPipe, "plot [] [-2:2] \"%s\" using 1:3 title \"Y\"\n",
			filename);
	fprintf(gnuplotPipe, "set term x11 3\n");
	fprintf(gnuplotPipe, "plot [] [-2:2] \"%s\" using 1:4 title \"Z\"\n",
			filename);

	fprintf(gnuplotPipe, "q\n");
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

void MeasurmentsHandler::storeResult(long currTime){
	double x = sensor.getAccelX();
	double y = sensor.getAccelY();
	double z = sensor.getAccelZ();
	addToTempList(currTime, x, y, z);
}

void MeasurmentsHandler::addToTempList(long currTime, double x, double y, double z){
	//std::cout << "Adding to tempList" << std::endl;
	//std::cout << "addToTempList" << std::endl;
	//std::cout << currTime << ": " << x << std::endl;
	templist.push_back(inz::Measurement(currTime, x, y, z));
	if(templist.size() > 2)
		templist.pop_front();
	if(currTime >= lastResultTime + period){
		lastResultTime += period;
		addToResultsForConvolution();
	}
}

void MeasurmentsHandler::addToResultsForConvolution(){
	//std::cout << "Adding to ResultsForConvolution" << std::endl;
	std::list<inz::Measurement>::iterator it_begin = templist.begin();
	std::list<inz::Measurement>::iterator it_end = templist.end();
	int factor_end = lastResultTime - it_begin->time;
	int factor_begin = it_end->time - lastResultTime;
	int delta = it_end->time - it_begin->time;
	double x = ((double)factor_begin * it_begin->accelX + (double)factor_end * it_end->accelX) / delta;
	double y = ((double)factor_begin * it_begin->accelY + (double)factor_end * it_end->accelY) / delta;
	double z = ((double)factor_begin * it_begin->accelZ + (double)factor_end * it_end->accelZ) / delta;
	//std::cout << "addToResultsForConvolution" << std::endl;
	//std::cout << it_begin->time << ": " << it_begin->accelX << std::endl;
	//std::cout << it_end->time << ": " << it_end->accelX << std::endl;
	//std::cout << lastResultTime << ": " << x << std::endl;
	resultsForConvolution.push_back(inz::Measurement(lastResultTime, x, y, z));
	if(resultsForConvolution.size() > convolutionFactors.size()){
		resultsForConvolution.pop_front();
		addToConvultionResults();
	}
}

void MeasurmentsHandler::addToConvultionResults(){
	//std::cout << "Adding to Convolution results" << std::endl;
	double x, y, z;
	std::list<inz::Measurement>::iterator listIt;
	std::vector<double>::iterator factorIt;
	for(listIt = resultsForConvolution.begin(), factorIt = convolutionFactors.begin(); listIt != resultsForConvolution.end(); listIt++, factorIt++){
		x += listIt->accelX * (*factorIt);
		y += listIt->accelY * (*factorIt);
		z += listIt->accelZ * (*factorIt);
	}
	convolutionResults.push_back(inz::Measurement(lastResultTime, x, y, z));
}

MeasurmentsHandler::~MeasurmentsHandler() {
	// Nothing to do so far
}

