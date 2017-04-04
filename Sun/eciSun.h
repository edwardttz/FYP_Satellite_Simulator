#pragma once

#include "sVector.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// class EciSun
//////////////////////////////////////////////////////////////////////

class EciSun
{
	public:
		EciSun();
		const sVector& SunPosition() const { return sun_Position; }
		const sVector& SunVec() const { return sun_Vector; }

		double julianDate;
		
		void setJulianDate(double);
		void calculateSunVec();
		void computeBodySunVec(double, double, double);

		double getJulianDate();

	protected:
		sVector sun_Position;
		sVector sun_Vector;
};
