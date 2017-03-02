#include "Gyroscope Noise Model.h"
#include <string>
#include <iostream>
#include <ctime>
#include <math.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

double GyroNoiseModel::pollingTime;
int GyroNoiseModel::numberOfNoiseGenerations;
vector <double> GyroNoiseModel::realWValues;
double GyroNoiseModel::wXreal;
double GyroNoiseModel::wYreal;
double GyroNoiseModel::wZreal;
double GyroNoiseModel::accumNoise;
double GyroNoiseModel::wXideal;
double GyroNoiseModel::wYideal;
double GyroNoiseModel::wZideal;
vector <double> GyroNoiseModel::allanDeviationValues;
vector <double> GyroNoiseModel::randomGyroValues;
vector <double> GyroNoiseModel::thetaValues;
int GyroNoiseModel::fsValue;
double GyroNoiseModel::samplePeriod;
long int GyroNoiseModel::noOfSamples;
double GyroNoiseModel::maxNonLinearity;
double GyroNoiseModel::minNonLinearity;
double GyroNoiseModel::maxNoiseDensity;
double GyroNoiseModel::minNoiseDensity;

GyroNoiseModel::GyroNoiseModel() {
	wXRealValues.setFileName("wX Noise.txt");
	wYRealValues.setFileName("wY Noise.txt");
	wZRealValues.setFileName("wZ Noise.txt");
	allanDeviation.setFileName("Allan Deviation.txt");
	allanDeviation.clearFile();
	wXRealValues.clearFile();
	wYRealValues.clearFile();
	wZRealValues.clearFile();
	setAccumNoise(0.0);
	numberOfNoiseGenerations = 2;
}

double GyroNoiseModel::getPollingTime() {
	return pollingTime;
}

void GyroNoiseModel::setPollingTime(double value) {
	pollingTime = value;
}

void GyroNoiseModel::getUserInput() {
	cout << "Enter gyroscope full scale value: " << endl;
	cin >> fsValue;
}

double GyroNoiseModel::getRealwX() {
	return wXreal;
}

double GyroNoiseModel::getRealwY() {
	return wYreal;
}

double GyroNoiseModel::getRealwZ() {
	return wZreal;
}

void GyroNoiseModel::setRealwX(double value) {
	wXreal = value;
}

void GyroNoiseModel::setRealwY(double value) {
	wYreal = value;
}

void GyroNoiseModel::setRealwZ(double value) {
	wZreal = value;
}

double GyroNoiseModel::getIdealwX() {
	return wXideal;
}

double GyroNoiseModel::getIdealwY() {
	return wYideal;
}

double GyroNoiseModel::getIdealwZ() {
	return wZideal;
}

void GyroNoiseModel::setIdealwX() {
	wXideal = s1.getVelocityX();
}

void GyroNoiseModel::setIdealwY() {
	wYideal = s1.getVelocityY();
}

void GyroNoiseModel::setIdealwZ() {
	wZideal = s1.getVelocityZ();
}

void GyroNoiseModel::setFSValue(int value) {
	fsValue = value;
	setMinNonLinearity();
	setMaxNonLinearity();
}

int GyroNoiseModel::getFSValue() {
	return fsValue;
}

void GyroNoiseModel::setSamplePeriod(double period) {
	samplePeriod = period;
	findNoOfSamples();
}

double GyroNoiseModel::getSamplePeriod() {
	return samplePeriod;
}

double GyroNoiseModel::getAccumNoise() {
	return accumNoise;
}

void GyroNoiseModel::setAccumNoise(double noiseValue) {
	accumNoise = noiseValue;
}

double GyroNoiseModel::generateRandomNumber(double min, double max) {
	srand((unsigned)time(0));
	double f;
	f = (double)rand() / RAND_MAX;
	return (min + (f * (max - min)));
}

/*------------------------------------------------------------------------------------------------------*/
/*Non Linearity*/

void GyroNoiseModel::setMaxNonLinearity() {
	maxNonLinearity = (0.2 / 100)*fsValue;
}

void GyroNoiseModel::setMinNonLinearity() {
	minNonLinearity = -(0.2 / 100)*fsValue;
}

double GyroNoiseModel::nonLinearityValue() { //Calculates noise value between the max and min
	return generateRandomNumber(minNonLinearity, maxNonLinearity);
}

double GyroNoiseModel::accumNonLinearityValues() {
	double nonLinearity = 0;
	for (int i = 0;i < numberOfNoiseGenerations;i++) {
		nonLinearity = nonLinearity + nonLinearityValue();
	}
	return nonLinearity;
}

/*------------------------------------------------------------------------------------------------------*/
/*Noise Density*/

void GyroNoiseModel::setMaxNoiseDensity(double value) {
	maxNoiseDensity = value;
}

void GyroNoiseModel::setMinNoiseDensity(double value) {
	minNoiseDensity = value;
}

void GyroNoiseModel::storeAllanDeviation() {
	for (int i = 0;i < allanDeviationValues.size(); i++) {
		allanDeviation.storeInFile(allanDeviationValues[i]);
	}
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
	generateRawValues(-0.37, 0.37); //Range of values of nonlinearity
	//findRawValues("raw valuesX.txt");
	//findRawValues("raw valuesY.txt");
	//findRawValues("raw valuesZ.txt");
	calculateThetaValues();
	int initialM = 10; //M=1 for raw gyro data
	for (int i = initialM; i < 1000; i=i+initialM) { //i<195 for raw gyro data
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

double GyroNoiseModel::noiseDensityValue() { //Calculates noise density between values found using allan variance method
	return generateRandomNumber(minNoiseDensity, maxNoiseDensity);
}

double GyroNoiseModel::accumNoiseDensityValues() {
	double noiseDensity=0;
	for (int i = 0;i < numberOfNoiseGenerations;i++) {
		noiseDensity = noiseDensity + noiseDensityValue();
	}
	return noiseDensity;
}

/*------------------------------------------------------------------------------------------------------*/
/*Final Real W calculation (including all noise)*/

void GyroNoiseModel::findAccumNoise() {
	double totalNoise;
	totalNoise = accumNoise + accumNoiseDensityValues() + accumNonLinearityValues();
	setAccumNoise(totalNoise);
}

void GyroNoiseModel::findRealW() {
	double wX, wY, wZ;
	findAccumNoise();
	wX = getIdealwX() + getAccumNoise();
	wY = getIdealwY() + getAccumNoise();
	wZ = getIdealwZ() + getAccumNoise();
	setRealwX(wX);
	setRealwY(wY);
	setRealwZ(wZ);
	realWValues.push_back(wXreal);
	realWValues.push_back(wYreal);
	realWValues.push_back(wZreal);
	storeRealwValues();
}

void GyroNoiseModel::storeRealwValues() {
	wXRealValues.storeInFile(wXreal);
	wYRealValues.storeInFile(wYreal);
	wZRealValues.storeInFile(wZreal);
}

vector <double> GyroNoiseModel::getRealWValues() {
	return realWValues;
}

void GyroNoiseModel::testGyroModel() {
	//s1.setMOIValues(3.03, 4.85, 2.98); //KR 1
	//s1.setMOIValues(40.45, 42.09, 41.36); //UoSat12
	s1.setMOIValues(3.4, 2.18, 1.68); //MOST

	//s1.setTorque(0.0107, 0.0107, 0.0107); //KR 1
	//s1.setTorque(0.05, 0.05, 0.05); //UoSat12
	s1.setTorque(0.01, 0.01, 0.01); //MOST
	s1.setStepSize(0.02); 
	s1.setInitialW(0.0, 0.0, 0.0);
	s1.findConstants();
	double totalNoise;

	for (int i = 0; i < 9000; i++) //For 3 minutes at 20ms stepsize
		{
		//switching torque off after 30s
			if (i == 1499)
			{
			s1.setTorque(0.0, 0.0, 0.0);
			}

		//negative torque after 30s
			if (i == 2999)
			{
				//s1.setTorque(-0.0107, -0.0107, -0.0107); //KR 1
				//s1.setTorque(-0.05, -0.05, -0.05); //UoSat12
				s1.setTorque(-0.01, -0.01, -0.01); //MOST
			}

		//switching torque off after 30s
			if (i == 4499)
			{
				s1.setTorque(0.0, 0.0, 0.0);
			}
			s1.findAcc();
			s1.getNextw();
			totalNoise = accumNoise + noiseDensityValue();
			setAccumNoise(totalNoise);
			findRealW();
			cout << i << endl;
			}
	}
