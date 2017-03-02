/*Point of entry for gyro noise model*/

#include <iostream>
#include <string>
#include <cmath>
#include "Gyroscope Noise Model.h"
#include "Storage.h"

using namespace std;

GyroNoiseModel g1;
static vector <double> wValues;

vector <double> getwValues() {
	return g1.getRealWValues();
}

void printwValues() {
	vector <double> wValues = g1.getRealWValues();
	cout << "wX=" << wValues[0] << " , wY=" << wValues[1] << " , wZ=" << wValues[2] << endl;
}

int main(void) 
{
	/*
	Leslie :
	To get the w values call the function:
	g1.getRealWValues()
	It will return you a vector <double> type in the form [wX, wY, wZ]. (Refer to line 14 in main.cpp)
	Also this program assumes you are running at 100ms. Please do tell me if it changes as I will have to update my side too.
	*/

	g1.setPollingTime(0.05); //Polling time by default is 50ms
	g1.setSamplePeriod(0.02); //sample period 1 for raw gyro values
	g1.setFSValue(250); //Full scale of gyro by default = 250
	g1.setMaxNoiseDensity(0.37); 
	g1.setMinNoiseDensity(-0.37);
	g1.findRealW();
	//g1.findAllanDeviation();
	//printwValues();

	/*
	For testing purposes
	g1.testGyroModel();
	*/

	system("PAUSE");
}