#include "stdafx.h"
#include "testCase.h"

using namespace std;

//void ExecuteSunPosition(cJulian date, vector<EciSun>& getPos, vector<SunSensorModel>& getGroundTruth, double eciX, double eciY, double eciZ, EciSun e, SunSensorModel s);
void ExecuteSunPosition(const double JD, vector<EciSun>& getPos, vector<SunSensorModel>& getGroundTruth, double eciX, double eciY, double eciZ, EciSun e, SunSensorModel s);
void PrintSunParameters(const vector <EciSun>& sunPos, const vector <SunSensorModel>& getGroundTruth);

int main()
{
	// Testing parameters for sun position
	// Wednesday, A.D. 2017 Feb 15	15:38:45.2
	double JD = 2457800.151912;

	// Initialize vector for Sun Parameters
	vector<EciSun> getPos;
	vector<SunSensorModel> getGroundTruth;
	SunSensorModel s;
	EciSun e;

	for (int i = 1; i <= 365; i++) {
		// We need julian date in order to calculate the sun position
		// So pass the cJulian object to this function
		// Replace JD with the cJulian object from the SGP4
		ExecuteSunPosition(JD, getPos, getGroundTruth, eciX[i], eciY[i], eciZ[i], e, s);
		// ~Sampled every 24 hours
		JD += 1;
	}
	PrintSunParameters(getPos, getGroundTruth);
}


//////////////////////////////////////////////////////////////////////
// Printing for testing purposes 
//////////////////////////////////////////////////////////////////////

void PrintSunParameters(const vector <EciSun>& getPos, const vector <SunSensorModel>& getGroundTruth)
{
	ofstream myfile;
	myfile.precision(12);

	// Save Satellite ECI position, Radius file
	myfile.open("SunPos_ECI.csv", ios::trunc);
	myfile << "No.of Days,Sun_X,Sun_Y,Sun_Z,Body_X,Body_Y,Body_Z,Body_Mag,X_azi,X_ele,Y_azi,Y_ele," << endl;
		for (unsigned int i = 0; i < getPos.size(); i++)
		{
			myfile << i << ',' << getPos[i].SunPosition().x << ',' 
				<< getPos[i].SunPosition().y << ','
				<< getPos[i].SunPosition().z << ',' 
				<< getPos[i].BodyPosition().x << ','
				<< getPos[i].BodyPosition().y << ','
				<< getPos[i].BodyPosition().z << ','
				<< getPos[i].BodyPosition().m << ','
				<< getGroundTruth[i].FaceX().azi << ',' 
				<< getGroundTruth[i].FaceX().ele << ','
				<< getGroundTruth[i].FaceY().azi << ','
				<< getGroundTruth[i].FaceY().ele << ','
				<< getGroundTruth[i].nFaceX().azi << ','
				<< getGroundTruth[i].nFaceX().ele << ','
				<< getGroundTruth[i].nFaceY().azi << ','
				<< getGroundTruth[i].nFaceY().ele << endl;
		}
	myfile.close();
}

//////////////////////////////////////////////////////////////////////
// Call this method when you want the position of the sun
//////////////////////////////////////////////////////////////////////
void ExecuteSunPosition(const double JD, vector<EciSun>& getPos,
	vector<SunSensorModel>& getGroundTruth, double eciX, double eciY, 
	double eciZ, EciSun e, SunSensorModel s)
{
	// Calculates sun position and body position of satellite
	e.setJulianDate(JD);
	e.calculateSunVec();
	e.computeBodyFrame(eciX, eciY, eciZ);
	getPos.push_back(e);
	
	// Set the plane of the sensor coordinates based on body frame.
	s.computeSensorVector(e);
	getGroundTruth.push_back(s);
}

void Execute_Sgp4(const cSatellite& sat, int mpe,
	vector<cEci>& vecPos, vector<cGeo>& geoPos, vector<cEcef>& ecefPos) {

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