#include "SpacecraftDynamics.h"
#include <iostream>

using namespace std;

SpacecraftDynamics::SpacecraftDynamics() 
{
	wX_values.setFileName("wX.txt");
	wY_values.setFileName("wY.txt");
	wZ_values.setFileName("wZ.txt");
	aX_values.setFileName("aX.txt");
	aY_values.setFileName("aY.txt");
	aZ_values.setFileName("aZ.txt");
	wX_values.clearFile();
	wY_values.clearFile();
	wZ_values.clearFile();
	aX_values.clearFile();
	aY_values.clearFile();
	aZ_values.clearFile();
}

double SpacecraftDynamics::rungeKutta (double torque, double MOI, double w_initial, double w1, double w2, double constant) 
{
	double k1,k2,k3,k4;
	k1 = (torque/MOI) - constant * w1 * w2;
	k2 = (torque/MOI) - constant * (w1 + (k1/2))*(w2 + (k1/2));
	k3 = (torque/MOI) - constant * (w1 + (k2/2))*(w2 + (k2/2));
	k4 = (torque/MOI) - constant * (w1 + k3)*(w2 + k3); 
	return w_initial + (stepSize/6) * (k1 + 2 * k2 + 2 * k3 + k4);
}

void SpacecraftDynamics::printValues (double iteration, double a_X,double a_Y, double a_Z, double w_X,double w_Y, double w_Z) 
{
	cout << "**** Iteration " << iteration << " ****" << endl;
	cout << "Angular Velocity: X=" << w_X << " Y=" << w_Y << " Z=" << w_Z << endl;
	cout << "Angular Acceleration: X=" << a_X << " Y=" << a_Y << " Z=" << a_Z << endl << endl;
}

void SpacecraftDynamics::findNextW() 
{
	double wX_next = rungeKutta(torqueX, iX, wX, wY, wZ, constantX);
	double wY_next = rungeKutta(torqueY, iY, wY, wX, wZ, constantY);
	double wZ_next = rungeKutta(torqueZ, iZ, wZ, wX, wY, constantZ);
	wX = wX_next;
	wY = wY_next;
	wZ = wZ_next;
}

void SpacecraftDynamics::findAcc() 
{
	aX = (torqueX / iX) - constantX * wY * wZ;
	aY = (torqueY / iY) - constantY * wX * wZ;
	aZ = (torqueZ / iZ) - constantZ * wX * wY;
}

void SpacecraftDynamics::findConstants() 
{
	constantX = (iZ - iY) / iX;
	constantY = (iX - iZ) / iY;
	constantZ = (iY - iX) / iZ;
}

void SpacecraftDynamics::setInitialW(double xValue, double yValue, double zValue) 
{
	wX = xValue;
	wY = yValue;
	wZ = zValue;
}

void SpacecraftDynamics::setTorque(double xValue, double yValue, double zValue) 
{
	torqueX = xValue;
	torqueY = yValue;
	torqueZ = zValue;
}

void SpacecraftDynamics::setMOIValues(double xValue, double yValue, double zValue) 
{
	iX = xValue;
	iY = yValue;
	iZ = zValue;
}

void SpacecraftDynamics::setStepSize(double timeInMS) 
{
	stepSize = timeInMS;
}

void SpacecraftDynamics::storeValues() 
{
	wX_values.storeInFile(wX);
	wY_values.storeInFile(wY);
	wZ_values.storeInFile(wZ);
	aX_values.storeInFile(aX);
	aY_values.storeInFile(aY);
	aZ_values.storeInFile(aZ);
}


