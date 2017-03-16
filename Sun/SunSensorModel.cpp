#pragma once

#include "stdafx.h"

using namespace std;

SunSensorModel::SunSensorModel()
{	
}

//*****************************************************************************
// Computes the sun vector that is normal to each plane face
//*****************************************************************************
void SunSensorModel::computeSensorVector(EciSun e)
{
	// Plane normal is x
	sensorX.x = 1;
	sensorX.y = 0;
	sensorX.z = 0;

	sensorX.azi = sensorX.computeAzimuth(e.BodyPosition());
	if (sensorX.azi > 70 || sensorX.azi < -70) {
		sensorX.azi = NULL;
	}
	sensorX.ele = sensorX.computeElevation(e.BodyPosition());

	// Plane normal is y
	sensorY.x = 0;
	sensorY.y = 1;
	sensorY.z = 0;

	sensorY.azi = sensorY.computeAzimuth(e.BodyPosition());
	if (sensorY.azi > 70 || sensorY.azi < -70) {
		sensorY.azi = NULL;
	}
	sensorY.ele = sensorY.computeElevation(e.BodyPosition());

	// Plane normal is -x
	nSensorX.x = -1;
	nSensorX.y = 0;
	nSensorX.z = 0;

	nSensorX.azi = nSensorX.computeAzimuth(e.BodyPosition());
	if (nSensorX.azi > 70 || nSensorX.azi < -70) {
		nSensorX.azi = NULL;
	}
	nSensorX.ele = nSensorX.computeElevation(e.BodyPosition());

	// Plane normal is -y
	nSensorY.x = 0;
	nSensorY.y = -1;
	nSensorY.z = 0;

	nSensorY.azi = nSensorY.computeAzimuth(e.BodyPosition());
	if (nSensorY.azi > 70 || nSensorY.azi < -70) {
		nSensorY.azi = NULL;
	}
	nSensorY.ele = nSensorY.computeElevation(e.BodyPosition());
}

// Explain the theoretical.. and noise
//how these 2 part...

//ECI north vector projection of the sun vector onto the sun sensor plane.
//Plots of a satellite ground track... always a curve.
//

// Show the real devices characteristics
// Show your noise devices characteristics