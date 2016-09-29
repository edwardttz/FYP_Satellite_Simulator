#include "RungeKutta.h"
#include <iostream>
#include <fstream>

using namespace std;

double rungeKutta (double torque, double I, double w_initial, double w1, double w2, double h, double c) {
	double k1,k2,k3,k4,w_next;
	k1 = (torque/I) - c * w1 * w2;
	k2 = (torque/I) - c * (w1 * w2 + ((h/2) * k1) );
	k3 = (torque/I) - c * (w1 * w2 + ((h/2) * k2) );
	k4 = (torque/I) - c * (w1 * w2 + h * k3); 
	w_next = w_initial + (h/6) * (k1 + 2 * k2 + 2 * k3 + k4);
	return w_next;
}

void printValues (double i, double a_X,double a_Y, double a_Z, double w_X,double w_Y, double w_Z) {
	cout << "**** Iteration " << i << " ****" << endl;
	cout << "Angular Velocity: X=" << w_X << " Y=" << w_Y << " Z=" << w_Z << endl;
	cout << "Angular Acceleration: X=" << a_X << " Y=" << a_Y << " Z=" << a_Z << endl << endl;
}

void storeInFile(double value, string fileName) {
	ofstream myFile;
	myFile.open(fileName, ios::app);
	myFile << value << endl;
	myFile.close();
}

void clearFile(string fileName) {
	ofstream myFile;
	myFile.open(fileName, ios::trunc);
	myFile.close();
}
