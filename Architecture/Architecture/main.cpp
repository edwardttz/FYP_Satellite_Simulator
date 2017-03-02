//libraries
#include <iostream>
#include <cmath>
#include <string>
#include <thread>
#include <mutex>
#include <windows.h>
#include <cstdlib>
#include <condition_variable>
#include <time.h>

//headers for ground truth
#include "SpacecraftDynamics.h"
#include "Storage.h"
#include "coreLib.h"
#include "orbitLib.h"

//headers for noise models

using namespace std;

mutex groundTruthLock;
condition_variable groundTruthConVar;
bool groundTruthDone = false;
bool noiseModelsDone = false;
bool transportDone = true;


double groundTruthBuffer[50];
double noiseModelsBuffer[50];
double dataToSendBuffer[50];

//forward declarations
void calculateGroundTruth(void);
void calculateNoiseModels(void);
void tango(void);
void Execute_Sgp4(const cSatellite& sat, int timeLength,
	vector<cEci>& vecPos, vector<cGeo>& geoPos, vector<cEcef>& ecefPos);
void calculateMagField(const double, const double, const double, const double, vector<double>&);
void calcMagFieldMain(const cGeo, const cJulian, vector<double>&);

int main(void)
{
	clock_t tStart = clock();
	
	thread groundTruth(calculateGroundTruth);
	thread noiseModels(calculateNoiseModels);

	if (groundTruth.joinable())
	{
		groundTruth.join();
	}
	
	if (noiseModels.joinable())
	{
		noiseModels.join();
	}
	
	printf("Time taken = %.10fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	system("PAUSE");
}

void calculateGroundTruth()
{
	unique_lock<mutex> groundLock(groundTruthLock);
	groundTruthConVar.wait(groundLock, [] {return transportDone;});
	printf("ground truth\n");


	SpacecraftDynamics s1;

	//s1.setMOIValues(3.03, 4.85, 2.98); //KR 1
	//s1.setMOIValues(40.45, 42.09, 41.36); //UoSat12
	s1.setMOIValues(3.4, 2.18, 1.68); //MOST

	//s1.setTorque(0.0107, 0.0107, 0.0107); //KR 1
	//s1.setTorque(0.05, 0.05, 0.05); //UoSat12
	s1.setTorque(0.01, 0.01, 0.01); //MOST

	//s1.setStepSize(0.01); //10ms stepsize
	s1.setStepSize(0.05); //50ms stepsize

	s1.setQuaternionInitialValues(0, 0, 0, 1);

	//change i < some number to fit iterations and stepsize
	//for multiple iterations
	for (int i = 0; i < 3600; i++) //18000 for 10ms, 3600 for 50ms
	{
		
		//switching torque off after 30s
		if (i == 599) //2999 for 10ms, 599 for 50ms
		{
			s1.setTorque(0.0, 0.0, 0.0);
		}

		//negative torque after 30s
		if (i == 1199) //5999 for 10ms, 1199 for 50ms
		{
			//s1.setTorque(-0.0107, -0.0107, -0.0107); //KR 1
			//s1.setTorque(-0.05, -0.05, -0.05); //UoSat12
			s1.setTorque(-0.01, -0.01, -0.01); //MOST
		}

		//switching torque off after 30s
		if (i == 1799) //8999 for 10ms, 1799 for 50ms
		{
			s1.setTorque(0.0, 0.0, 0.0);
		}
		

		//Find acceleration, next velocity and store all values in text file

		s1.findAcc();
		s1.findThetaValues();
		s1.storeValues();
		s1.getNextw();
		s1.findNextQuaternion();
		s1.findNextVector();
	}

	cout << "wX = " << s1.getVelocityX() << endl;
	cout << "wY = " << s1.getVelocityY() << endl;
	cout << "wZ = " << s1.getVelocityZ() << endl;
	cout << "aX = " << s1.getAccX() << endl;
	cout << "aY = " << s1.getAccY() << endl;
	cout << "aZ = " << s1.getAccZ() << endl;
	cout << "q0 = " << s1.getQuaternion0() << endl;
	cout << "qX = " << s1.getQuaternionX() << endl;
	cout << "qY = " << s1.getQuaternionY() << endl;
	cout << "qZ = " << s1.getQuaternionZ() << endl;

	printf("start TLE\n");
	// Test SGP4 TLE data
	string str1 = "SGP4 Test";
	string str2 = "1 25544U 98067A   16291.11854479  .00010689  00000-0  16758-3 0  9992";
	string str3 = "2 25544  51.6446 169.8664 0007102  80.6091  76.5051 15.54264543 23954";
	string tle[3] = { str1, str2, str3 };

	// Start of Initialization
	vector<cEci> vecPos;
	vector<cGeo> geoPos;
	vector<cEcef> ecefPos;

	// Create a TLE object using the data above
	cTle tleSGP4(tle[0], tle[1], tle[2]);

	// Create a satellite object from the TLE object
	cSatellite satSGP4(tleSGP4);
	// End of Initialization

	// Locate position and velocity information of the satellite
	// Time in minutes
	// mpe = "minutes past epoch"
	printf("start loop\n");
	for (int mpe = 0; mpe <= 3.0; mpe += 1.0)
	{
		cout << "mpe = " << mpe << endl;
		Execute_Sgp4(satSGP4, mpe, vecPos, geoPos, ecefPos);
	}

	groundTruthDone = true;
	groundTruthConVar.notify_one();
}

void Execute_Sgp4(const cSatellite& sat, int mpe,
	vector<cEci>& vecPos, vector<cGeo>& geoPos, vector<cEcef>& ecefPos) {

	cout << "mpe in exe = " << mpe << endl;
	// Calculate the position and velocity of the satellite for various times.

	// Get the position of the satellite at time "mpe"
	cEciTime eci = sat.PositionEci(mpe);
	cEcefTime ecef = sat.PositionEcef(mpe);
	cGeoTime geo = sat.PositionEcef(mpe);

	// Push the coordinates object onto the end of the vector.
	vecPos.push_back(eci);
	geoPos.push_back(geo);
	ecefPos.push_back(ecef);
}

void calculateNoiseModels()
{
	unique_lock<mutex> groundLock(groundTruthLock);
	groundTruthConVar.wait(groundLock, [] {return groundTruthDone; });
	
	//magnetometer

	//gyro

	//sun

	printf("noise\n");
}

void tango()
{
	printf("tpt\n");
}

