// Magnetometer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

double * calculateMagField(double lat, double lon, double h, double t);
double calculateDecimalYear(double julianDate);

int main() {
	//Input
	//ECEF lat, long, alt, julian Date
	double lat = 11.72833442, lon = -88.76596036, alt = 403.7934671, jDate = 2457767.511806;

	// Output
	// magFieldValues = [Bx, By, Bz, H, F, D, I]
	double *magFieldValues;
	magFieldValues = calculateMagField(lat, lon, alt, jDate);
/*
 *	How each value is being called from pointer array
 *	Print
 */
	for(int i = 0; i <=6; i++) {
		cout << *(magFieldValues + i) << endl;
	}
	return 0;
}

// 
// Input: ECEF Latitude, Longitude and Altitude values, julianDate
// Output: Horizontal Intensity(H), Total Intensity(F), Declination(D),
// Inclination(I), East Component(Bx), North Component(By), Vertical Component(Bz)
//
double * calculateMagField(double lat, double lon, double h, double t) {
	
	MagneticModel mag("wmm2015", "../GeographicLib/magnetic");
	static double magFieldValues[7];
	double Bx, By, Bz, H, F, D, I;
	t = calculateDecimalYear(t);
	mag(t, lat, lon, h * 1000, Bx, By, Bz);
	MagneticModel::FieldComponents(Bx, By, Bz, H, F, D, I);
	Bx = fabs(Bx);
	By = fabs(By);
	Bz = fabs(Bz);
	magFieldValues[0] = Bx * (100.2 / 100.0);
	magFieldValues[1] = By * (100.2 / 100.0);
	magFieldValues[2] = Bz * (100.2 / 100.0);
	magFieldValues[3] = H;
	magFieldValues[4] = F * (100.1 / 100.0);
	magFieldValues[5] = D;
	magFieldValues[6] = I;
	return magFieldValues;
}
double calculateDecimalYear(double julianDate) {
	int daysAYear = 365, leapDay, nYear;
	double refDate = 2451544.5; //Date: 2000/1/1
	double decimalValue, decYear;
	decimalValue = julianDate - (int)julianDate;
	if (decimalValue < 0.5) {
		julianDate = (int)julianDate - 1 + 0.5;
	} 
	else {
		julianDate = (int)julianDate + 0.5;
	}
	julianDate -= refDate;
	nYear = julianDate / daysAYear;
	leapDay = nYear / 4;
	julianDate -= leapDay;
	julianDate -= nYear * daysAYear;
	decYear = nYear + 2000 + julianDate / daysAYear;
	return decYear;
}