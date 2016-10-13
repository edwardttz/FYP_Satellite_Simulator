#ifndef SPACECRAFTDYNAMICS_H
#define SPACECRAFTDYNAMICS_H

#include <string>
#include <vector>
#include "Storage.h"
using namespace std;

class SpacecraftDynamics : public Storage 
{
private:
	double stepSize;
	double constantX, constantY, constantZ;
	double wX, wY, wZ;
	double aX, aY, aZ;
	double iX, iY, iZ;
	double torqueX, torqueY, torqueZ;
	double spaceTorqueX, spaceTorqueY, spaceTorqueZ;
	double reactionTorqueX, reactionTorqueY, reactionTorqueZ;
	double q0, qX, qY, qZ;
	vector<double> quaternion;
	Storage wX_values;
	Storage wY_values;
	Storage wZ_values;
	Storage aX_values;
	Storage aY_values;
	Storage aZ_values;
	Storage q0_values;
	Storage qX_values;
	Storage qY_values;
	Storage qZ_values;

public:
	SpacecraftDynamics();
	double rungeKutta (double, double, double, double, double, double);
	double rungeKuttaQuaternions (double, double, double, double, double);
	void printValues (double, double, double, double, double, double, double); 
	void setStepSize(double);
	void setInitialW(double, double, double);
	void setTorque(double, double, double);
	void setMOIValues(double, double, double);
	void findConstants();
	void findNextW();
	void findAcc();
	void storeValues();
	//double vectorMultiplication(vector<double>, vector<double>);
	//vector<double> assignQuaternionValue(double, vector<double>);
	void findNextQuaternion();
	void setQuaternionInitialValues(double, double, double, double);
	double getVelocityX();
	double getVelocityY();
	double getVelocityZ();
	double getQuaternion0();
	double getQuaternionX();
	double getQuaternionY();
	double getQuaternionZ();

};
 


#endif