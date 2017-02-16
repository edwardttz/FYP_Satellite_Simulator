#include "SpacecraftDynamics.h"
#include <iostream>
#include <vector>
#include <cmath>

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
	vector0_values.setFileName("vector0.txt");
	vectorX_values.setFileName("vectorX.txt");
	vectorY_values.setFileName("vectorY.txt");
	vectorZ_values.setFileName("vectorZ.txt");
	thetaX_values.setFileName("thetaX.txt");
	thetaY_values.setFileName("thetaY.txt");
	thetaZ_values.setFileName("thetaZ.txt");
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
	vector0_values.clearFile();
	vectorX_values.clearFile();
	vectorY_values.clearFile();
	vectorZ_values.clearFile();
	thetaX_values.clearFile();
	thetaY_values.clearFile();
	thetaZ_values.clearFile();
}

double SpacecraftDynamics::rungeKutta (double torque, double MOI, double w_initial, double w1, double w2, double constant) 
{
	double k1,k2,k3,k4;
	k1 = (torque/MOI) - constant * w1 * w2;
	k2 = (torque/MOI) - constant * (w1 + ((k1*stepSize)/2))*(w2 + ((k1*stepSize)/2));
	k3 = (torque/MOI) - constant * (w1 + ((k2*stepSize)/2))*(w2 + ((k2*stepSize)/2));
	k4 = (torque/MOI) - constant * (w1 + (k3*stepSize))*(w2 + (k3*stepSize)); 
	return w_initial + (stepSize/6) * (k1 + 2 * k2 + 2 * k3 + k4);
}

double SpacecraftDynamics::tempRKwX()
{
	double k1, k2, k3, k4;
	double tX = getTorqueX();
	double tY = getTorqueY();
	double tZ = getTorqueZ();
	double iX = getMOIx();
	double iY = getMOIy();
	double iZ = getMOIz();
	double accY = getAccY();
	double accZ = getAccZ();
	double wX = getVelocityX();
	k1 = stepSize * ((tX / iX) - ((iZ-iY)/iX) * ((tZ - accZ*iZ)/(iY-iX)*wX) * ((tY - accY*iY) / (iX - iZ)*wX));
	k2 = stepSize * ((tX / iX) - ((iZ - iY) / iX) * ((tZ - accZ*iZ) / (iY - iX)*(wX + k1/2)) * ((tY - accY*iY) / (iX - iZ)*(wX + k1/2)));
	k3 = stepSize * ((tX / iX) - ((iZ - iY) / iX) * ((tZ - accZ*iZ) / (iY - iX)*(wX + k2/2)) * ((tY - accY*iY) / (iX - iZ)*(wX + k2/2)));
	k4 = stepSize * ((tX / iX) - ((iZ - iY) / iX) * ((tZ - accZ*iZ) / (iY - iX)*(wX + k3)) * ((tY - accY*iY) / (iX - iZ)*(wX + k3)));
	return (wX + (k1 / 6) + (k2 / 3) + (k3 / 3) + (k4 / 6));
}

double SpacecraftDynamics::tempRKwY()
{
	double k1, k2, k3, k4;
	double tX = getTorqueX();
	double tY = getTorqueY();
	double tZ = getTorqueZ();
	double iX = getMOIx();
	double iY = getMOIy();
	double iZ = getMOIz();
	double accX = getAccX();
	double accZ = getAccZ();
	double wY = getVelocityY();
	k1 = stepSize * ((tY / iY) - ((iX - iZ) / iY) * ((tZ - accZ*iZ) / (iY - iX)*wY) * ((tX - accX*iX) / (iZ - iY)*wY));
	k2 = stepSize * ((tY / iY) - ((iX - iZ) / iY) * ((tZ - accZ*iZ) / (iY - iX)*(wY + k1/2)) * ((tX - accX*iX) / (iZ - iY)*(wY + k1/2)));
	k3 = stepSize * ((tY / iY) - ((iX - iZ) / iY) * ((tZ - accZ*iZ) / (iY - iX)*(wY + k2/2)) * ((tX - accX*iX) / (iZ - iY)*(wY + k2/2)));
	k4 = stepSize * ((tY / iY) - ((iX - iZ) / iY) * ((tZ - accZ*iZ) / (iY - iX)*(wY + k3)) * ((tX - accX*iX) / (iZ - iY)*(wY + k3)));
	return (wY + (k1 / 6) + (k2 / 3) + (k3 / 3) + (k4 / 6));
}

double SpacecraftDynamics::tempRKwZ()
{
	double k1, k2, k3, k4;
	double tX = getTorqueX();
	double tY = getTorqueY();
	double tZ = getTorqueZ();
	double iX = getMOIx();
	double iY = getMOIy();
	double iZ = getMOIz();
	double accX = getAccX();
	double accY = getAccY();
	double wZ = getVelocityZ();
	k1 = stepSize * ((tZ / iZ) - ((iY - iX) / iZ) * ((tY - accY*iY) / (iX - iZ)*wZ) * ((tX - accX*iX) / (iZ - iY)*wZ));
	k2 = stepSize * ((tZ / iZ) - ((iY - iX) / iZ) * ((tY - accY*iY) / (iX - iZ)*(wZ + k1/2)) * ((tX - accX*iX) / (iZ - iY)*(wZ + k1/2)));
	k3 = stepSize * ((tZ / iZ) - ((iY - iX) / iZ) * ((tY - accY*iY) / (iX - iZ)*(wZ + k2 / 2)) * ((tX - accX*iX) / (iZ - iY)*(wZ + k2 / 2)));
	k4 = stepSize * ((tZ / iZ) - ((iY - iX) / iZ) * ((tY - accY*iY) / (iX - iZ)*(wZ + k3)) * ((tX - accX*iX) / (iZ - iY)*(wZ + k3)));
	return (wZ + (k1 / 6) + (k2 / 3) + (k3 / 3) + (k4 / 6));
}

void SpacecraftDynamics::getNextw()
{
	wX = tempRKwX();
	wY = tempRKwY();
	wZ = tempRKwZ();
}

void SpacecraftDynamics::getNextAcc()
{
	aX = (torqueX / iX) - ((iZ - iY) / iX) * ((torqueZ - aZ*iZ) / (iY - iX)*wX) * ((torqueY - aY*iY) / (iX - iZ)*wX);
	aY = (torqueY / iY) - ((iX - iZ) / iY) * ((torqueZ - aZ*iZ) / (iY - iX)*wY) * ((torqueX - aX*iX) / (iZ - iY)*wY);
	aZ = (torqueZ / iZ) - ((iY - iX) / iZ) * ((torqueY - aY*iY) / (iX - iZ)*wZ) * ((torqueX - aX*iX) / (iZ - iY)*wZ);
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

double SpacecraftDynamics::eulerMethodQuaternions(double row1, double row2, double row3, double row4, double q_initial)
{
	return q_initial + stepSize * ((row1 * getQuaternion0()) + (row2 * getQuaternionX()) + (row3 * getQuaternionY()) + (row4 * getQuaternionZ()));
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
	vector0_values.storeInFile(vector0);
	vectorX_values.storeInFile(vectorX);
	vectorY_values.storeInFile(vectorY);
	vectorZ_values.storeInFile(vectorZ);
	thetaX_values.storeInFile(thetaX);
	thetaY_values.storeInFile(thetaY);
	thetaZ_values.storeInFile(thetaZ);
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
	/**
	double q0_next = eulerMethodQuaternions(0, -getVelocityX(), -getVelocityY(), -getVelocityZ(), getQuaternion0());
	double qX_next = eulerMethodQuaternions(getVelocityX(), 0, getVelocityZ(), -getVelocityY(), getQuaternionX());
	double qY_next = eulerMethodQuaternions(getVelocityY(), -getVelocityZ(), 0, getVelocityX(), getQuaternionY());
	double qZ_next = eulerMethodQuaternions(getVelocityZ(), getVelocityY(), -getVelocityX(), 0, getQuaternionZ());
	**/

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

void SpacecraftDynamics::setQuaternionInverseInitialValues(double quaternion0Inverse, double quaternionXInverse, double quaternionYInverse, double quaternionZInverse)
{
	q0Inverse = quaternion0Inverse;
	qXInverse = quaternionXInverse;
	qYInverse = quaternionYInverse;
	qZInverse = quaternionZInverse;
}

void SpacecraftDynamics::setVectorInitialValues(double v0, double vX, double vY, double vZ)
{
	vector0 = v0;
	vectorX = vX;
	vectorY = vY;
	vectorZ = vZ;
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

double SpacecraftDynamics::getTorqueX()
{
	return torqueX;
}

double SpacecraftDynamics::getTorqueY()
{
	return torqueY;
}

double SpacecraftDynamics::getTorqueZ()
{
	return torqueZ;
}

double SpacecraftDynamics::getMOIx()
{
	return iX;
}

double SpacecraftDynamics::getMOIy()
{
	return iY;
}

double SpacecraftDynamics::getMOIz()
{
	return iZ;
}

double SpacecraftDynamics::getconstantX()
{
	return constantX;
}

double SpacecraftDynamics::getconstantY()
{
	return constantY;
}

double SpacecraftDynamics::getconstantZ()
{
	return constantZ;
}

double SpacecraftDynamics::getAccX()
{
	return aX;
}

double SpacecraftDynamics::getAccY()
{
	return aY;
}

double SpacecraftDynamics::getAccZ()
{
	return aZ;
}

double SpacecraftDynamics::getQ0Inverse()
{
	return q0Inverse;
}

double SpacecraftDynamics::getQXInverse()
{
	return qXInverse;
}

double SpacecraftDynamics::getQYInverse()
{
	return qYInverse;
}

double SpacecraftDynamics::getQZInverse()
{
	return qZInverse;
}
double SpacecraftDynamics::getVector0()
{
	return vector0;
}

double SpacecraftDynamics::getVectorX()
{
	return vectorX;
}

double SpacecraftDynamics::getVectorY()
{
	return vectorY;
}

double SpacecraftDynamics::getVectorZ()
{
	return vectorZ;
}


//a x b
double SpacecraftDynamics::quaternion0Mulitplication(double a0, double a1, double a2, double a3, double b0, double b1, double b2, double b3)
{
	return ((b0 * a0) - (b1 * a1) - (b2 * a2) - (b3 * a3));
}

//a x b
double SpacecraftDynamics::quaternionXMulitplication(double a0, double a1, double a2, double a3, double b0, double b1, double b2, double b3)
{
	return ((b0 * a1) + (b1 * a0) - (b2 * a3) + (b3 * a2));
}

//a x b
double SpacecraftDynamics::quaternionYMulitplication(double a0, double a1, double a2, double a3, double b0, double b1, double b2, double b3)
{
	return ((b0 * a2) + (b1 * a3) + (b2 * a0) - (b3 * a1));
}

//a x b
double SpacecraftDynamics::quaternionZMulitplication(double a0, double a1, double a2, double a3, double b0, double b1, double b2, double b3)
{
	return ((b0 * a3) - (b1 * a2) + (b2 * a1) + (b3 * a0));
}

double SpacecraftDynamics::inverseQuaternionVal(double qVal)
{
	//return (qVal / findMagnitude(getQuaternion0(), getQuaternionX(), getQuaternionY(), getQuaternionZ()));
	return qVal;

}

double SpacecraftDynamics::findMagnitude(double q0, double qX, double qY, double qZ)
{
	return sqrt(pow(q0, 2.0) + pow(qX, 2.0) + pow(qY, 2.0) + pow(qZ, 2.0));
}

void SpacecraftDynamics::findNextVector()
{
	//find inverse by normalizing conjugate (conjugate is negative for all vector parts)
	double quaternion0Inverse_temp = inverseQuaternionVal(getQuaternion0());
	double quaternionXInverse_temp = inverseQuaternionVal(getQuaternionX() * -1.0);
	double quaternionYInverse_temp = inverseQuaternionVal(getQuaternionY() * -1.0);
	double quaternionZInverse_temp = inverseQuaternionVal(getQuaternionZ() * -1.0);
	q0Inverse = quaternion0Inverse_temp;
	qXInverse = quaternionXInverse_temp;
	qYInverse = quaternionYInverse_temp;
	qZInverse = quaternionZInverse_temp;

	//multiply between vector and inverse of quaternion
	double quaternion0Value_temp = quaternion0Mulitplication(getVector0(), getVectorX(), getVectorY(), getVectorZ(),
		getQ0Inverse(), getQXInverse(), getQYInverse(), getQZInverse());

	double quaternionXValue_temp = quaternionXMulitplication(getVector0(), getVectorX(), getVectorY(), getVectorZ(),
		getQ0Inverse(), getQXInverse(), getQYInverse(), getQZInverse());

	double quaternionYValue_temp = quaternionYMulitplication(getVector0(), getVectorX(), getVectorY(), getVectorZ(),
		getQ0Inverse(), getQXInverse(), getQYInverse(), getQZInverse());

	double quaternionZValue_temp = quaternionZMulitplication(getVector0(), getVectorX(), getVectorY(), getVectorZ(),
		getQ0Inverse(), getQXInverse(), getQYInverse(), getQZInverse());
	
	//multiply between quaternion and product of vector and inverse of quaternion
	double vector0_next = quaternion0Mulitplication(getQuaternion0(), getQuaternionX(), getQuaternionY(), getQuaternionZ(),
		quaternion0Value_temp, quaternionXValue_temp, quaternionYValue_temp, quaternionZValue_temp);

	double vectorX_next = quaternionXMulitplication(getQuaternion0(), getQuaternionX(), getQuaternionY(), getQuaternionZ(),
		quaternion0Value_temp, quaternionXValue_temp, quaternionYValue_temp, quaternionZValue_temp);

	double vectorY_next = quaternionYMulitplication(getQuaternion0(), getQuaternionX(), getQuaternionY(), getQuaternionZ(),
		quaternion0Value_temp, quaternionXValue_temp, quaternionYValue_temp, quaternionZValue_temp);

	double vectorZ_next = quaternionZMulitplication(getQuaternion0(), getQuaternionX(), getQuaternionY(), getQuaternionZ(),
		quaternion0Value_temp, quaternionXValue_temp, quaternionYValue_temp, quaternionZValue_temp);
		

	vector0 = vector0_next;
	vectorX = vectorX_next;
	vectorY = vectorY_next;
	vectorZ = vectorZ_next;
}

void SpacecraftDynamics::findThetaValues()
{
	thetaX = -2 * asin(getQuaternionX());
	thetaY = -2 * asin(getQuaternionY());
	thetaZ = -2 * asin(getQuaternionZ());
}
