#include "stdafx.h"
#include "testCase.h"

using namespace std;

void ExecuteSunPosition(cJulian date, vector<EciSun>& getPos, vector<SunSensorModel>& getGroundTruth, double eciX, double eciY, double eciZ, EciSun e, SunSensorModel s);
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
	myfile << "No.of Days,Sun_X,Sun_Y,Sun_Z,Azimuth,Elevation,Body_X,Body_Y,Body_Z,Body_Mag,Azimuth,Elevation" << endl;
		for (unsigned int i = 0; i < getPos.size(); i++)
		{
			myfile << i << ',' << getPos[i].SunPosition().x << ',' 
				<< getPos[i].SunPosition().y << ','
				<< getPos[i].SunPosition().z << ',' 
				<< getPos[i].SunPosition().azi << ','
				<< getPos[i].SunPosition().ele << ','
				<< getPos[i].BodyPosition().x << ','
				<< getPos[i].BodyPosition().y << ','
				<< getPos[i].BodyPosition().z << ','
				<< getPos[i].BodyPosition().m << ','
				<< getGroundTruth[i].Plane().azi << ','
				<< getGroundTruth[i].Plane().ele << endl;
		}
	myfile.close();
}

//////////////////////////////////////////////////////////////////////
// Call this method when you want the position of the sun
//////////////////////////////////////////////////////////////////////
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
}

