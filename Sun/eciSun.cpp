#pragma once

#include "cVector.h"
#include "EciSun.h"
#include "globals.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Class EciSun
//////////////////////////////////////////////////////////////////////
EciSun::EciSun()
{
	calculateSunVec();
}
void EciSun::calculateSunVec()
{
	double UT1 = 0.0;
	double longMSUN = 0.0;
	double mSUN = 0.0;
	double eccentricity = 0.0;
	double ecliptic = 0.0;

	// Number of elapsed Julian centuries
	UT1 = (julianDate - 2451545) / 36525;

	// Mean longitude of sun in degrees
	longMSUN = 280.4606184 + 36000.77005361 * UT1;

	// Mean anomaly of sun in degrees
	mSUN = 357.5277233 + 35999.05034 * UT1;

	// Ecliptic longitude in degrees
	ecliptic = longMSUN + 1.914666471 * sin(mSUN * PI / 180) + 0.918994643*sin(2 * mSUN * PI / 180);

	// Obliquity of the ecliptic in degrees
	eccentricity = 23.439291 - 0.0130042 * UT1;

	sun_Position.sun_x = cos(ecliptic * PI / 180);
	sun_Position.sun_y = cos(eccentricity * PI / 180) * sin(ecliptic * PI / 180);
	sun_Position.sun_z = sin(eccentricity * PI / 180) * sin(ecliptic * PI / 180);

	// Unit Sun Vector to Sun Vector
	sun_Position.sun_x *= sunDistInKm;
	sun_Position.sun_y *= sunDistInKm;
	sun_Position.sun_z *= sunDistInKm;

	sun_Position.sun_azi = (atan(sun_Position.sun_y / sun_Position.sun_x)) * 180 / PI;
	sun_Position.sun_ele = (atan(sun_Position.sun_z / sqrt(powl(sun_Position.sun_x, 2) +
		powl(sun_Position.sun_y, 2)))) * 180 / PI;
}

/** 
//	Testing Purposes
void EciSun::computeBodyFrame(double eci)
{
	sun_Position.body_frame = -eci + (sun_Position.sun_x + sun_Position.sun_y + sun_Position.sun_z);
} **/

void EciSun::computeBodyFrame(double eciX, double eciY, double eciZ)
{
	sun_Position.body_frame = -(eciX + eciY + eciZ) + 
		(sun_Position.sun_x + sun_Position.sun_y + sun_Position.sun_z);
}

void EciSun::setJulianDate(double JD)
{
	julianDate = JD;
}

double EciSun::getJulianDate()
{
	return julianDate;
}


