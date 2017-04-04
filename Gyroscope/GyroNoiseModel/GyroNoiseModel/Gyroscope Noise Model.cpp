#include "Gyroscope Noise Model.h"
#include <string>
#include <iostream>
#include <ctime>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <Windows.h>

using namespace std;

double GyroNoiseModel::pollingTime;
int GyroNoiseModel::numberOfNoiseGenerations;
int GyroNoiseModel::counterForNoise;
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
double GyroNoiseModel::timeGenerated;
double GyroNoiseModel::maxNonLinearity;
double GyroNoiseModel::minNonLinearity;
double GyroNoiseModel::maxNoiseDensity;
double GyroNoiseModel::minNoiseDensity;

GyroNoiseModel::GyroNoiseModel() {
	wXRealValues.setFileName("wX Noise.txt");
	wYRealValues.setFileName("wY Noise.txt");
	wZRealValues.setFileName("wZ Noise.txt");
	allanDeviation.setFileName("Allan Deviation.txt");
	noiseValues.setFileName("Noise.txt");
	randomValues.setFileName("Random Data AD.txt");
	angleVectorX.setFileName("ARWx.txt");
	angleVectorY.setFileName("ARWy.txt");
	angleVectorZ.setFileName("ARWz.txt");
	reverseGyroModel.setFileName("reverse w values.txt");
	filter.setFileName("kalman filter.txt");
	allanDeviation.clearFile();
	wXRealValues.clearFile();
	wYRealValues.clearFile();
	wZRealValues.clearFile();
	noiseValues.clearFile();
	randomValues.clearFile();
	angleVectorX.clearFile();
	angleVectorY.clearFile();
	angleVectorZ.clearFile();
	reverseGyroModel.clearFile();
	filter.clearFile();
	setAccumNoise(0.0);
	srand((unsigned)time(NULL));
	counterForNoise = 0;
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

double GyroNoiseModel::getDataGenTime() {
	return timeGenerated;
}

void GyroNoiseModel::setDataGenTime(double timeInHours) {
	timeGenerated = timeInHours;
}

void GyroNoiseModel::setAccumNoise(double noiseValue) {
	accumNoise = noiseValue;
}

double GyroNoiseModel::generateRandomNumber(double min, double max) {
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

//Calculates noise value between the max and min
double GyroNoiseModel::nonLinearityValue(double timeStep) { 
	double nonLinValue;
	nonLinValue = generateRandomNumber(minNonLinearity, maxNonLinearity);
	return nonLinValue*timeStep;
}

//Accumulates the noise, used for integration
double GyroNoiseModel::accumNonLinearityValues(double timeStep) {
	double nonLinearity = 0;
	for (int i = 0;i < numberOfNoiseGenerations;i++) {
		nonLinearity = nonLinearity + nonLinearityValue(timeStep);
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

void GyroNoiseModel::storeRandomValues() {
	for (int i = 0; i < randomGyroValues.size(); i++) {
		randomValues.storeInFile(randomGyroValues[i]);
	}
}

void GyroNoiseModel::generateRawValues(double min, double max) {
	double randValue;
	for (long int i = 0; i < noOfSamples; i++) 
	{
		randValue = generateRandomNumber(min, max);
		randomGyroValues.push_back(randValue);
	}
}

void GyroNoiseModel::findNoOfSamples() {
	noOfSamples = (long int) (timeGenerated * 60 * 60) / samplePeriod; 
	//noOfSamples = (long int)(391) / samplePeriod; //To generate AD plot for real data from gyroscope
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
	double averagingTimeTau = (double) m*samplePeriod; //10*0.02=0.2
	long int noOfSummations = (noOfSamples - (2 * m)); //360000 - 20 = 359980
	double constant = 1 / (2 * (averagingTimeTau*averagingTimeTau)*(noOfSummations)); //6.94 * 10^-5
	double tempSumValue=0;
	double tempValue;
	double allanVarianceAtTau;
	double allanDeviation;
	for (int k = 0; k < noOfSummations; k++) { 
		tempValue = thetaValues[k + (2 * m)] - (2 * (thetaValues[k + m])) + thetaValues[k];
		tempSumValue = tempSumValue + (tempValue*tempValue);
	}
	allanVarianceAtTau = constant*tempSumValue;
	allanDeviation = sqrt(allanVarianceAtTau);
	allanDeviationValues.push_back(allanDeviation);
}

void GyroNoiseModel::findAllanDeviation() {
	srand((unsigned)time(0));
	generateRawValues(-0.37, 0.37); 
	/* For AD of real gyro data
	findRawValues("raw valuesX.txt");
	findRawValues("raw valuesY.txt");
	findRawValues("raw valuesZ.txt");
	*/
	calculateThetaValues();
	int initialM = 10;  
	int i;
	for (i = initialM; i < 1000; i=i+initialM) { 
		calculateAllanDeviation(i);
		cout << i << endl;
	}
	//storeRandomValues(); //AD data for the real gyroscope data
	storeAllanDeviation(); //AD data for the generated data from noise model
	
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

vector <double> GyroNoiseModel::findRawGyro(string fileName) {
	vector <double> gyroValues;
	double value;
	int i = 0;
	ifstream myFile(fileName);
	if (myFile.is_open())
	{
		while (myFile >> value)
		{
			gyroValues.push_back(value);
		}
		myFile.close();
	}

	return gyroValues;
}


double GyroNoiseModel::noiseDensityValue(double timeStep) { //Calculates noise density between values found using allan variance method
	double noiseDValue;
	noiseDValue = generateRandomNumber(minNoiseDensity, maxNoiseDensity);
	return noiseDValue*timeStep;
}

double GyroNoiseModel::accumNoiseDensityValues(double timeStep) {
	double noiseDensity=0;
	for (int i = 0;i < numberOfNoiseGenerations;i++) {
		noiseDensity = noiseDensity + noiseDensityValue(timeStep);
	}
	return noiseDensity;
}

/*------------------------------------------------------------------------------------------------------*/
/*Final Real W calculation (including all noise)*/

void GyroNoiseModel::findAccumNoise(double timeStep) {
	double totalNoise;
	totalNoise = accumNoiseDensityValues(timeStep) + accumNonLinearityValues(timeStep);
	setAccumNoise(totalNoise);
}

void GyroNoiseModel::findRealW() {
	double wX, wY, wZ;
	/* For integration purpose
	if(counterForNoise%2==0) {
	numberOfNoiseGenerations = 2;
	}
	else {
	numberOfNoiseGenerations = 3;
	}
	findAccumNoise(0.05);
	counterForNoise++;
	*/
	wX = getIdealwX() + getAccumNoise();
	wY = getIdealwY() + getAccumNoise();
	wZ = getIdealwZ() + getAccumNoise();
	setRealwX(wX);
	setRealwY(wY);
	setRealwZ(wZ);
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
	srand((unsigned)time(0));
	s1.setMOIValues(3.4, 2.18, 1.68); //MOST Satellite
	s1.setTorque(0.01, 0.01, 0.01); 
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

		//negative torque after 60s
			if (i == 2999) 
			{
				s1.setTorque(-0.01, -0.01, -0.01); //MOST
			}

		//switching torque off after 90s
			if (i == 4499) 
			{
				s1.setTorque(0.0, 0.0, 0.0);
			}
			noiseValues.storeInFile(accumNoise);
			setIdealwX();
			setIdealwY();
			setIdealwZ();
			s1.findAcc();
			s1.storeValues();
			s1.getNextw();
			totalNoise = noiseDensityValue(0.02) + nonLinearityValue(0.02);
			setAccumNoise(totalNoise);
			findRealW();
 			/* For integration purposes
			totalNoise = accumNoise + noiseDensityValue() + nonLinearityValue();
			setAccumNoise(totalNoise);
			findRealW();
			*/
			}
	}

//To test noise on constant w value
void GyroNoiseModel::testConstW() {
	srand((unsigned)time(0));
	s1.setMOIValues(3.4, 2.18, 1.68); //MOST
	s1.setTorque(0, 0, 0); 
	s1.setStepSize(0.02); 
	s1.setInitialW(-5, 0, 0);
	s1.findConstants();
	double totalNoise;

	for (int i = 0; i < 19550; i++) //For 3 minutes at 20ms stepsize 
	{
		noiseValues.storeInFile(accumNoise);
		setIdealwX();
		setIdealwY();
		setIdealwZ();
		s1.findAcc();
		s1.storeValues();
		s1.findNextW();
		totalNoise = noiseDensityValue(0.02) + nonLinearityValue(0.02);
		setAccumNoise(totalNoise);
		findRealW();
		cout << i << endl;
	}
	generateARW("wX Noise.txt", angleVectorX);
	generateARW("wY Noise.txt", angleVectorY);
	generateARW("wZ Noise.txt", angleVectorZ);
} 

double GyroNoiseModel::generateNextAngle(double thetaN, double time, double nextVelocity ) {
	return (thetaN + (time*nextVelocity));
}

void GyroNoiseModel::generateRawARW() {
	findRawValues("raw valuesX.txt");
	//findRawValues("valuesX.txt");
	//findRawValues("raw valuesY.txt");
	//findRawValues("raw valuesZ.txt");
	double nextTheta;
	double prevTheta = 0.0;
	double nextVelocity;
	double timeStep = 1;
	for (int i = 0; i < randomGyroValues.size(); i++) {
		nextVelocity = randomGyroValues[i];
		nextTheta = generateNextAngle(prevTheta, timeStep, nextVelocity);
		angleVectorX.storeInFile(nextTheta);
		prevTheta = nextTheta;
		cout << i << endl;
	}

}

void GyroNoiseModel::generateARW(string filename, Storage storageFile) {
	vector <double> values;
	double nextTheta;
	double prevTheta = 0.0;
	double nextVelocity;
	double timeStep = 1; 
	values = findRawGyro(filename);
	for (int i = 0; i < values.size(); i++) {
		nextVelocity = values[i];
		nextTheta = generateNextAngle(prevTheta, timeStep, nextVelocity);
		storageFile.storeInFile(nextTheta);
		prevTheta = nextTheta;
		cout << i << endl;
	}
}

void GyroNoiseModel::testReverseNoiseModel(string fileName) {
	srand((unsigned)time(0));
	vector <double> realGyroValues = findRawGyro(fileName);
	double noise;
	double idealW;
	double realW;
	for (int i = 0; i < realGyroValues.size(); i++) {
		noise = nonLinearityValue(1) + noiseDensityValue(1);
		noiseValues.storeInFile(noise);
		idealW = realGyroValues[i] - noise;
		reverseGyroModel.storeInFile(idealW);
		cout << i << endl;
	}
}

void GyroNoiseModel::kalmanFilter(string fileName) {
	vector <double> data;
	data = findRawGyro(fileName);
	double xCurrbar, xPrev, pCurrbar, pPrev, k, r = 0.01, z, xCurr, pCurr;
	xPrev = 0; pPrev = 1;
	for (int i = 0; i < data.size(); i++) {
		z = data[i];
		xCurrbar = xPrev;
		pCurrbar = pPrev;
		k = (pCurrbar) / (pCurrbar + r);
		xCurr = xCurrbar + k*(z - xCurrbar);
		pCurr = (1 - k)*pCurrbar;
		filter.storeInFile(xCurr);
		cout << i << endl;
	}
}
