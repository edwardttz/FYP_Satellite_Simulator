#include "cVector.h"
#include <cmath>

using namespace std;

//*****************************************************************************
// Multiply each component in the vector by 'factor'.
//*****************************************************************************

void cVector::Mul(double factor)
{
	sun_x *= factor;
	sun_y *= factor;
	sun_z *= factor;
}

//*****************************************************************************
// Subtract a vector from this one.
//*****************************************************************************

void cVector::Sub(const cVector& vec)
{
	sun_x -= vec.sun_x;
	sun_y -= vec.sun_y;
	sun_z -= vec.sun_z;
}

//*****************************************************************************
// Calculate the angle between this vector and another
//*****************************************************************************
		
double cVector::Angle(const cVector& vec) const
{
	return acos(Dot(vec) / (Magnitude() * vec.Magnitude()));
}

//*****************************************************************************
//
//*****************************************************************************

double cVector::Magnitude() const
{
	return sqrt((sun_x * sun_x) +
				(sun_y * sun_y) +
				(sun_z * sun_z));
}

//*****************************************************************************
// Return the dot product
//*****************************************************************************

double cVector::Dot(const cVector& vec) const
{
	return (sun_x * vec.sun_x) +
			(sun_y * vec.sun_y) +
			(sun_z * vec.sun_z);
}

