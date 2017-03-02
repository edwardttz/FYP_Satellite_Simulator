/*Point of entry for gyro noise model*/

#include <iostream>
#include <string>
#include <cmath>
#include "Gyroscope Noise Model.h"
#include "Storage.h"

using namespace std;

int main(void) 
{
	/*
	Leslie :
	Copy all of the main function into your code. It will calculate my values which are wX, wY and wZ after noise is added.
	After it runs, to access the data you can use the GyroNoiseModel object g1 and call the following:
		For wX value - g1.getRealwX() [Returns you double type wX]
		For wY value - g1.getRealwY() [Returns you double type wY]
		For wZ value - g1.getRealwZ() [Returns you double type wZ]
	Also this program assumes you are running at 50ms. Please do tell me if it changes as I will have to update my side too.
	*/

	GyroNoiseModel g1;
	g1.setPollingTime(0.05); //Polling time by default is 50ms
	g1.setSamplePeriod(0.02); //sample period 1 for raw gyro values
	g1.setFSValue(250); //Full scale of gyro by default = 250
	g1.setMaxNoiseDensity(0.37); 
	g1.setMinNoiseDensity(-0.37);
	g1.findRealW();
	//g1.findAllanDeviation();

	/*
	For testing purposes
	g1.testGyroModel();
	*/

	system("PAUSE");
}