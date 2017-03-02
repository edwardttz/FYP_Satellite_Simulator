#include "stdafx.h"

mutex groundTruthLock;
condition_variable groundTruthConVar;
bool groundTruthDone = false;
bool noiseModelsDone = false;
bool transportDone = true;

SpacecraftDynamics s1;
GyroNoiseModel gyroModel;
//sgp4Data tleData;

//forward declarations
void calculateGroundTruth(void);
void calculateNoiseModels(void);
void transportData(void);
void Execute_Sgp4(const cSatellite& sat, int timeLength,
	vector<cEci>& vecPos, vector<cGeo>& geoPos, vector<cEcef>& ecefPos);
void calculateMagField(const double, const double, const double, const double, vector<double>&);
void calcMagFieldMain(const cGeo, const cJulian, vector<double>&);
double calculateDecimalYear(double);
void calculateMagField(const double, const double, const double, const double, vector<double>&);
//void ExecuteSunPosition(cJulian date, vector<EciSun>& getPos, vector<SunSensorModel>& getGroundTruth, double eciX, double eciY, double eciZ, EciSun e, SunSensorModel s);
void runSunVector(cSatellite);

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

	// Create a TLE object using the data above
	cTle tleSGP4(tle[0], tle[1], tle[2]);

	// Create a satellite object from the TLE object
	cSatellite satSGP4(tleSGP4);

	// Start of Initialization
	vector<cEci> vecPos;
	vector<cGeo> geoPos;
	vector<cEcef> ecefPos;
	vector<double> magFieldValues;

	// Locate position and velocity information of the satellite
	// Time in minutes
	// mpe = "minutes past epoch"
	printf("start loop\n");
	for (int mpe = 0; mpe <= 60 * 24; mpe += 1)
	{
		cout << "mpe = " << mpe << endl;
		//Execute_Sgp4(tleData.getSatSGP4(), mpe, tleData.getVecPos(), tleData.getGeoPos(), tleData.getEcefPos());
		Execute_Sgp4(satSGP4, mpe, vecPos, geoPos, ecefPos);

		//magnetometer
		//calcMagFieldMain(geoPos[geoPos.size() - 1], satSGP4.Orbit().Epoch(), magFieldValues);

		cout << "test in loop = " << vecPos[mpe].Position().m_y << endl;
	}

	//unlocking of mutex
	groundTruthDone = true;
	groundTruthConVar.notify_one();
}

void calculateNoiseModels()
{
	unique_lock<mutex> groundLock(groundTruthLock);
	groundTruthConVar.wait(groundLock, [] {return groundTruthDone; });
	
	//magnetometer

	//vector<double> magFieldValues;
	//calcMagFieldMain(geoPos[geoPos.size() - 1], satSGP4.Orbit().Epoch(), magFieldValues);

	//gyro
	//for (int i = 0; i < 3600; i++)
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
	/*
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

	printf("noise\n");
}

void transportData()
{
	printf("tpt\n");
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

void calcMagFieldMain(const cGeo geoPos, const cJulian date, vector<double>& magFieldValues) {
	double lat = geoPos.LatitudeDeg();
	double lon = geoPos.LongitudeDeg();
	double h = geoPos.AltitudeKm();
	double t = date.Date();
	calculateMagField(lat, lon, h, t, magFieldValues);
}

void calculateMagField(const double lat, const double lon, const double h, const double t, vector<double>& magFieldValues) {

	MagneticModel mag("wmm2015", "../GeographicLib/magnetic");

	double Bx, By, Bz, H, F, D, I, Bx_temp, By_temp, Bz_temp;
	double time = calculateDecimalYear(t);
	mag(time, lat, lon, h * 1000, Bx, By, Bz);
	MagneticModel::FieldComponents(Bx, By, Bz, H, F, D, I);
	Bz = -Bz;

	// Conversion to mG (100nT = 1mG)
	Bx_temp = Bx / 100.0;
	By_temp = By / 100.0;
	Bz_temp = Bz / 100.0;
	H /= 100.0;
	F /= 100.0;

	// Include noise due to Cross-Axis Sensitivity
	Bx = Bx_temp + ((By_temp + Bz_temp) * 0.02);
	By = By_temp + ((Bx_temp + Bz_temp) * 0.02);
	Bz = Bz_temp + ((Bx_temp + By_temp) * 0.02);
	H = sqrt(pow(Bx, 2) + pow(By, 2));
	F = sqrt(pow(Bx, 2) + pow(By, 2) + pow(Bz, 2));

	// Adding Noise floor
	Bx += genRandNoiseFloor(4);
	By += genRandNoiseFloor(4);
	Bz += genRandNoiseFloor(4);
	H += genRandNoiseFloor(4);
	F += genRandNoiseFloor(4);

	// Linearity 0.1% Full-Scale
	F = F * (100.1 / 100);

	// Analog values
	magFieldValues.push_back(Bx);
	magFieldValues.push_back(By);
	magFieldValues.push_back(Bz);
	magFieldValues.push_back(H);
	magFieldValues.push_back(F);
	magFieldValues.push_back(D);
	magFieldValues.push_back(I);

	// Digital (Quantized) values
	magFieldValues.push_back(quantizeValue(Bx));
	magFieldValues.push_back(quantizeValue(By));
	magFieldValues.push_back(quantizeValue(Bz));
	magFieldValues.push_back(quantizeValue(H));
	magFieldValues.push_back(quantizeValue(F));
}

double calculateDecimalYear(double julianDate) {
	int daysAYear = 365, leapDay, nYear;
	double refDate = 2451544.5; //Date: 2000/1/1
	double decimalValue, decYear;
	decimalValue = julianDate - (int)julianDate;
	if (decimalValue < 0.5) {
		julianDate = (int)julianDate - 1 + 0.5;
	}
	else {
		julianDate = (int)julianDate + 0.5;
	}
	julianDate -= refDate;
	nYear = julianDate / daysAYear;
	leapDay = nYear / 4;
	julianDate -= leapDay;
	julianDate -= nYear * daysAYear;
	decYear = nYear + 2000 + julianDate / daysAYear;

	return decYear;
}

//////////////////////////////////////////////////////////////////////
// Call this method when you want the position of the sun
//////////////////////////////////////////////////////////////////////
/*
void ExecuteSunPosition(cJulian date, vector<EciSun>& getPos,
	vector<SunSensorModel>& getGroundTruth, double eciX, double eciY,
	double eciZ, EciSun e, SunSensorModel s)
{
	// Calculates sun position and body position of satellite
	e.setJulianDate(date.ToDate());
	e.calculateSunVec();
	e.computeBodyFrame(eciX, eciY, eciZ);
	getPos.push_back(e);

	// Set the plane of the sensor coordinates based on body frame.
	s.setPlane(0, 1, 0);
	s.computeSensorVector(e);
	getGroundTruth.push_back(s);
}*/

/*
void runSunVector(cSatellite JD)
{
	// Testing parameters for sun position
	// Wednesday, A.D. 2017 Feb 15	15:38:45.2
	//double JD = 2457800.151912;

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
		ExecuteSunPosition(JD.Orbit().Epoch(), getPos, getGroundTruth, JD.PositionEci.eciX[i], JD.PositionEci.eciY[i], JD.PositionEci.eciZ[i], e, s);
		// ~Sampled every 24 hours
		//JD += 1;
	}
}*/
