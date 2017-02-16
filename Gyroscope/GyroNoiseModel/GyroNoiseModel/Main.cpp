#include <iostream>
#include <string>
#include <cmath>
#include "Gyroscope Noise Model.h"
#include "Storage.h"

using namespace std;

GyroNoiseModel g1;

int main(void) 
{
	/*
	//For user input :
	g1.getUserInput(); //Gets full scale value
	*/
	
	g1.setFSValue(250); //gets min and max non linearity
	g1.setSamplePeriod(0.02);
	//g1.setSamplePeriod(1);
	g1.findAllanDeviation();
	
	system("PAUSE");
}