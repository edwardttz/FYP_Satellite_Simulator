#ifndef SPACECRAFTDYNAMICS_H
#define SPACECRAFTDYNAMICS_H

#include <string>
#include <vector>
#include <cmath>
#include "Storage.h"
using namespace std;

class SpacecraftDynamics : public Storage 
{
private:
	static double stepSize;
	static double constantX;
	static double constantY;
	static double constantZ;

	static double wX;
	static double wY;
	static double wZ;

	static double aX;
	static double aY;
	static double aZ;

	static double iX;
	static double iY;
	static double iZ;

	static double torqueX;
	static double torqueY;
	static double torqueZ;

	double spaceTorqueX, spaceTorqueY, spaceTorqueZ;
	double reactionTorqueX, reactionTorqueY, reactionTorqueZ;
	double q0, qX, qY, qZ;
	double vector0, vectorX, vectorY, vectorZ;
	double q0Inverse, qXInverse, qYInverse, qZInverse;
	double thetaX, thetaY, thetaZ;
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
	Storage vector0_values;
	Storage vectorX_values;
	Storage vectorY_values;
	Storage vectorZ_values;
	Storage thetaX_values;
	Storage thetaY_values;
	Storage thetaZ_values;

public:
	SpacecraftDynamics();
	void getUserInput();
	void setSatellite(string);
	double rungeKutta (double, double, double, double, double, double);
	double rungeKuttaQuaternions (double, double, double, double, double);
	double eulerMethodQuaternions(double, double, double, double, double);
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
	void setQuaternionInverseInitialValues(double, double, double, double);
	void setVectorInitialValues(double, double, double, double);
	double getVelocityX();
	double getVelocityY();
	double getVelocityZ();
	double getAccX();
	double getAccY();
	double getAccZ();
	double getQuaternion0();
	double getQuaternionX();
	double getQuaternionY();
	double getQuaternionZ();
	double getTorqueX();
	double getTorqueY();
	double getTorqueZ();
	double getMOIx();
	double getMOIy();
	double getMOIz();
	double getconstantX();
	double getconstantY();
	double getconstantZ();
	double tempRKwX();
	void getNextw();
	void getNextAcc();
	double tempRKwY();
	double tempRKwZ();
	double getQ0Inverse();
	double getQXInverse();
	double getQYInverse();
	double getQZInverse();
	double getVector0();
	double getVectorX();
	double getVectorY();
	double getVectorZ();
	double quaternion0Mulitplication(double, double, double, double, double, double, double, double);
	double quaternionXMulitplication(double, double, double, double, double, double, double, double);
	double quaternionYMulitplication(double, double, double, double, double, double, double, double);
	double quaternionZMulitplication(double, double, double, double, double, double, double, double);
	double inverseQuaternionVal(double);
	double findMagnitude(double, double, double, double);
	void findNextVector();
	void findThetaValues();
	double RKwX();
	double RKwY();
	double RKwZ();
	void getNexttempw();
	 
};
 


#endif