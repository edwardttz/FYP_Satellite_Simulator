#ifndef SUNSENSOR_H
#define SUNSENSOR_H

#include <string>
#include <vector>
#include <cmath>
#include "Storage.h"

using namespace std;
enum sunVector {sv_X, sv_Y, sv_Z};

class SunSensor : public Storage
{
private:
	double PI = 3.142;
	double x, y, z;
	double xy, yx, xz, yz, zx, zy;
	double mxy, myx, mxz, myz, mzx, mzy;
	double stepSize;
	double thetaX, thetaY, thetaZ;
	
	vector<double> quaternion;
	Storage x_values;
	Storage y_values;
	Storage z_values;

public:
	SunSensor();
	//void setStepSize(double);
	void storeValues();
	void calculateAngles(double, double);
	void setFOVSensorConstraints(double, double);
	void setFOVPerpSensorConstraints(double, double);
	void getSunAngles(double, double, double, double, double);
	void rad2deg();
	//void getSunVector(double *sunVector);
};



#endif