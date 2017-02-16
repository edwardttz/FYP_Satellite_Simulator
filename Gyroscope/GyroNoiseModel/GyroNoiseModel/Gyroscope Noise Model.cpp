#include "Gyroscope Noise Model.h"
#include <string>
#include <iostream>
#include <ctime>
#include <math.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

double GyroNoiseModel::wX_noise;
double GyroNoiseModel::wY_noise;
double GyroNoiseModel::wZ_noise;
vector <double> GyroNoiseModel::allanDeviationValues;
vector <double> GyroNoiseModel::randomGyroValues;
vector <double> GyroNoiseModel::thetaValues;
int GyroNoiseModel::fsValue;
double GyroNoiseModel::samplePeriod;
long int GyroNoiseModel::noOfSamples;
double GyroNoiseModel::maxNonLinearity;
double GyroNoiseModel::minNonLinearity;

GyroNoiseModel::GyroNoiseModel() {
	wXNoise.setFileName("wX Noise.txt");
	wYNoise.setFileName("wY Noise.txt");
	wZNoise.setFileName("wZ Noise.txt");
	allanDeviation.setFileName("Allan Deviation.txt");
	allanDeviation.clearFile();
	wXNoise.clearFile();
	wYNoise.clearFile();
	wZNoise.clearFile();
}

void GyroNoiseModel::getUserInput() {
	cout << "Enter gyroscope full scale value: " << endl;
	cin >> fsValue;
}

double GyroNoiseModel::findNoiseW(double value) {
	return value + randomNoiseValue();
}

double GyroNoiseModel::getRealwX() {
	return wX_noise;
}

double GyroNoiseModel::getRealwY() {
	return wY_noise;
}

double GyroNoiseModel::getRealwZ() {
	return wZ_noise;
}

/*void GyroNoiseModel::findRealW() {
	double wX, wY, wZ;
	wX = s1.getVelocityX();
	wY = s1.getVelocityY();
	wZ = s1.getVelocityZ();
	wX_noise = findNoiseW(wX);
	wY_noise = findNoiseW(wY);
	wZ_noise = findNoiseW(wZ);
}*/

/*void GyroNoiseModel::storeValues() {
	wXNoise.storeInFile(wX_noise);
	wYNoise.storeInFile(wY_noise);
	wZNoise.storeInFile(wZ_noise);
}*/

void GyroNoiseModel::storeAllanDeviation() {
	allanDeviation.storeAllInFile(allanDeviationValues);
}

void GyroNoiseModel::setFSValue(int value) {
	fsValue = value;
	setMinNonLinearity();
	setMaxNonLinearity();
}

int GyroNoiseModel::getFSValue() {
	return fsValue;
}

void GyroNoiseModel::setMaxNonLinearity() {
	maxNonLinearity = (0.2 / 100)*fsValue;
}

void GyroNoiseModel::setMinNonLinearity() {
	minNonLinearity = -(0.2 / 100)*fsValue;
}

double GyroNoiseModel::randomNoiseValue() {
	double f = (double)rand() / RAND_MAX;
	return  minNonLinearity + (f * (maxNonLinearity - minNonLinearity));
}

void GyroNoiseModel::generateRawValues(double min, double max) {
	double randValue;
	srand((unsigned)time(0));
	double f;
	for (long int i = 0; i < noOfSamples; i++)
	{
		f = (double)rand() / RAND_MAX;
		randValue = min + (f * (max - min));
		randomGyroValues.push_back(randValue);
	}
}

void GyroNoiseModel::setSamplePeriod(double period) {
	samplePeriod = period;
	findNoOfSamples();
}

double GyroNoiseModel::getSamplePeriod() {
	return samplePeriod;
}

void GyroNoiseModel::findNoOfSamples() {
	noOfSamples = (long int) (2 * 60 * 60) / samplePeriod;
	//noOfSamples = (long int)(391) / samplePeriod;
}

void GyroNoiseModel::calculateThetaValues() {
	double accumGyroRate = 0;
	double thetaValue;
	for (long int i = 0; i<randomGyroValues.size(); i++)
	{
		accumGyroRate = accumGyroRate + randomGyroValues[i];
		thetaValue = accumGyroRate*samplePeriod;
		thetaValues.push_back(thetaValue);
	}
}

void GyroNoiseModel::calculateAllanDeviation(int m) {
	double averagingTimeTau = (double) m*samplePeriod;
	long int noOfSummations = (noOfSamples - (2 * m));
	double constant = 1 / (2 * (averagingTimeTau*averagingTimeTau)*(noOfSummations));
	double tempSumValue=0;
	double tempValue;
	double allanVarianceAtTau;
	double allanDeviation;
	for (int k = 0; k < noOfSummations; k++) { //no of summations = 19496 
		tempValue = thetaValues[k + (2 * m)] - (2 * (thetaValues[k + m])) + thetaValues[k];
		tempSumValue = tempSumValue + (tempValue*tempValue);
	}
	allanVarianceAtTau = constant*tempSumValue;
	allanDeviation = sqrt(allanVarianceAtTau);
	allanDeviationValues.push_back(allanDeviation);
}

void GyroNoiseModel::findAllanDeviation() {
	generateRawValues(-0.36, 0.36);
	//findRawValues("raw valuesX.txt");
	//findRawValues("raw valuesY.txt");
	//findRawValues("raw valuesZ.txt");
	calculateThetaValues();
	int initialM = 10;
	//int initialM = 1;
	for (int i = initialM; i < 1000; i=i+initialM) { //i<195 for raw
		calculateAllanDeviation(i);
	}
	storeAllanDeviation();
}

void GyroNoiseModel::findRawValues(string fileName) {
	double value;
	int i = 0;
	ifstream myFile(fileName);
	if (myFile.is_open())
	{
		while (myFile >> value)
		{
			randomGyroValues.push_back(value);
		}
		myFile.close();
	}
}

