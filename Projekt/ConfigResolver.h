/*
 * ConfigResolver.h
 *
 *  Created on: 9 gru 2013
 *      Author: mis3k
 */

#ifndef CONFIGRESOLVER_H_
#define CONFIGRESOLVER_H_

#include <fstream>

class ConfigResolver {

	std::ifstream is;

	int accRange;
	int timestamp;
	std::string fileToStore;
	int filterGrade;
	bool seperate;

	void getConfigurationFromFile();
	void processLine(std::string&);
	void setValues(std::string& before, std::string& after);

	void processAccRange(std::string&);
	void processTime(std::string&);
	void processFilename(std::string&);
	void processSeperate(std::string&);
	void processFilter(std::string&);

public:
	ConfigResolver(const char* filename);
	virtual ~ConfigResolver();

	int getAccRange(){
		return accRange;
	}

	int getFilterGrade(){
		return filterGrade;
	}

	std::string getFileToStore(){
		return fileToStore;
	}

	bool isSeperate(){
		return seperate;
	}

	int getTimestamp(){
		return timestamp;
	}
};

#endif /* CONFIGRESOLVER_H_ */
