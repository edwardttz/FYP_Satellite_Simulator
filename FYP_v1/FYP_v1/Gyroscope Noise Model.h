#ifndef GYROSCOPE_NOISE_MODEL_H
#define GYROSCOPE_NOISE_MODEL_H

#include <string>
#include "SpacecraftDynamics.h"
#include "Storage.h"
using namespace std;

class GyroNoiseModel
{
private:
	static double wX_noise;
	static double wY_noise;
	static double wZ_noise;
	static int fsValue;
	static double samplePeriod;
	static long int noOfSamples;
	static double maxNonLinearity;
	static double minNonLinearity;
	static vector <double> allanDeviationValues;
	static vector <double> randomGyroValues;
	static vector <double> thetaValues;
	SpacecraftDynamics s1;
	Storage wXNoise;
	Storage wYNoise;
	Storage wZNoise;
	Storage allanDeviation;

public:
	GyroNoiseModel();
	void getUserInput();
	void setFSValue(int);
	int getFSValue();
	void setMaxNonLinearity();
	void setMinNonLinearity();
	double randomNoiseValue();
	void getRandomNoise();
	double getRealwX();
	double getRealwY();
	double getRealwZ();
	double findNoiseW(double);
	void findRealW();
	void storeValues();
	void storeAllanDeviation();
	void findAllanDeviation();
	void generateRawValues(double, double);
	void calculateThetaValues();
	void setSamplePeriod(double);
	double getSamplePeriod();
	void findNoOfSamples();
	void calculateAllanDeviation(int);

};



#endif