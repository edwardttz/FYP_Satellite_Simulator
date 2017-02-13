#include <iostream>
#include "SunSensor.h"
#include "Storage.h"
#include "eciSun.h"
//#include <stdio.h>

eciSun eci;

using namespace std;

int main(void)
{
	// Testing parameters for sun vector
	// A.D. 2017 Feb 13	03:41:04.0
	double JD = 2457797.653519;
	for (int i = 0; i < 35040; i++) {
		eci.setJulianDate(JD);
		eci.calculateSunVec();
		eci.computeSunAziEle();
		eci.storeValues();
		// ~Sampled every 15 mins
		// JD += 0.10417;
	}
}