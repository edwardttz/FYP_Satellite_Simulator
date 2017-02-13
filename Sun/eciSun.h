#ifndef SUN_H
#define SUN_H

#include "Storage.h"

using namespace std;

class eciSun
{
private:
	/** Parameters **/
	// double sunDistInAU = 1.00140612 - 0.016708617*cos(mSUN * PI / 180) - 0.000139589*cos(2 * mSUN * PI / 180);
	const double sunDistInKm = 149597870700;
	const double PI = 3.14159265359;

	double julianDate;
	double UT1;
	double longMSUN;
	double unitSunVec_X;
	double unitSunVec_Y;
	double unitSunVec_Z;
	double sunVec_X;
	double sunVec_Y;
	double sunVec_Z;
	double mSUN;
	double ecliptic;
	double eccentricity;
	double azimuth;
	double elevation;

	Storage sunVecX;
	Storage sunVecY;
	Storage sunVecZ;
	Storage sunAzi;
	Storage sunEle;

public:
	eciSun();
	void storeValues();
	void computeSunVec(double, double, double);
	void setJulianDate(double);
	void calculateSunVec();
	void computeSunAziEle();
	void unitSunVec();

	double getSunUnitVecX();
	double getSunUnitVecY();
	double getSunUnitVecZ();
	double getSunVecX();
	double getSunVecY();
	double getSunVecZ();
	double getJulianDate();
};

#endif