#include "stdafx.h"

using namespace std;

//*****************************************************************************
// Multiply each component in the vector by 'factor'.
//*****************************************************************************

void sVector::Mul(double factor)
{
	x *= factor;
	y *= factor;
	z *= factor;
}

//*****************************************************************************
// Subtract a vector from this one.
//*****************************************************************************

void sVector::Sub(const sVector& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
}

//*****************************************************************************
// Calculate the 2D/3D between this vector and another
//*****************************************************************************
		
double sVector::Angle_2D(const sVector& vec) const
{
	return acos(Dot_2D(vec) / (Magnitude_2D() * vec.Magnitude_2D()));
}

double sVector::Angle_3D(const sVector& vec) const
{
	return acos(Dot_3D(vec) / (Magnitude_3D() * vec.Magnitude_3D()));
}

//*****************************************************************************
// Calculate the magnitude for 2D
//*****************************************************************************

double sVector::Magnitude_2D() const
{
	return sqrt((x * x) + (y * y));
}

//*****************************************************************************
// Calculate the magnitude for 3D
//*****************************************************************************

double sVector::Magnitude_3D() const
{
	return sqrt((x * x) + (y * y) + (z*z));
}

//*****************************************************************************
// Return the dot product of 2D
//*****************************************************************************

double sVector::Dot_2D(const sVector& vec) const
{
	return (x * vec.x) + (y * vec.y);
}

//*****************************************************************************
// Return the dot product of 3D
//*****************************************************************************

double sVector::Dot_3D(const sVector& vec) const
{
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}


//*****************************************************************************
// Computes the azimuth of the given faces
//*****************************************************************************
double sVector::computeAzimuth(const sVector& vec) const
{
	double result;

	result = rad2deg(Angle_2D(vec));

	//	+X face
	if (vec.y < 0 && x == 1) {
		result = -result;
	}

	//  -X face
	if (vec.y > 0 && x == -1) {
		result = -result;
	}

	//	+Y face
	if (vec.x < 0 && y == 1) {
		result = -result;
	}

	//	-Y face
	if (vec.x > 0 && y == -1) {
		result = -result;
	}

	return result;
}

//*****************************************************************************
// Computes the elevation
//*****************************************************************************
double sVector::computeElevation(const sVector& vec) const
{
	double aX, aY, aZ, aDotB, magA, magB, result;
	
	// The cross product of X(1,0,0) and Y(0,1,0) yields us a Z component
	aX = 0;
	aY = 0;
	aZ = 1;

	aDotB = abs(vec.x * aX + vec.y * aY + vec.z * aZ);
	magA = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));
	magB = sqrt(pow(aX, 2) + pow(aY, 2) + pow(aZ, 2));

	result = rad2deg((acos(aDotB / (magA * magB))));

	if (vec.y > 0) {
		result = 180 - result;
	}
	
	result -= 90;

	return result;
}




