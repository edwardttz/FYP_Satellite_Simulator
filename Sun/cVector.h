#pragma once

using namespace std;

class cVector
{
public:
	cVector(double x = 0.0, double y = 0.0, double z = 0.0, double a = 0.0, double e = 0.0, double b = 0.0)
		: sun_x(x), sun_y(y), sun_z(z), sun_azi(a), sun_ele(e), body_frame(b)
	{
	}

	virtual ~cVector() {};

	void Sub(const cVector&);     // subtraction
	void Mul(double factor);      // multiply each component by 'factor'

	double Angle(const cVector&) const;    // angle between two vectors
	double Magnitude() const;              // vector magnitude
	double Dot(const cVector& vec) const;  // dot product

	double sun_x;
	double sun_y;
	double sun_z;
	double sun_azi;
	double sun_ele;
	double body_frame;
};

