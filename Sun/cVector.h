#pragma once

using namespace std;

class cVector
{
public:
	cVector(double x = 0.0, double y = 0.0, double z = 0.0, double a = 0.0, double e = 0.0, 
		double u = 0.0, double m = 0.0)
		: x(x), y(y), z(z), u(u), m(m), azi(a), ele(e)
	{
	}

	virtual ~cVector() {};

	void Sub(const cVector&);     // subtraction
	void Mul(double factor);      // multiply each component by 'factor'

	double Angle_2D(const cVector&) const;    // angle between two vectors
	double Angle_3D(const cVector&) const;    // angle between two vectors
	double Magnitude_2D() const;              // 2D magnitude
	double Magnitude_3D() const;			  // 3D magnitude
	double Dot_3D(const cVector& vec) const;  // dot product of azimuth
	double Dot_2D(const cVector& vec) const;  // dot product of elevation

	double x;
	double y;
	double z;
	double u;
	double m;
	double azi;
	double ele;
};

