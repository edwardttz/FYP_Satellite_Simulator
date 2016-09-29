#include <iostream>
#include <string>
#include "RungeKutta.h"

using namespace std;

int main(void) {

	double tX=0.02, tY=0.02, tZ=0.02; //Torque values
	double spaceTorqueX = 0.0, spaceTorqueY = 0.0, spaceTorqueZ = 0.0; //future implementation when taking torque vals from space env
	double reactionTorqueX = 0.0, reactionTorqueY = 0.0, reactionTorqueZ = 0.0; //future implementation when taking vals from reaction wheel
	double iX=0.25, iY=0.15, iZ=0.35; // MOI values in kg/m2
	double h = 0.1; //100ms step
	double aX=0.0 , aY=0.0, aZ=0.0; 
	double wX=0.0169,wY=0.0169,wZ=0.0169; //angular velocity values in rad/s
	double wX_next=0.0, wY_next=0.0, wZ_next=0.0; //To calculate next w values
	string wX_values = "wX.txt", wY_values = "wY.txt", wZ_values = "wZ.txt"; //Filenames for w
	string aX_values = "aX.txt", aY_values = "aY.txt", aZ_values = "aZ.txt"; //Filenames for acc

	//Clear all files 
	clearFile(wX_values);
	clearFile(wY_values);
	clearFile(wZ_values);
	clearFile(aX_values);
	clearFile(aY_values);
	clearFile(aZ_values);

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

	//simplification of MOI vals into 1 var
	double cX = (iZ-iY)/iX;
	double cY = (iX-iZ)/iY;
	double cZ = (iY-iX)/iZ;

	double k1=0.0, k2=0.0, k3=0.0, k4=0.0;

	/**
	
	//get sum of all torques
	tX = spaceTorqueX + reactionTorqueX;
	tY = spaceTorqueY + reactionTorqueY;
	tZ = spaceTorqueZ + reactionTorqueZ;

	**/

	//run RK4 150 times, 50 for positive torque, 50 in negative torque and 50 with no torque after application of previous torques
	for (int i = 0; i < 150; i++) {

		//Switch torque values after 50 iterations
		if(i == 49) { 
			tX = -tX;
			tY = -tY;
			tZ = -tZ;
		}

		//no more torque after 100 iterations
		if (i == 99) {
			tX = 0.0;
			tY = 0.0;
			tZ = 0.0;
		}
		//Find accelerations
		aX = (tX/iX) - cX * wY * wZ; 
		aY = (tY/iY) - cY * wX * wZ;
		aZ = (tZ/iZ) - cZ * wX * wY;
	
		//Store into files
		storeInFile(aX,aX_values);
		storeInFile(aY,aY_values);
		storeInFile(aZ,aZ_values);
		storeInFile(wX,wX_values);
		storeInFile(wY,wY_values);
		storeInFile(wZ,wZ_values);

		//Find next velocity values
		wX_next = rungeKutta(tX,iX,wX,wY,wZ,h,cX);
		wY_next = rungeKutta(tY,iY,wY,wX,wZ,h,cY);
		wZ_next = rungeKutta(tZ,iZ,wZ,wX,wY,h,cZ);
		wX = wX_next;
		wY = wY_next;
		wZ = wZ_next;
	} 

	system("PAUSE");

}