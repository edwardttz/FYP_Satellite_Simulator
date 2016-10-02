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

// "coreLib.h" includes basic types from the core library,
// such as cSite, cJulian, etc. The header file also contains a
// "using namespace" statement for Zeptomoby::OrbitTools.
#include "coreLib.h"

// "orbitLib.h" includes basic types from the orbit library,
// including cOrbit.
#include "orbitLib.h"

// Forward declaration of helper function; see below
void PrintPosVel(const cSatellite& sat);

//////////////////////////////////////////////////////////////////////////////
int main(int /* argc */, char* /* argv[] */)
{
   // Test SGP4 TLE data
   string str1 = "SGP4 Test";
   string str2 = "1 25544U 98067A   16274.13098880  .00005780  00000-0  94956-4 0  9994";
   string str3 = "2 25544  51.6434 254.5439 0006578  21.2685  71.2382 15.53986571 21317";

   // Create a TLE object using the data above
   cTle tleSGP4(str1, str2, str3);

  // Create a satellite object from the TLE object
   cSatellite satSGP4(tleSGP4);
   
   // Print the position and velocity information of the satellite
   PrintPosVel(satSGP4);

   //cJulian date
   // Now create a site object. Site objects represent a location on the 
   // surface of the earth. Here we arbitrarily select a point on the
   // equator.
   //cSite siteEquator(0.0, -100.0, 0); // 0.00 N, 100.00 W, 0 km altitude

   // Now get the "look angle" from the site to the satellite. 
   // Note that the ECI object "eciSDP4" contains a time associated
   // with the coordinates it contains; this is the time at which
   // the look angle is valid.
   //cTopo topoLook = siteEquator.GetLookAngle(eciSDP4);

   // Print out the results.
   //printf("AZ: %.3f  EL: %.3f\n", 
   //       topoLook.AzimuthDeg(),
   //       topoLook.ElevationDeg());
}

/////////////////////////////////////////////////////////////////////////////
// Helper function to output position and velocity information
void PrintPosVel(const cSatellite& sat)
{
   vector<cEci> vecPos;
   long double radius = 0.0;
   ofstream myfile;
   myfile.precision(10);
   // Calculate the position and velocity of the satellite for various times.
   // mpe = "minutes past epoch"
   for (int mpe = 0; mpe <= (120); mpe += 1)
   {
      // Get the position of the satellite at time "mpe"
      cEciTime eci = sat.PositionEci(mpe);
    
      // Push the coordinates object onto the end of the vector.
      vecPos.push_back(eci);
   }

   // Open ECI position, Radius file
   myfile.open("Satellite_Pos_Rad_ECI.csv", ios::trunc);
   myfile << "T since,X,Y,Z,Radius,,," << sat.Name().c_str() << endl;
   for (unsigned int i = 0; i < vecPos.size(); i++)
   {
	   radius = sqrt(powl(vecPos[i].Position().m_x, 2) + powl(vecPos[i].Position().m_y, 2)
		   + powl(vecPos[i].Position().m_z, 2));
	   if (i == 0) 
	   {
		   myfile << i << ',' << vecPos[i].Position().m_x << ',' << vecPos[i].Position().m_y << 
			   ',' << vecPos[i].Position().m_z << ',' << radius << ",,," 
			   << sat.Orbit().TleLine1().c_str() << endl;
			   
	   }
	   else if (i == 1) 
	   {
		   myfile << i << ',' << vecPos[i].Position().m_x << ',' << vecPos[i].Position().m_y <<
			   ',' << vecPos[i].Position().m_z << ',' << radius << ",,," 
			   << sat.Orbit().TleLine2().c_str() << endl;
	   }
	   else if (i == 2)
	   {
		   int month = sat.Orbit().Epoch().ToTime().tm_mon + 1;

		   myfile << i << ',' << vecPos[i].Position().m_x << ',' << vecPos[i].Position().m_y <<
			   ',' << vecPos[i].Position().m_z << ',' << radius << ",,," 
			   << sat.Orbit().Epoch().ToTime().tm_mday << '/'
			   << month << '/'
			   << sat.Orbit().Epoch().ToTime().tm_year << ' '
			   << sat.Orbit().Epoch().ToTime().tm_hour << ':'
			   << sat.Orbit().Epoch().ToTime().tm_min << ':'
			   << sat.Orbit().Epoch().ToTime().tm_sec << endl;
	   }
	   else 
	   {
		   myfile << i << ',' << vecPos[i].Position().m_x << ',' << vecPos[i].Position().m_y <<
			   ',' << vecPos[i].Position().m_z << ',' << radius << endl;
	   }   
   }
   myfile.close();
   
   // Open Velocity file
   myfile.open("Satellite_Velocity.csv", ios::trunc);
   myfile << "T since,Xdot,Ydot,Zdot,,,," << sat.Name().c_str() << endl;
   for (unsigned int i = 0; i < vecPos.size(); i++)
   {
	   if (i == 0)
	   {
		   myfile << i << ',' << vecPos[i].Velocity().m_x << ',' << vecPos[i].Velocity().m_y <<
			   ',' << vecPos[i].Velocity().m_z << ",,,," 
			   << sat.Orbit().TleLine1().c_str() << endl;

	   }
	   else if (i == 1)
	   {
		   myfile << i << ',' << vecPos[i].Velocity().m_x << ',' << vecPos[i].Velocity().m_y <<
			   ',' << vecPos[i].Velocity().m_z << ",,,," 
			   << sat.Orbit().TleLine2().c_str() << endl;
	   }
	   else if (i == 2)
	   {
		   myfile << i << ',' << vecPos[i].Position().m_x << ',' << vecPos[i].Position().m_y <<
			   ',' << vecPos[i].Position().m_z << ',' << radius << ",,,"
			   << sat.Orbit().Epoch().ToTime().tm_mday << '/'
			   << sat.Orbit().Epoch().ToTime().tm_mon << '/'
			   << sat.Orbit().Epoch().ToTime().tm_mon << ' '
			   << sat.Orbit().Epoch().ToTime().tm_hour << ':'
			   << sat.Orbit().Epoch().ToTime().tm_min << ':'
			   << sat.Orbit().Epoch().ToTime().tm_sec << endl;
	   }
	   else
	   {
		   myfile << i << ',' << vecPos[i].Velocity().m_x << ',' << vecPos[i].Velocity().m_y <<
			   ',' << vecPos[i].Velocity().m_z << ",,"  << endl;
	   }
   }
   myfile.close();
}