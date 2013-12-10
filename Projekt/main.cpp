//#include <stdio.h>
//#include <sys/time.h>
#include <cstdlib>
//#include <fstream>
#include <iostream>
#include <string>

//#include "i2c8Bit.h"
#include "mpu6050.h"
#include "MeasurmentsHandler.h"
#include "ConfigResolver.h"

using namespace std;
 
int main(int argc, const char** argv)
{
	ConfigResolver cr("config.cfg");

	int timestamp = cr.getTimestamp();

    mpu6050 sensor(cr.getAccRange());

    MeasurmentsHandler mHandler(sensor, cr.getFileToStore().c_str(), cr.getFilterGrade(), cr.getAccRange());
    if(timestamp!=0){
    	mHandler.measure(timestamp);
    }else{
    	mHandler.measureUntilKeyPressed('q');
    }
    mHandler.storeValuesToFile();

    if(cr.isSeperate()){
    	mHandler.plotResultsInSeparateCharts();
    }else{
    	mHandler.plotResultsOnOneChart();
    }

}


