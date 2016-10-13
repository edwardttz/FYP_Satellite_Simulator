#include "SpacecraftDynamics.h"
#include <iostream>
#include <vector>

using namespace std;

SpacecraftDynamics::SpacecraftDynamics() 
{
	wX_values.setFileName("wX.txt");
	wY_values.setFileName("wY.txt");
	wZ_values.setFileName("wZ.txt");
	aX_values.setFileName("aX.txt");
	aY_values.setFileName("aY.txt");
	aZ_values.setFileName("aZ.txt");
	q0_values.setFileName("q0.txt");
	qX_values.setFileName("qX.txt");
	qY_values.setFileName("qY.txt");
	qZ_values.setFileName("qZ.txt");
	wX_values.clearFile();
	wY_values.clearFile();
	wZ_values.clearFile();
	aX_values.clearFile();
	aY_values.clearFile();
	aZ_values.clearFile();
	q0_values.clearFile();
	qX_values.clearFile();
	qY_values.clearFile();
	qZ_values.clearFile();
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

double SpacecraftDynamics::rungeKuttaQuaternions (double row1, double row2, double row3, double row4, double q_initial)
{
	double k1, k2, k3, k4;

	k1 = 0.5 * ((row1 * getQuaternion0()) + (row2 * getQuaternionX()) + (row3 * getQuaternionY()) + (row4 * getQuaternionZ()));

	k2 = 0.5 * ((row1 * (getQuaternion0() + (stepSize / 2) * k1)) + (row2 * (getQuaternionX() + (stepSize / 2) * k1)) 
		+ (row3 * (getQuaternionY() + (stepSize / 2) * k1)) + (row4 * (getQuaternionZ() + (stepSize / 2) * k1)));

	k3 = 0.5 * ((row1 * (getQuaternion0() + (stepSize / 2) * k2)) + (row2 * (getQuaternionX() + (stepSize / 2) * k2)) 
		+ (row3 * (getQuaternionY() + (stepSize / 2) * k2)) + (row4 * (getQuaternionZ() + (stepSize / 2) * k2)));

	k4 = 0.5 * ((row1 * (getQuaternion0() + (stepSize * k3))) + (row2 * (getQuaternionX() + (stepSize * k3))) 
		+ (row3 * (getQuaternionY() + (stepSize * k3))) + (row4 * (getQuaternionZ() + (stepSize * k3))));

	return q_initial + (stepSize / 6) * (k1 + 2 * k2 + 2 * k3 + k4);
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
	q0_values.storeInFile(q0);
	qX_values.storeInFile(qX);
	qY_values.storeInFile(qY);
	qZ_values.storeInFile(qZ);
}

/**
double SpacecraftDynamics::vectorMultiplication(vector<double> row, vector<double> column)
{
	double answer = 0.0;

	for (int i = 0; i < column.max_size; i++)
	{
		answer += row[i] * row[i];
	}

	return answer;
}
**/

/**
vector<double> SpacecraftDynamics::assignQuaternionValue(double value, vector<double> quaternionTemp)
{
	quaternionTemp.push_back(value);

	return quaternionTemp;
}
**/

void SpacecraftDynamics::findNextQuaternion()
{
	double q0_next = rungeKuttaQuaternions(0, -getVelocityX(), -getVelocityY(), -getVelocityZ(), getQuaternion0());
	double qX_next = rungeKuttaQuaternions(getVelocityX(), 0, getVelocityZ(), -getVelocityY(), getQuaternionX());
	double qY_next = rungeKuttaQuaternions(getVelocityY(), -getVelocityZ(), 0, getVelocityX(), getQuaternionY());
	double qZ_next = rungeKuttaQuaternions(getVelocityZ(), getVelocityY(), -getVelocityX(), 0, getQuaternionZ());
	q0 = q0_next;
	qX = qX_next;
	qY = qY_next;
	qZ = qZ_next;
}

void SpacecraftDynamics::setQuaternionInitialValues(double quaternion0, double quaternionX, double quaternionY, double quaternionZ)
{
	q0 = quaternion0;
	qX = quaternionX;
	qY = quaternionY;
	qZ = quaternionZ;
}

double SpacecraftDynamics::getVelocityX()
{
	return wX;
}

double SpacecraftDynamics::getVelocityY()
{
	return wY;
}

double SpacecraftDynamics::getVelocityZ()
{
	return wZ;
}

double SpacecraftDynamics::getQuaternion0()
{
	return q0;
}

double SpacecraftDynamics::getQuaternionX()
{
	return qX;
}

double SpacecraftDynamics::getQuaternionY()
{
	return qY;
}

double SpacecraftDynamics::getQuaternionZ()
{
	return qZ;
}

