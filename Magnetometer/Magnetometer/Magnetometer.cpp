// Magnetometer.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int calculateMagField(double lat, double lon, double h, double t);
double calculateDecimalYear(double julianDate);

int main() {
	double lat = 11.72833442, lon = -88.76596036, alt = 403.7934671, jDate = 2457767.511806;
	int value = calculateMagField(lat, lon, alt, jDate);
	return value;
}

// 
// Input: ECEF Latitude, Longitude and Altitude values, julianDate
// Output: Horizontal Intensity(H), Total Intensity(F), Declination(D),
// Inclination(I), East Component(Bx), North Component(By), Vertical Component(Bz)
//
int calculateMagField(double lat, double lon, double h, double t) {
	try {
		MagneticModel mag("wmm2015", "../GeographicLib/magnetic");
		double Bx, By, Bz;
		t = calculateDecimalYear(t);
		mag(t, lat, lon, h * 1000, Bx, By, Bz);
		double H, F, D, I;
		MagneticModel::FieldComponents(Bx, By, Bz, H, F, D, I);
		Bx = fabs(Bx);
		By = fabs(By);
		Bz = fabs(Bz);
		cout << H << " " << F << " " << D << " " << I << "\n";
	}
	catch (const exception& e) {
		cerr << "Caught exception: " << e.what() << "\n";
		return 1;
	}
	return 0;
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