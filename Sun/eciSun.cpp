#include "eciSun.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

eciSun::eciSun()
{
	sunVecX.setFileName("eciSun_X.txt");
	sunVecY.setFileName("eciSun_Y.txt");
	sunVecZ.setFileName("eciSun_Z.txt");
	sunAzi.setFileName("eciSunAzi.txt");
	sunEle.setFileName("eciSunEle.txt");
}

void eciSun::storeValues()
{
	sunVecX.storeInFile(sunVec_X);
	sunVecY.storeInFile(sunVec_Y);
	sunVecZ.storeInFile(sunVec_Z);
	sunAzi.storeInFile(azimuth);
	sunEle.storeInFile(elevation);
}

void eciSun::calculateSunVec()
{
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
	unitSunVec();
}

void eciSun::unitSunVec()
{
	unitSunVec_X = cos(ecliptic * PI / 180);
	unitSunVec_Y = cos(eccentricity * PI / 180) * sin(ecliptic * PI / 180);
	unitSunVec_Z = sin(eccentricity * PI / 180) * sin(ecliptic * PI / 180);
	computeSunVec(unitSunVec_X, unitSunVec_Y, unitSunVec_Z);
}

void eciSun::computeSunVec(double unitVec_X, double unitVec_Y, double unitVec_Z)
{
	sunVec_X = unitVec_X * sunDistInKm;
	sunVec_Y = unitVec_Y * sunDistInKm;
	sunVec_Z = unitVec_Z * sunDistInKm;
}

void eciSun::computeSunAziEle()
{
	azimuth = (atan(sunVec_Y / sunVec_X)) * 180 / PI;
	elevation = (atan(sunVec_Z / sqrt(powl(sunVec_X,2) + powl(sunVec_Y, 2)))) * 180 / PI;
}

double eciSun::getSunUnitVecX()
{
	return unitSunVec_X;
}

double eciSun::getSunUnitVecY()
{
	return unitSunVec_Y;
}

double eciSun::getSunUnitVecZ()
{
	return unitSunVec_Z;
}

double eciSun::getSunVecX()
{
	return sunVec_X;
}

double eciSun::getSunVecY()
{
	return sunVec_Y;
}

double eciSun::getSunVecZ()
{
	return sunVec_Z;
}

void eciSun::setJulianDate(double JD)
{
	julianDate = JD;
}

double eciSun::getJulianDate()
{
	return julianDate;
}


