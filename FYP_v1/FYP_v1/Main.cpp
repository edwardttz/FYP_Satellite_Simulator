#include <iostream>
#include <string>
#include "RungeKutta.h"

using namespace std;

int main(void) {

double tX=0.2, tY=0.2, tZ=0.2; //Torque values
double iX=0.002, iY=0.002, iZ=0.002; // MOI values in kg/m2
double h = 0.1; //100ms step
double aX=0.0 , aY=0.0, aZ=0.0; 
double wX=0.0169,wY=0.0169,wZ=0.0169; //angular velocity values in rad/s
double wX_next=0.0, wY_next=0.0, wZ_next=0.0; //To calculate next w values

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

double cX = (iY-iZ)/iX;
double cY = (iZ-iX)/iY;
double cZ = (iX-iY)/iZ;
double k1=0.0, k2=0.0, k3=0.0, k4=0.0;

for (int i=1; i<=100; i++) {
	//Switch torque values after 50 iterations
	if(i==50) { 
		tX=-tX;
		tY=-tY;
		tZ=-tZ;
	}
	//Find accelerations
	aX = (tX/iX) - cX*wY*wZ; 
	aY = (tY/iY) - cY*wX*wZ;
	aZ = (tZ/iZ) - cZ*wX*wY;
	//Print wX values for current iteration
	cout << wX << endl;      
	//printValues(i,aX,aY,aZ,wX,wY,wZ);
	
	//Find next w values
	wX_next = rungeKutta(tX,iX,wX,wY,wZ,h,cX);
	wY_next = rungeKutta(tY,iY,wY,wX,wZ,h,cY);
	wZ_next = rungeKutta(tZ,iZ,wZ,wX,wY,h,cZ);
	wX = wX_next;
	wY = wY_next;
	wZ = wZ_next;
} 

system("PAUSE");

}