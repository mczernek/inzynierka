/*
 * ConfigResolver.cpp
 *
 *  Created on: 9 gru 2013
 *      Author: mis3k
 */

#include <iostream>
#include <string>
#include "ConfigResolver.h"

ConfigResolver::ConfigResolver(const char* filename) :
		is(filename), timestamp(10), fileToStore("results.cpp"), filterGrade(0), seperate(
				false), accRange(0) {
	getConfigurationFromFile();
}

void ConfigResolver::getConfigurationFromFile() {
	while (!is.eof()) {
		std::string toProcess;
		std::getline(is, toProcess);
		processLine(toProcess);
	}
}

void ConfigResolver::processLine(std::string& toProcess) {
	if (toProcess[0] == '#' || toProcess.size() == 0) {
		return;
	} else {
		int equalSign = toProcess.find('=');
		std::string beforeEqual = toProcess.substr(0, equalSign);
		std::string afterEqual = toProcess.substr(equalSign + 1,
				toProcess.size() - equalSign);
		setValues(beforeEqual, afterEqual);
	}
}

void ConfigResolver::setValues(std::string& before, std::string& after) {
	if (before == "ACCELEROMETER_RANGE") {
		processAccRange(after);
	} else if (before == "TIME") {
		processTime(after);
	} else if (before == "FILENAME") {
		processFilename(after);
	} else if (before == "SEPERATE") {
		processSeperate(after);
	}else if (before == "FILTER") {
		processFilter(after);
	}
	std::cout << "Before: " << before << ", after: " << after << std::endl;
}

void ConfigResolver::processAccRange(std::string& value) {
	accRange = std::atoi(value.c_str());
	std::cout << "Accelerometer range set to: " << accRange << std::endl;
}

void ConfigResolver::processTime(std::string& value) {
	timestamp = std::atoi(value.c_str());
	std::cout << "Time set to: " << timestamp << std::endl;
}

void ConfigResolver::processFilename(std::string& value) {
	fileToStore = value;
	std::cout << "Filename set to: " << fileToStore << std::endl;
}

void ConfigResolver::processSeperate(std::string& value){
	if(value=="TRUE"){
		seperate=true;
	}
	std::cout << "Seperate charts set to: " << seperate << std::endl;
}

void ConfigResolver::processFilter(std::string& value){
	filterGrade = std::atoi(value.c_str());
	std::cout << "Filter grade set to: " << filterGrade << std::endl;
}

ConfigResolver::~ConfigResolver() {
	is.close();
}
