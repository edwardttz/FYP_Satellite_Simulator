#include <iostream>
#include <string>
#include <cmath>
#include "SpacecraftDynamics.h"
#include "Storage.h"
#include <time.h>

using namespace std;

int main(void) 
{
	clock_t tStart = clock();
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

	//s1.setMOIValues(3.03, 4.85, 2.98); //KR 1
	//s1.setMOIValues(40.45, 42.09, 41.36); //UoSat12
	s1.setMOIValues(3.4, 2.18, 1.68); //MOST

	//s1.setTorque(0.0107, 0.0107, 0.0107); //KR 1
	//s1.setTorque(0.05, 0.05, 0.05); //UoSat12
	s1.setTorque(0.01, 0.01, 0.01); //MOST
	
	//s1.setStepSize(0.01); //10ms stepsize
	s1.setStepSize(0.05); //50ms stepsize

	//for single iteration
	/*
	s1.setInitialW(0.0, 0.0, 0.0);
	s1.setQuaternionInitialValues(0.0, 0.0, 0.0, 1.0);
	s1.setVectorInitialValues(0.0, 0.0, 0.0, 1.0);
	s1.setQuaternionInverseInitialValues(0.0, 0.0, 0.0, 0.0);
	s1.findConstants();
	
	printf("Time taken = %.10fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	*/

	//change i < some number to fit iterations and stepsize
	//for multiple iterations
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

		s1.findAcc();
		s1.findThetaValues();
		s1.storeValues();
		s1.getNextw();
		s1.findNextQuaternion();
		s1.findNextVector();	
	} 
	printf("Time taken = %.10fs\n", (double)(clock() - tStart) / CLOCKS_PER_SEC);
	
	system("PAUSE");
}