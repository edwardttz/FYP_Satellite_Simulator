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
   string str2 = "1 25544U 98067A   16270.20968569  .00003031  00000-0  53388-4 0  9999";
   string str3 = "2 25544  51.6452 274.0859 0006285   7.4866  93.6781 15.53934258 20700";

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

   // Calculate the position and velocity of the satellite for various times.
   // mpe = "minutes past epoch"
   for (int mpe = 0; mpe <= (360 * 4); mpe += 60)
   {
      // Get the position of the satellite at time "mpe"
      cEciTime eci = sat.PositionEci(mpe);
    
      // Push the coordinates object onto the end of the vector.
      vecPos.push_back(eci);
   }

   // Print TLE data
   printf("%s\n",   sat.Name().c_str());
   printf("%s\n",   sat.Orbit().TleLine1().c_str());
   printf("%s\n\n", sat.Orbit().TleLine2().c_str());

   // Header
   printf("  TSINCE            X                Y                Z\n\n");

   // Iterate over each of the ECI position objects pushed onto the
   // position vector, above, printing the ECI position information
   // as we go.
   for (unsigned int i = 0; i < vecPos.size(); i++)
   {
      printf("%8d.00  %16.8f %16.8f %16.8f\n",
               i * 60,
               vecPos[i].Position().m_x,
               vecPos[i].Position().m_y,
               vecPos[i].Position().m_z);
   }

   printf("\n                    XDOT             YDOT             ZDOT\n\n");

   // Iterate over each of the ECI position objects in the position
   // vector again, but this time print the velocity information.
   for (unsigned int i = 0; i < vecPos.size(); i++)
   {
      printf("             %16.8f %16.8f %16.8f\n",
             vecPos[i].Velocity().m_x,
             vecPos[i].Velocity().m_y,
             vecPos[i].Velocity().m_z);
   }

   printf("\n");
}
