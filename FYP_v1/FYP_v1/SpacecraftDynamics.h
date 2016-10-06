#ifndef SPACECRAFTDYNAMICS_H
#define SPACECRAFTDYNAMICS_H

#include <string>
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
	Storage wX_values;
	Storage wY_values;
	Storage wZ_values;
	Storage aX_values;
	Storage aY_values;
	Storage aZ_values;

public:
	SpacecraftDynamics();
	double rungeKutta (double, double, double, double, double, double);
	void printValues (double, double, double, double, double, double, double); 
	void setStepSize(double);
	void setInitialW(double, double, double);
	void setTorque(double, double, double);
	void setMOIValues(double, double, double);
	void findConstants();
	void findNextW();
	void findAcc();
	void storeValues();
};
 


#endif