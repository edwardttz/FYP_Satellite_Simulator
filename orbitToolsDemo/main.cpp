//
// main.cpp 
//
// This sample code demonstrates how to use the OrbitTools C++ classes
// to determine satellite position and look angles.
//
// Copyright (c) 2003-2014 Michael F. Henry
//
// 06/2014
//
#include "stdafx.h"

#include <stdio.h>

#include <time.h>

// "coreLib.h" includes basic types from the core library,
// such as cSite, cJulian, etc. The header file also contains a
// "using namespace" statement for Zeptomoby::OrbitTools.
#include "coreLib.h"

// "orbitLib.h" includes basic types from the orbit library,
// including cOrbit.
#include "orbitLib.h"

// Forward declaration of helper function; see below
void Execute_Sgp4(const cSatellite& sat, int timeLength,
	vector<cEci>& vecPos, vector<cGeo>& geoPos, vector<cEcef>& ecefPos);

void PrintPosVel(string tle[], cJulian date,
	const vector<cEci>& vecPos, const vector<cGeo>& geoPos, const vector<cEcef>& ecefPos);

//////////////////////////////////////////////////////////////////////////////
int main(int /* argc */, char* /* argv[] */)
{
	clock_t tStart = clock();
	// Input
   // Test SGP4 TLE data
   string str1 = "SGP4 Test";
   string str2 = "1 25544U 98067A   16291.11854479  .00010689  00000-0  16758-3 0  9992";
   string str3 = "2 25544  51.6446 169.8664 0007102  80.6091  76.5051 15.54264543 23954";
   string tle[3] = {str1, str2, str3};

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
   for (double mpe = 0; mpe <= 1.0; mpe += 1.0/60.0) {
	   Execute_Sgp4(satSGP4, mpe, vecPos, geoPos, ecefPos);
   }

   // Print the position and velocity information of the satellite
   PrintPosVel(tle, satSGP4.Orbit().Epoch(), vecPos, geoPos, ecefPos);

   printf("Time taken: %.10fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);

   system("PAUSE");
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
/////////////////////////////////////////////////////////////////////////////
// Helper function to output position and velocity information
void PrintPosVel(string tle[], cJulian date,
		const vector<cEci>& vecPos, const vector<cGeo>& geoPos, const vector<cEcef>& ecefPos)
{
   long double radius = 0.0;
   ofstream myfile;
   myfile.precision(12);
  
   // Save Satellite ECI position, Radius file
   myfile.open("Satellite Data/Satellite_Pos_Rad_ECI.csv", ios::trunc);
   myfile << "T since,X,Y,Z,Radius,,," << tle[0] << endl;
   for (unsigned int i = 0; i < vecPos.size(); i++)
   {
	   radius = sqrt(powl(vecPos[i].Position().m_x, 2) + powl(vecPos[i].Position().m_y, 2)
		   + powl(vecPos[i].Position().m_z, 2));
	   if (i == 0) 
	   {
		   myfile << i << ',' << vecPos[i].Position().m_x << ',' << vecPos[i].Position().m_y << 
			   ',' << vecPos[i].Position().m_z << ',' << radius << ",,," 
			   << tle[1] << endl;
			   
	   }
	   else if (i == 1) 
	   {
		   myfile << i << ',' << vecPos[i].Position().m_x << ',' << vecPos[i].Position().m_y <<
			   ',' << vecPos[i].Position().m_z << ',' << radius << ",,," 
			   << tle[2] << endl;
	   }
	   else if (i == 2)
	   {
		   myfile << i << ',' << vecPos[i].Position().m_x << ',' << vecPos[i].Position().m_y <<
			   ',' << vecPos[i].Position().m_z << ',' << radius << ",,,"
			   << "Singapore UTC+8:" << ",,"
			   << date.ToTime().tm_mday << '/'
			   << date.ToTime().tm_mon << '/'
			   << date.ToTime().tm_year << ' '
			   << date.ToTime().tm_hour << ':'
			   << date.ToTime().tm_min << ':'
			   << date.ToTime().tm_sec << endl;
	   }
	   else 
	   {
		   myfile << i << ',' << vecPos[i].Position().m_x << ',' << vecPos[i].Position().m_y <<
			   ',' << vecPos[i].Position().m_z << ',' << radius << endl;
	   }   
   }
   myfile.close();
   
   // Save Satellite ECI Velocity file
   myfile.open("Satellite Data/Satellite_Velocity_ECI.csv", ios::trunc);
   myfile << "T since,Xdot,Ydot,Zdot,,," << tle[0] << endl;
   for (unsigned int i = 0; i < vecPos.size(); i++)
   {
	   if (i == 0)
	   {
		   myfile << i << ',' << vecPos[i].Velocity().m_x << ',' << vecPos[i].Velocity().m_y <<
			   ',' << vecPos[i].Velocity().m_z << ",,," 
			   << tle[1] << endl;

	   }
	   else if (i == 1)
	   {
		   myfile << i << ',' << vecPos[i].Velocity().m_x << ',' << vecPos[i].Velocity().m_y <<
			   ',' << vecPos[i].Velocity().m_z << ",,," 
			   << tle[2] << endl;
	   }
	   else if (i == 2)
	   {
		   myfile << i << ',' << vecPos[i].Position().m_x << ',' << vecPos[i].Position().m_y <<
			   ',' << vecPos[i].Position().m_z << ",,,"
			   << "Singapore UTC+8:" << ",,"
			   << date.ToTime().tm_mday << '/'
			   << date.ToTime().tm_mon << '/'
			   << date.ToTime().tm_year << ' '
			   << date.ToTime().tm_hour << ':'
			   << date.ToTime().tm_min << ':'
			   << date.ToTime().tm_sec << endl;
	   }
	   else
	   {
		   myfile << i << ',' << vecPos[i].Velocity().m_x << ',' << vecPos[i].Velocity().m_y <<
			   ',' << vecPos[i].Velocity().m_z  << endl;
	   }
   }
   myfile.close();

   // Save Satellite Latitude, Longtitude, Altitude to file
   myfile.open("Satellite Data/Satellite_Lat_Long_Alti_ECEF.csv", ios::trunc);
   myfile << "T since,Latitude,Longitude,Altitude,,," << tle[0] << endl;
   for (unsigned int i = 0; i < geoPos.size(); i++)
   {
	   if (i == 0)
	   {
		   myfile << i << ',' << geoPos[i].LatitudeDeg() << ',' << geoPos[i].LongitudeDeg() <<
			   ',' << geoPos[i].AltitudeKm() << ",,,"
			   << tle[1] << endl;

	   }
	   else if (i == 1)
	   {
		   myfile << i << ',' << geoPos[i].LatitudeDeg() << ',' << geoPos[i].LongitudeDeg() <<
			   ',' << geoPos[i].AltitudeKm() << ",,,"
			   << tle[2] << endl;
	   }
	   else if (i == 2)
	   {
		   myfile << i << ',' << geoPos[i].LatitudeDeg() << ',' << geoPos[i].LongitudeDeg() <<
			   ',' << geoPos[i].AltitudeKm() << ",,,"
			   << "Singapore UTC+8:" << ",,"
			   << date.ToTime().tm_mday << '/'
			   << date.ToTime().tm_mon << '/'
			   << date.ToTime().tm_year << ' '
			   << date.ToTime().tm_hour << ':'
			   << date.ToTime().tm_min << ':'
			   << date.ToTime().tm_sec << endl;
	   }
	   else
	   {
		   myfile << i << ',' << geoPos[i].LatitudeDeg() << ',' << geoPos[i].LongitudeDeg() <<
			   ',' << geoPos[i].AltitudeKm() << endl;
	   }
   }
   myfile.close();

   // Save Satellite ECEF position file
   myfile.open("Satellite Data/Satellite_Pos_ECEF.csv", ios::trunc);
   myfile << "T since,X,Y,Z,,," << tle[0] << endl;
   for (unsigned int i = 0; i < ecefPos.size(); i++)
   {
	   radius = sqrt(powl(ecefPos[i].Position().m_x, 2) + powl(ecefPos[i].Position().m_y, 2)
		   + powl(ecefPos[i].Position().m_z, 2));
	   if (i == 0)
	   {
		   myfile << i << ',' << ecefPos[i].Position().m_x << ',' << ecefPos[i].Position().m_y <<
			   ',' << ecefPos[i].Position().m_z << ",,,"
			   << tle[1] << endl;

	   }
	   else if (i == 1)
	   {
		   myfile << i << ',' << ecefPos[i].Position().m_x << ',' << ecefPos[i].Position().m_y <<
			   ',' << ecefPos[i].Position().m_z << ",,,"
			   << tle[2] << endl;
	   }
	   else if (i == 2)
	   {
		   myfile << i << ',' << ecefPos[i].Position().m_x << ',' << ecefPos[i].Position().m_y <<
			   ',' << ecefPos[i].Position().m_z << ",,,"
			   << "Singapore UTC+8:" << ",,"
			   << date.ToTime().tm_mday << '/'
			   << date.ToTime().tm_mon << '/'
			   << date.ToTime().tm_year << ' '
			   << date.ToTime().tm_hour << ':'
			   << date.ToTime().tm_min << ':'
			   << date.ToTime().tm_sec << endl;
	   }
	   else
	   {
		   myfile << i << ',' << ecefPos[i].Position().m_x << ',' << ecefPos[i].Position().m_y <<
			   ',' << ecefPos[i].Position().m_z << endl;
	   }
   }
   myfile.close();

   // Save Satellite Velocity file
   myfile.open("Satellite Data/Satellite_Velocity_ECEF.csv", ios::trunc);
   myfile << "T since,Xdot,Ydot,Zdot,,," << tle[0] << endl;
   for (unsigned int i = 0; i < ecefPos.size(); i++)
   {
	   if (i == 0)
	   {
		   myfile << i << ',' << ecefPos[i].Velocity().m_x << ',' << ecefPos[i].Velocity().m_y <<
			   ',' << ecefPos[i].Velocity().m_z << ",,,"
			   << tle[1] << endl;

	   }
	   else if (i == 1)
	   {
		   myfile << i << ',' << ecefPos[i].Velocity().m_x << ',' << ecefPos[i].Velocity().m_y <<
			   ',' << ecefPos[i].Velocity().m_z << ",,,"
			   << tle[2] << endl;
	   }
	   else if (i == 2)
	   {
		   myfile << i << ',' << ecefPos[i].Velocity().m_x << ',' << ecefPos[i].Velocity().m_y <<
			   ',' << ecefPos[i].Velocity().m_z << ",,,"
			   << "Singapore UTC+8:" << ",,"
			   << date.ToTime().tm_mday << '/'
			   << date.ToTime().tm_mon << '/'
			   << date.ToTime().tm_year << ' '
			   << date.ToTime().tm_hour << ':'
			   << date.ToTime().tm_min << ':'
			   << date.ToTime().tm_sec << endl;
	   }
	   else
	   {
		   myfile << i << ',' << ecefPos[i].Velocity().m_x << ',' << ecefPos[i].Velocity().m_y <<
			   ',' << ecefPos[i].Velocity().m_z << endl;
	   }
   }
   myfile.close();
}