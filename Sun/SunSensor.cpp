
#include "SunSensor.h"
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

SunSensor::SunSensor()
{
	x_values.setFileName("x.txt");
	y_values.setFileName("y.txt");
	z_values.setFileName("z.txt");
}

/*void SunSensor::setStepSize(double timeInMS)
{
stepSize = timeInMS;
}*/


/*void SunSensor::setInitialValues(double vect_x, double vect_y, double vect_z)
{
	x = vect_x;
	y = vect_y;
	z = vect_z;
}*/

void SunSensor::storeValues()
{
	x_values.storeInFile(x);
	y_values.storeInFile(y);
	z_values.storeInFile(z);
}

// Determine calculation of theta and phi
void SunSensor::calculateAngles(double theta, double phi)
{
	double theta_rad, phi_rad;
	theta_rad = theta * PI / 180;
	phi_rad = phi * PI / 180;

	if (theta >= -90 & theta <= 90)
		x = 1;
	else
		x = -1;
	
	if (theta >= 0 & theta <= 180)
		y = 1;
	else
		y = -1;

	if (phi >= 0)
		z = 1;
	else
		z = -1;
	
	getSunAngles(theta_rad, phi_rad, x, y, z);
}

void SunSensor::getSunAngles(double theta, double phi, double x, double y, double z)
{
	if (x == 1)
	{
		xy = theta;
		xz = atan(tan(phi) / cos(theta));
		mxy = -PI / 2;
		mxz = -PI / 2;
	}
	else
	{
		xy = -PI / 2;
		xz = -PI / 2;
		mxy = PI - theta;
		if (mxy > PI)
			mxy = -(2 * PI - mxy);
		mxz = -atan(tan(phi) / cos(theta));
	}

	if (y == 1)
	{
		yx = PI / 2 - theta;
		yz = atan(tan(phi) / sin(theta));
		myx = -PI / 2;
		myz = -PI / 2;
	}
	else
	{
		yx = -PI / 2;
		yz = -PI / 2;
		myx = PI / 2 - theta;
		if (myx > PI / 2)
			myx = PI - myx;
		myz = -atan(tan(phi) / sin(theta));
	}

	if (z == 1)
	{
		zx = PI / 2 - atan(tan(phi) / cos(theta));
		if (zx > PI / 2)
			zx = zx - PI;
		zy = PI / 2 - atan(tan(phi) / sin(theta));
		if (zy > PI / 2)
			zy = zy - PI;
		mzx = -PI / 2;
		mzy = -PI / 2;
	}
	else
	{
		zx = -PI / 2;
		zy = -PI / 2;
		mzx = PI/ 2 - atan(tan(-phi) / cos(theta));
		if (mzx > PI / 2)
			mzx = mzx - PI;
		mzy = PI / 2 - atan(tan(-phi) / sin(theta));
		if (mzy > PI / 2)
			mzy = mzy - PI;
	}
	xy = xy * 180 / PI;
	xz = xz * 180 / PI;
	yx = yx * 180 / PI;
	yz = yz * 180 / PI;
	zx = zx * 180 / PI;
	zy = zy * 180 / PI;

	mxy = mxy * 180 / PI;
	mxz = mxz * 180 / PI;
	myx = myx * 180 / PI;
	myz = myz * 180 / PI;
	mzx = mzx * 180 / PI;
	mzy = mzy * 180 / PI;
}

// Exceeded FOV (Field of view) in sensor direction

void SunSensor::setFOVSensorConstraints(double xy, double xz)
{
	// Plane x
	if (xy < -70 || xy > 70)
		xy = -90;
	if (xz < -70 ||xz > 70)
		xz = -90;
	
	// Plane y
	if (yx < -70 || yx > 70)
		yx = -90;
	if (yz < -70 || yz > 70)
		yz = -90;

	// Plane z
	if (zx < -70 || zx > 70)
		zx = -90;
	if (zy < -70 || zy > 70)
		zy = -90;

	// Plane -x
	if (mxy < -70 || mxy > 70)
		mxy = -90;
	if (mxz < -70 || mxz > 70)
		mxz = -90;

	// Plane -y
	if (myx < -70 || myx > 70)
		myx = -90;
	if (myz < -70 || myz > 70)
		myz = -90;

	// Plane -z
	if (mzx < -70 || mzx > 70)
		mzx = -90;
	if (mzy < -70 || mzy > 70)
		mzy = -90;
}

// Exceeded field of view perpendicular to sensor direction
void SunSensor::setFOVPerpSensorConstraints(double xy, double xz)
{
	double tmpXY, tmpYX, tmpZX, mTmpXY, mTmpYX, mTmpZX;
	tmpXY = xy, tmpYX = yx, tmpZX = zx;
	mTmpXY = mxy, mTmpYX = myx, mTmpZX = mzx;
	
	// Plane x
	if (xz < -55 || xz > 55)
		xy = -90;
	if (tmpXY < -55 || tmpXY > 55)
		xz = -90;

	// Plane y
	if (yz < -55 || yz > 55)
		yx = -90;
	if (tmpYX < -55 || tmpYX > 55)
		yz = -90;

	// Plane z
	if (zy < -55 || zy > 55)
		zx = -90;
	if (tmpZX < -55 || tmpZX > 55)
		zy = -90;

	// Plane -x
	if (mxz < -55 || mxz > 55)
		mxy = -90;
	if (mTmpXY < -55 || mTmpXY > 55)
		mxz = -90;

	// Plane -y
	if (myz < -55 || myz > 55)
		myx = -90;
	if (mTmpYX < -55 || mTmpYX > 55)
		myz = -90;

	// Plane -z
	if (mzy < -55 || mzy > 55)
		mzx = -90;
	if (mTmpZX < -55 || mTmpZX > 55)
		mzy = -90;


}

//void SunSensor::setSensorConstraints(double xy, double xz, double yx, double yz, double zx, double zy)


/*void SunSensor::getSunVector(double* sunVector)
{
	sunVector[sv_X] = sin(phi) * cos(theta);
	sunVector[sv_Y] = sin(phi) * cos(theta);
	sunVector[sv_Z] = cos(phi);
}*/
