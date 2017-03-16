#pragma once

#include "sVector.h"

using namespace std;

class SunSensorModel
{

public:
	SunSensorModel();	

	/* Each satellite face returns the parameters
	We only need azimuth and elevation */

	const sVector& FaceX() const { return sensorX; }
	const sVector& FaceY() const { return sensorY; }
	const sVector& FaceZ() const { return sensorZ; }
	const sVector& nFaceX() const { return nSensorX; }
	const sVector& nFaceY() const { return nSensorY; }
	const sVector& nFaceZ() const { return nSensorZ; }
	const sVector& Plane() const { return plane; }

	void computeSensorVector(EciSun e);

protected:
	sVector sensorX;
	sVector sensorY;
	sVector sensorZ;
	sVector nSensorX;
	sVector nSensorY;
	sVector nSensorZ;
	sVector plane;
};
