#pragma once

#include "cVector.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// class EciSun
//////////////////////////////////////////////////////////////////////

class EciSun
{
	public:
		EciSun();
		const cVector& SunPosition() const { return sun_Position; }
		const cVector& BodyPosition() const { return body_Position; }

		double julianDate;
		
		void setJulianDate(double);
		void calculateSunVec();
		void computeBodyFrame(double, double, double);

		double getJulianDate();

	protected:
		cVector sun_Position;
		cVector body_Position;
};
