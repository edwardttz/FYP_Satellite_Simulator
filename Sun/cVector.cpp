#include "cVector.h"
#include <cmath>

using namespace std;

//*****************************************************************************
// Multiply each component in the vector by 'factor'.
//*****************************************************************************

void cVector::Mul(double factor)
{
	x *= factor;
	y *= factor;
	z *= factor;
}

//*****************************************************************************
// Subtract a vector from this one.
//*****************************************************************************

void cVector::Sub(const cVector& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
}

//*****************************************************************************
// Calculate the angle between this vector and another
//*****************************************************************************
		
double cVector::Angle_2D(const cVector& vec) const
{
	return acos(Dot_2D(vec) / (Magnitude_2D() * vec.Magnitude_2D()));
}

double cVector::Angle_3D(const cVector& vec) const
{
	return acos(Dot_3D(vec) / (Magnitude_3D() * vec.Magnitude_3D()));
}

//*****************************************************************************
// Calculate the magnitude for 2D
//*****************************************************************************

double cVector::Magnitude_2D() const
{
	return sqrt((x * x) + (y * y));
}

//*****************************************************************************
// Calculate the magnitude for 3D
//*****************************************************************************

double cVector::Magnitude_3D() const
{
	return sqrt((x * x) + (y * y) + (z*z));
}

//*****************************************************************************
// Return the dot product of 2D
//*****************************************************************************

double cVector::Dot_2D(const cVector& vec) const
{
	return (x * vec.x) + (y * vec.y);
}

//*****************************************************************************
// Return the dot product of 3D
//*****************************************************************************

double cVector::Dot_3D(const cVector& vec) const
{
	return (x * vec.x) + (y * vec.y) + (z * vec.z);
}

