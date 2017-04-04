/*This project calculates the ideal angular velocity values, and then calculates the real/noisy angular velocity values using the
gyroscope noise model.*/

#include <iostream>
#include <string>
#include <cmath>
#include <ctime>
#include "Gyroscope Noise Model.h"
#include "Storage.h"

using namespace std;

int main(void) 
{
	GyroNoiseModel g1;

	//Specifications for gyroscope L3G4200D
	g1.setSamplePeriod(0.02); //sample period is 1 when testing with real gyro values
	g1.setFSValue(500); //Full scale of gyro 
	
	//For the rate noise density
	g1.setDataGenTime(2); //Time for which AD data is generated
	g1.setMaxNoiseDensity(0.37); 
	g1.setMinNoiseDensity(-0.37);
	
	//For integration
	
	/* For integration, the following functions will calculate and return the real angular velocity at each time step
	g1.setPollingTime(0.05); 
	g1.getRealW();
	g1.getRealwX();
	g1.getRealwX();
	g1.getRealwX();
	*/

	
	/*For testing purposes*/
	g1.testGyroModel();
	//g1.testConstW();
	//g1.testReverseNoiseModel("raw valuesX.txt");
	//g1.generateRawARW();
	//g1.kalmanFilter("filter values.txt");
	//g1.findRealW();
	//g1.findAllanDeviation();
	
	system("PAUSE");
}