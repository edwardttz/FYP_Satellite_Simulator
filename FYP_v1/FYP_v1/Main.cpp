#include <iostream>
#include <string>
#include "SpacecraftDynamics.h"
#include "Storage.h"

using namespace std;

int main(void) 
{
	/*
	For user input :
	cout << "Enter tX: " << endl;
	cin >> tX;
	cout << "Enter tY: " << endl;
	cin >> tY;
	cout << "Enter tZ: " << endl;
	cin >> tZ;
	cout << "Enter I_xx: " << endl;
	cin >> I_x;
	cout << "Enter I_yy: " << endl;
	cin >> I_y;
	cout << "Enter I_zz: " << endl;
	cin >> I_z;
	*/

	SpacecraftDynamics s1;
	s1.setMOIValues(0.5, 0.3, 0.7);
	//positive torque initially
	s1.setTorque(0.02, 0.02, 0.02);
	s1.setInitialW(0.0, 0.0, 0.0);
	s1.setStepSize(0.05);
	s1.setQuaternionInitialValues(0.0, 0.0, 0.0, 1.0);
	s1.findConstants();
	//change i < some number to fit iterations and stepsize
	for (int i = 0; i < 3600; i++) 
	{
		//switching torque off after 30s
		if(i == 599) 
		{ 
			s1.setTorque(0.0, 0.0, 0.0);
		}

		//negative torque after 30s
		if (i == 1199) 
		{
			s1.setTorque(-0.02, -0.02, -0.02);
		}

		//switching torque off after 30s
		if (i == 1799)
		{
			s1.setTorque(0.0, 0.0, 0.0);
		}

		//Find acceleration, next velocity and store all values in text file
		s1.findAcc();
		s1.storeValues();
		s1.findNextW();
		s1.findNextQuaternion();
	} 
	system("PAUSE");
}