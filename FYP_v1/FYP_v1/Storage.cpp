#include "Storage.h"
#include <fstream>

using namespace std;

void Storage::setFileName(string nameOfFile) 
{
	fileName = nameOfFile;
}

void Storage::storeInFile(double value) 
{
	ofstream myFile;
	myFile.open(fileName, ios::app);
	myFile << value << endl;
	myFile.close();
}

void Storage::storeAllInFile(vector <double> values) {
	ofstream myFile;
	for (int i = 0; i < values.size(); i++) {
		myFile.open(fileName, ios::app);
		myFile << values[i] << endl;
		myFile.close();
	}
}

void Storage::clearFile() 
{
	ofstream myFile;
	myFile.open(fileName, ios::trunc);
	myFile.close();
}