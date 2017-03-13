// Magnetometer.cpp : Defines the entry point for the console application.
// All fields are in mG
//

#include "stdafx.h"

int main() {
	// Initialization
	vector<double> magFieldValues;

	// Output
	// magFieldValues = [Bx, By, Bz, H, F, D, I, qBx, qBy, qBz, qH, qF]
	/*
	 * Leslie: Use this instead.
	 * calcMagFieldMain(geoPos, date, magFieldValues)
	 * Need you to help me uncomment this function below and also in .h file
	 */

	// For Testing purposes
	//Input
	//ECEF lat, long, alt, julian Date
	double lat = 40.7077778, lon = -73.90527777, alt = 403.7934671, jDate = 2457814.5;

	//calculateMagField(lat, lon, alt, jDate, magFieldValues);

	testNoiseFloor();

	for (int i = 0; i < magFieldValues.size(); i++) {
		cout << magFieldValues[i] << endl;
	}
	return 0;
}
/*
void calcMagFieldMain(const cGeo geoPos, const cJulian date, vector<double>& magFieldValues) {
	double lat = geoPos.LatitudeDeg();
	double lon = geoPos.LongitudeDeg();
	double h = geoPos.AltitudeKm();
	double t = date.Date();
	calculateMagField(lat, lon, h, t, magFieldValues);
}*/
// 
// Input: ECEF Latitude, Longitude and Altitude values, julianDate
// Output: East Component(Bx), North Component(By), Vertical Component(Bz)
// Horizontal Intensity(H), Total Intensity(F), Declination(D), Inclination(I)
// Digital of Bx, By, Bz, H, F
//
void calculateMagField(const double lat, const double lon, const double h, const double t, vector<double>& magFieldValues) {
	
	MagneticModel mag("wmm2015", "../GeographicLib/magnetic");
	
	double Bx, By, Bz, H, F, D, I, Bx_temp, By_temp, Bz_temp;
	double time = calculateDecimalYear(t);
	mag(time, lat, lon, h * 1000, Bx, By, Bz);
	MagneticModel::FieldComponents(Bx, By, Bz, H, F, D, I);
	Bz = -Bz;
	
	// Conversion to mG (100nT = 1mG)
	Bx_temp = Bx / 100.0;
	By_temp = By / 100.0;
	Bz_temp = Bz / 100.0;
	H /= 100.0;
	F /= 100.0;
	
	// Include noise due to Cross-Axis Sensitivity
	Bx = Bx_temp + ((By_temp + Bz_temp) * 0.02);
	By = By_temp + ((Bx_temp + Bz_temp) * 0.02);
	Bz = Bz_temp + ((Bx_temp + By_temp) * 0.02);

	// Adding Noise floor
	Bx += genRandNoiseFloor(4)[99];
	By += genRandNoiseFloor(4)[99];
	Bz += genRandNoiseFloor(4)[99];

	H = sqrt(pow(Bx, 2) + pow(By, 2));
	F = sqrt(pow(Bx, 2) + pow(By, 2) + pow(Bz, 2));

	// Linearity 0.1% Full-Scale
	F *= (100.1 / 100); 

	// Analog values
	magFieldValues.push_back(Bx);
	magFieldValues.push_back(By);
	magFieldValues.push_back(Bz);
	magFieldValues.push_back(H);
	magFieldValues.push_back(F);
	magFieldValues.push_back(D);
	magFieldValues.push_back(I);

	// Digital (Quantized) values
	magFieldValues.push_back(quantizeValue(Bx));
	magFieldValues.push_back(quantizeValue(By));
	magFieldValues.push_back(quantizeValue(Bz));
	magFieldValues.push_back(quantizeValue(H));
	magFieldValues.push_back(quantizeValue(F));
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