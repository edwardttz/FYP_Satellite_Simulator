#pragma once

#include "stdafx.h";

using namespace std;

SunSensorModel::SunSensorModel()
{	
}

void SunSensorModel::setPlane(double x, double y, double z)
{
	// Sets the sensor plane
	plane.x = x;
	plane.y = y;
	plane.z = z;
}

void SunSensorModel::computeSensorVector(EciSun e)
{
	// Computes the dot product of the azimuth 
	plane.azi = plane.Angle_2D(e.BodyPosition());
	plane.azi = rad2deg(plane.azi);

	plane.ele = plane.Angle_3D(e.BodyPosition());
	plane.ele = rad2deg(plane.ele);
}
