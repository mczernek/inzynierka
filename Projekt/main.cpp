#include "i2c8Bit.h"
#include "mpu6050.h"
#include "MeasurmentsHandler.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sys/time.h>

using namespace std;

bool isForMultiCharts = false;
bool returnsOnKey = false;

void processParameters(string params);
 
int main(int argc, const char** argv)
{
    const char* filename = "measure.txt";
    long timestamp = 10;
    int filterStrength = 8;
    
    if(argc>1){
        filename = argv[1];
    }
    
    if(argc>2){
         timestamp = atoi(argv[2]);
    }
    
    if(argc>3){
    	filterStrength = atoi(argv[3]);
    }

    if(argc>4){
    	processParameters(argv[4]);
    }

    mpu6050 sensor;

    MeasurmentsHandler mHandler(sensor, filename, filterStrength);
    if(timestamp!=0){
    	mHandler.measure(timestamp);
    }else{
    	mHandler.measureUntilKeyPressed('q');
    }
    mHandler.storeValuesToFile();

    if(isForMultiCharts){
    	mHandler.plotResultsInSeparateCharts();
    }else{
    	mHandler.plotResultsOnOneChart();
    }


}

void processParameters(string params){
	std::cout << params << endl;
	if(params[0] != '-' || params[1] == '-')
		return;
	if(params.find('s') != string::npos || params.find('S') != string::npos){
		isForMultiCharts = true;
	}
}


