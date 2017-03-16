#pragma once

using namespace std;

class sVector
{
public:
	sVector(double x = 0.0, double y = 0.0, double z = 0.0, double a = 0.0, double e = 0.0, 
		double u = 0.0, double m = 0.0)
		: x(x), y(y), z(z), u(u), m(m), azi(a), ele(e)
	{
	}

	virtual ~sVector() {};

	void Sub(const sVector&);     // subtraction
	void Mul(double factor);      // multiply each component by 'factor'

	double Angle_2D(const sVector&) const;    // angle between two vectors
	double Angle_3D(const sVector&) const;    // angle between two vectors
	double Magnitude_2D() const;              // 2D magnitude
	double Magnitude_3D() const;			  // 3D magnitude
	double Dot_3D(const sVector& vec) const;  // 3D dot product between two vectors
	double Dot_2D(const sVector& vec) const;  // 2D dot product between two vectors
	double computeElevation(const sVector& vec) const;
	double computeAzimuth(const sVector& vec) const;
	double Cross_3D_x(double, double) const;
	double Cross_3D_y(double, double) const;
	double Cross_3D_z(double, double) const;


	double x;
	double y;
	double z;
	double u;
	double m;
	double azi;
	double ele;
};

