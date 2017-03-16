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
		const sVector& BodyPosition() const { return body_Position; }

		double julianDate;
		
		void setJulianDate(double);
		void calculateSunVec();
		void computeBodyFrame(double, double, double);

		double getJulianDate();

	protected:
		sVector sun_Position;
		sVector body_Position;
};
