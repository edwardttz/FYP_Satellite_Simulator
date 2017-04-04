#ifndef GYROSCOPE_NOISE_MODEL_H
#define GYROSCOPE_NOISE_MODEL_H

#include <string>
#include "SpacecraftDynamics.h"
#include "Storage.h"
using namespace std;

class GyroNoiseModel
{
private:
	static double pollingTime; //polling time of main program in seconds.
	static int numberOfNoiseGenerations;
	static int counterForNoise;
	static vector <double> realWValues;
	static double wXreal;
	static double wYreal;
	static double wZreal;
	static double wXideal;
	static double wYideal;
	static double wZideal;
	static int fsValue;
	static double samplePeriod;
	static double timeGenerated;
	static long int noOfSamples;
	static double maxNonLinearity;
	static double minNonLinearity;
	static double maxNoiseDensity;
	static double minNoiseDensity;
	static vector <double> allanDeviationValues;
	static vector <double> randomGyroValues;
	static vector <double> thetaValues;
	static double accumNoise;
	SpacecraftDynamics s1;
	Storage wXRealValues;
	Storage wYRealValues;
	Storage wZRealValues;
	Storage allanDeviation;
	Storage noiseValues;
	Storage randomValues;
	Storage angleVectorX;
	Storage angleVectorY;
	Storage angleVectorZ;
	Storage reverseGyroModel;
	Storage filter;

public:
	/*Setting enviroment*/
	GyroNoiseModel();
	void setPollingTime(double);
	double getPollingTime();
	void getUserInput();
	void setFSValue(int);
	int getFSValue();
	void setSamplePeriod(double);
	double getSamplePeriod();
	void setDataGenTime(double);
	double getDataGenTime();
	/*General equations of getting and setting variables*/
	vector <double> getRealWValues();
	double getRealwX();
	double getRealwY();
	double getRealwZ();
	void setRealwX(double);
	void setRealwY(double);
	void setRealwZ(double);
	double getIdealwX();
	double getIdealwY();
	double getIdealwZ();
	void setIdealwX();
	void setIdealwY();
	void setIdealwZ();
	double getAccumNoise();
	void setAccumNoise(double);
	double generateRandomNumber(double, double);
	/*Non Linearity*/
	void setMaxNonLinearity();
	void setMinNonLinearity();
	double nonLinearityValue(double);
	double accumNonLinearityValues(double);
	/*Noise density*/
	void setMaxNoiseDensity(double);
	void setMinNoiseDensity(double);
	void storeAllanDeviation();
	void findAllanDeviation();
	void generateRawValues(double, double);
	void calculateThetaValues();
	void findNoOfSamples();
	void calculateAllanDeviation(int);
	void findRawValues(string);
	vector <double> findRawGyro(string);
	double noiseDensityValue(double);
	double accumNoiseDensityValues(double);
	void storeRandomValues();
	/*Real W with all noise calculation*/
	void findAccumNoise(double);
	void findRealW();
	void storeRealwValues();
	void testGyroModel();
	void testConstW();
	double generateNextAngle(double, double, double);
	void generateRawARW();
	void generateARW(string, Storage);
	void testReverseNoiseModel(string);
	void kalmanFilter(string);
};



#endif