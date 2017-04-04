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
	// Plane normal is X
	sensorX.x = 1;
	sensorX.y = 0;
	sensorX.z = 0;

	sensorX.azi = sensorX.computeAzimuth(e.SunVec());
	if (sensorX.azi > 60 || sensorX.azi < -60) {
		sensorX.azi = NULL;
	}
	sensorX.ele = sensorX.computeElevation(e.SunVec());

	// Plane normal is Y
	sensorY.x = 0;
	sensorY.y = 1;
	sensorY.z = 0;

	sensorY.azi = sensorY.computeAzimuth(e.SunVec());
	if (sensorY.azi > 60 || sensorY.azi < -60) {
		sensorY.azi = NULL;
		sensorY.ele = NULL;
	}
	sensorY.ele = sensorY.computeElevation(e.SunVec());

	// Plane normal is -X
	nSensorX.x = -1;
	nSensorX.y = 0;
	nSensorX.z = 0;

	nSensorX.azi = nSensorX.computeAzimuth(e.SunVec());
	if (nSensorX.azi > 60 || nSensorX.azi < -60) {
		nSensorX.azi = NULL;
		nSensorX.ele = NULL;
	}
	nSensorX.ele = nSensorX.computeElevation(e.SunVec());

	// Plane normal is -Y
	nSensorY.x = 0;
	nSensorY.y = -1;
	nSensorY.z = 0;

	nSensorY.azi = nSensorY.computeAzimuth(e.SunVec());
	if (nSensorY.azi > 60 || nSensorY.azi < -60) {
		nSensorY.azi = NULL;
		nSensorY.ele = NULL;
	}
	nSensorY.ele = nSensorY.computeElevation(e.SunVec());

	// Plane normal is Z
	sensorZ.x = 0;
	sensorZ.y = 0;
	sensorZ.z = 1;

	sensorZ.azi = sensorZ.computeAzimuth(e.SunVec());
	if (sensorZ.azi > 60 || sensorZ.azi < -60) {
		sensorZ.azi = NULL;
		sensorZ.ele = NULL;
	}
	sensorZ.ele = sensorZ.computeElevation(e.SunVec());

	// Plane normal is -Z
	nSensorZ.x = 0;
	nSensorZ.y = 0;
	nSensorZ.z = -1;

	nSensorZ.azi = nSensorZ.computeAzimuth(e.SunVec());
	if (nSensorZ.azi > 60 || nSensorZ.azi < -60) {
		nSensorZ.azi = NULL;
		nSensorZ.ele = NULL;
	}
	nSensorZ.ele = nSensorZ.computeElevation(e.SunVec());
}
