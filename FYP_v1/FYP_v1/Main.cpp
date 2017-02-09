#include <iostream>
#include <string>
#include <cmath>
#include "SpacecraftDynamics.h"
#include "Gyroscope Noise Model.h"
#include "Storage.h"

using namespace std;

SpacecraftDynamics s1;
GyroNoiseModel g1;

void findSpacecraftDynamics() {
for (int i = 0; i < 3600; i++) //18000 for 10ms, 3600 for 50ms
	{
		//switching torque off after 30s
		if (i == 599) //2999 for 10ms, 599 for 50ms
		{
			s1.setTorque(0.0, 0.0, 0.0);
		}

		//negative torque after 30s
		if (i == 1199) //5999 for 10ms, 1199 for 50ms
		{
			//s1.setTorque(-0.0107, -0.0107, -0.0107); //KR 1
			//s1.setTorque(-0.05, -0.05, -0.05); //UoSat12
			s1.setTorque(-0.01, -0.01, -0.01); //MOST
		}

		//switching torque off after 30s
		if (i == 1799) //8999 for 10ms, 1799 for 50ms
		{
			s1.setTorque(0.0, 0.0, 0.0);
		}

		//Find acceleration, next velocity and store all values in text file

		g1.findRealW();
		s1.findAcc();
		s1.findThetaValues();
		s1.storeValues();
		g1.storeValues();
		s1.getNextw();
		s1.findNextQuaternion();
		s1.findNextVector();	
		cout << i << endl;
	} 
}

int main(void) 
{
	/*
	//For user input :
	s1.getUserInput(); //Gets torque and MOI values
	g1.getUserInput(); //Gets full scale value
	*/

	//For MOI and max torque values: 
	s1.setSatellite("MOST");
	//s1.setSatellite("Kent Ridge 1");
	//s1.setSatellite("UoSat12");

	//For Gyro noise model:
	g1.setFSValue(250); //gets min and max non linearity
	
	//s1.setStepSize(0.01); //10ms stepsize
	s1.setStepSize(0.05); //50ms stepsize
	
	s1.setInitialW(0.0, 0.0, 0.0);
	s1.setQuaternionInitialValues(0.0, 0.0, 0.0, 1.0);
	s1.setVectorInitialValues(0.0, 0.0, 0.0, 1.0);
	s1.setQuaternionInverseInitialValues(0.0, 0.0, 0.0, 0.0);
	s1.findConstants();
	//findSpacecraftDynamics();
	g1.setSamplePeriod(0.02);
	g1.findAllanDeviation();
	system("PAUSE");
}