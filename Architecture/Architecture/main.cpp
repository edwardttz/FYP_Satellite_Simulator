#include "stdafx.h"

mutex groundTruthLock;
condition_variable groundTruthConVar;
bool groundTruthDone = false;
bool noiseModelsDone = false;
bool transportDone = true;

SpacecraftDynamics s1;
GyroNoiseModel gyroModel;

Storage satX_values;
Storage satY_values;
Storage satZ_values;

// Start of Initialization
vector<cEci> vecPos;
vector<cGeo> geoPos;
vector<cEcef> ecefPos;
vector<double> magFieldValues;
double mpe = 0;

//forward declarations
void calculateGroundTruth(int);
void calculateNoiseModels(void);
void transportData(void);
void Execute_Sgp4(const cSatellite& sat, double timeLength,
	vector<cEci>& vecPos, vector<cGeo>& geoPos, vector<cEcef>& ecefPos);
void calculateMagField(const double, const double, const double, const double, vector<double>&);
void calcMagFieldMain(const cGeo, const cJulian, vector<double>&);
double calculateDecimalYear(double);
void calculateMagField(const double, const double, const double, const double, vector<double>&);
void runSunVector(cSatellite);
void initSatelliteStorage(void);
void initGroundTruth(void);
void storeSatelliteValues(double, double, double);


int main(void)
{
	clock_t tStart = clock();
	
	//initialize storage of satellite position values
	initSatelliteStorage();

	//initialize ground truth values
	initGroundTruth();

	for (int count = 0; count < 111601; count++) {
		thread groundTruth(calculateGroundTruth, count);
		//thread noiseModels(calculateNoiseModels);

		//join back groundtruth thread once it has completed calculations
		if (groundTruth.joinable())
		{
			groundTruth.join();
		}

		/*
		if (noiseModels.joinable())
		{
			noiseModels.join();
		}
		*/

		mpe += 0.05/60;
	}
	printf("Time taken = %.10fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

	system("PAUSE");
}

void calculateGroundTruth(int counter)
{
	unique_lock<mutex> groundLock(groundTruthLock);
	groundTruthConVar.wait(groundLock, [] {return transportDone;});

	//switch off torque at 30s
	if (counter == 599)
	{
		s1.setTorque(0.0, 0.0, 0.0);
	}

	//switch on negative torque at 1min
	if (counter == 1199)
	{
		s1.setTorque(-0.01, -0.01, -0.01);
	}

	//switch off torque at 1min 30s
	if (counter == 1799)
	{
		s1.setTorque(0.0, 0.0, 0.0);
	}

	//Find acceleration, euler angles and next vector and store all values in text files
	//Find next velocity and quaternion values after that
	s1.findAcc();
	s1.findThetaValues();
	s1.findNextVector();
	s1.storeValues();
	s1.getNextw();
	s1.findNextQuaternion();
	
	// Test SGP4 TLE data
	string str1 = "SGP4 Test";
	string str2 = "1 25544U 98067A   16291.11854479  .00010689  00000-0  16758-3 0  9992";
	string str3 = "2 25544  51.6446 169.8664 0007102  80.6091  76.5051 15.54264543 23954";
	string tle[3] = { str1, str2, str3 };

	// Create a TLE object using the data above
	cTle tleSGP4(tle[0], tle[1], tle[2]);

	// Create a satellite object from the TLE object
	cSatellite satSGP4(tleSGP4);

	// Locate position and velocity information of the satellite
	// Time in minutes
	// mpe = "minutes past epoch"
	Execute_Sgp4(satSGP4, mpe, vecPos, geoPos, ecefPos);
	
	//store values in vecPos for writing into text file
	storeSatelliteValues(vecPos[counter].Position().m_x, vecPos[counter].Position().m_y, vecPos[counter].Position().m_z);

	//unlocking of mutex
	groundTruthDone = true;
	groundTruthConVar.notify_one();
}

void calculateNoiseModels()
{
	unique_lock<mutex> groundLock(groundTruthLock);
	groundTruthConVar.wait(groundLock, [] {return groundTruthDone; });
	
	/*
	//magnetometer

	//vector<double> magFieldValues;
	//calcMagFieldMain(geoPos[geoPos.size() - 1], satSGP4.Orbit().Epoch(), magFieldValues);

	//gyro
	gyroModel.setPollingTime(0.05); //Polling time by default is 50ms
	gyroModel.setSamplePeriod(0.02); //sample period 1 for raw gyro values
	gyroModel.setFSValue(250); //Full scale of gyro by default = 250
	gyroModel.setMaxNoiseDensity(0.37);
	gyroModel.setMinNoiseDensity(-0.37);
	gyroModel.findRealW();
	//gyroModel.findAllanDeviation();

	cout << "realwX = " << gyroModel.getRealwX() << endl;
	cout << "realwY = " << gyroModel.getRealwY() << endl;
	cout << "realwZ = " << gyroModel.getRealwZ() << endl;

	//sun
	// Testing parameters for sun position
	// Wednesday, A.D. 2017 Feb 15	15:38:45.2
	double JD = 2457800.151912;

	// Initialize vector for Sun Parameters
	vector<EciSun> getPos;
	vector<SunSensorModel> getGroundTruth;
	SunSensorModel s;
	EciSun e;

	for (int i = 1; i <= 365; i++)
	{
		// We need julian date in order to calculate the sun position
		// So pass the cJulian object to this function
		// Replace JD with the cJulian object from the SGP4
		ExecuteSunPosition(JD, getPos, getGroundTruth, eciX[i], eciY[i], eciZ[i], e, s);
		// ~Sampled every 24 hours
		JD += 1;
	}
	*/
}

void transportData()
{
	printf("tpt\n");
}

void Execute_Sgp4(const cSatellite& sat, double mpe,
	vector<cEci>& vecPos, vector<cGeo>& geoPos, vector<cEcef>& ecefPos)
{
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

void initGroundTruth()
{
	//s1.setMOIValues(3.03, 4.85, 2.98); //KR 1
	//s1.setMOIValues(40.45, 42.09, 41.36); //UoSat12
	s1.setMOIValues(3.4, 2.18, 1.68); //MOST

	//s1.setTorque(0.0107, 0.0107, 0.0107); //KR 1
	//s1.setTorque(0.05, 0.05, 0.05); //UoSat12
	s1.setTorque(0.01, 0.01, 0.01); //MOST

	s1.setStepSize(0.05); //50ms stepsize

	s1.setQuaternionInitialValues(0, 0, 0, 1);

	s1.setQuaternionInverseInitialValues(0, 0, 0, -1);

	s1.setVectorInitialValues(0, 0, 0, 1);
}

void initSatelliteStorage()
{
	satX_values.setFileName("satX.txt");
	satY_values.setFileName("satY.txt");
	satZ_values.setFileName("satZ.txt");
	satX_values.clearFile();
	satY_values.clearFile();
	satZ_values.clearFile();
}

void storeSatelliteValues(double satX, double satY, double satZ)
{
	satX_values.storeInFile(satX);
	satY_values.storeInFile(satY);
	satZ_values.storeInFile(satZ);
}