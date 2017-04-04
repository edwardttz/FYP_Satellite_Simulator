#pragma once

#include "stdafx.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Class EciSun
//////////////////////////////////////////////////////////////////////
EciSun::EciSun()
{
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

	sun_Position.x = cos(ecliptic * PI / 180);
	sun_Position.y = cos(eccentricity * PI / 180) * sin(ecliptic * PI / 180);
	sun_Position.z = sin(eccentricity * PI / 180) * sin(ecliptic * PI / 180);

	// Unit vector to Sun Vector
	sun_Position.x *= sunDistInKm;
	sun_Position.y *= sunDistInKm;
	sun_Position.z *= sunDistInKm;
}

void EciSun::computeBodySunVec(double eciX, double eciY, double eciZ)
{
	// Get the sun vector
	sun_Vector.x = (-eciX + sun_Position.x);
	sun_Vector.y = (-eciY + sun_Position.y);
	sun_Vector.z = (-eciZ + sun_Position.z);

	// Normalize the vectors
	sun_Vector.m = sun_Vector.Magnitude_2D();
	sun_Vector.x /= sun_Vector.m;
	sun_Vector.y /= sun_Vector.m;
	sun_Vector.z /= sun_Vector.m;
}

void EciSun::setJulianDate(double JD)
{
	julianDate = JD;
}

double EciSun::getJulianDate()
{
	return julianDate;
}


