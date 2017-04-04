#include "stdafx.h"

using namespace std;

void ExecuteSunPosition(double date, vector<EciSun>& getPos, vector<SunSensorModel>& getGroundTruth, double eciX, double eciY, double eciZ, EciSun e, SunSensorModel s);
void PrintSunParameters(const vector <EciSun>& sunPos, const vector <SunSensorModel>& getGroundTruth);
void Execute_Sgp4(const cSatellite& sat, int timeLength, vector<cEci>& vecPos, vector<cGeo>& geoPos, vector<cEcef>& ecefPos);

int main()
{
	// Test SGP4 TLE data
	string str1 = "SGP4 Test";
	string str2 = "1 25544U 98067A   17080.91291815 +.00000998 +00000-0 +22293-4 0  9990";
	string str3 = "2 25544 051.6418 113.3379 0007038 322.7961 098.1992 15.54230764048188";
	string tle[3] = { str1, str2, str3 };

	// Initialization for Satellite
	vector<cEci> vecPos;
	vector<cGeo> geoPos;
	vector<cEcef> ecefPos;

	// Initialize vector for Sun Parameters
	vector<EciSun> getPos;
	vector<SunSensorModel> getGroundTruth;
	SunSensorModel s;
	EciSun e;

	// Create a TLE object using the data above
	cTle tleSGP4(tle[0], tle[1], tle[2]);
	
	// Create a satellite object from the TLE object
	cSatellite satSGP4(tleSGP4);

	// Testing parameters for sun position
	// Initial J.D: Tuesday, A.D. 2017 Mar 21 UT 21:54:36.1
	cJulian date = satSGP4.Orbit().Epoch();
	double setDate = date.Date();

	// Locate position and velocity information of the satellite
	// Time in minutes, mpe = "minutes past epoch"
	for (int mpe = 0; mpe <= (60 * 24 * 365 * 2); mpe += 60*24) {
		Execute_Sgp4(satSGP4, mpe, vecPos, geoPos, ecefPos);
	}

	for (int i = 0; i < vecPos.size(); i++) {
		ExecuteSunPosition(setDate, getPos, getGroundTruth, vecPos[i].Position().m_x, vecPos[i].Position().m_y, vecPos[i].Position().m_z, e, s);
		
		// ~Sampled every 24 hours, for 2 years
		setDate += 1;
	}
	PrintSunParameters(getPos, getGroundTruth);

	/* Quaternions not in used
	q.setMOIValues(3.4, 2.18, 1.68); //MOST									
	q.setTorque(0.01, 0.01, 0.01); //MOST
	q.setStepSize(0.05); //50ms stepsize
	q.setQuaternionInitialValues(0, 0, 0, 1);
	q.setInitialW(0, 0, 1);
	q.setVectorInitialValues(0, 0, 0, 1);
	q.findAcc();
	q.findThetaValues();
	q.getNextw();
	q.findNextQuaternion();
	q.findNextVector();
	*/
}

//////////////////////////////////////////////////////////////////////
// Printing for testing purposes 
//////////////////////////////////////////////////////////////////////

void PrintSunParameters(const vector <EciSun>& getPos, const vector <SunSensorModel>& getGroundTruth)
{
	ofstream myfile;
	myfile.precision(12);

	// Save the relevant data into the CSV file
	myfile.open("SunPos_ECI.csv", ios::trunc);
	myfile << "No.of Days,Sun_PosX,Sun_PosY,Sun_PosZ,SunVecX,SunVecY,SunVecZ,SunVec_Mag,X_azi,Y_azi,Z_azi,nX_azi,nY_azi,nZ_azi,X_ele" << endl;
		for (unsigned int i = 0; i < getPos.size(); i++)
		{
			myfile << i << ',' << getPos[i].SunPosition().x << ',' 
				<< getPos[i].SunPosition().y << ','
				<< getPos[i].SunPosition().z << ',' 
				<< getPos[i].SunVec().x << ','
				<< getPos[i].SunVec().y << ','
				<< getPos[i].SunVec().z << ','
				<< getPos[i].SunVec().m << ','
				<< getGroundTruth[i].FaceX().azi << ',' 
				<< getGroundTruth[i].FaceY().azi << ','
				<< getGroundTruth[i].FaceZ().azi << ','
				<< getGroundTruth[i].nFaceX().azi << ','
				<< getGroundTruth[i].nFaceY().azi << ','
				<< getGroundTruth[i].nFaceZ().azi << ','
				<< getGroundTruth[i].FaceX().ele << endl;
		}
	myfile.close();
}

//////////////////////////////////////////////////////////////////////
// Call this method when you want the position of the sun
//////////////////////////////////////////////////////////////////////

void ExecuteSunPosition(double date, vector<EciSun>& getPos,
	vector<SunSensorModel>& getGroundTruth, double eciX, double eciY, 
	double eciZ, EciSun e, SunSensorModel s)
{
	// Calculates sun position and body position of satellite
	e.setJulianDate(date);
	e.calculateSunVec();
	e.computeBodySunVec(eciX, eciY, eciZ);
	getPos.push_back(e);
	
	// Set the plane of the sensor coordinates
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