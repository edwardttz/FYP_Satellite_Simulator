#pragma once

#include "cVector.h";

using namespace std;

class SunSensorModel
{

public:
	SunSensorModel();	
	const cVector& Plane() const { return plane; }

	void setPlane(double, double, double);
	void computeSensorVector(EciSun e);

protected:
	cVector plane;
};
