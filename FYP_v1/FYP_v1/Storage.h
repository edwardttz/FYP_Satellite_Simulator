#ifndef STORAGE_H
#define STORAGE_H

#include <string>
#include <vector>

using namespace std;

class Storage 
{
private:
	string fileName;
public:
	void setFileName(string);
	void storeInFile(double);
	void storeAllInFile(vector <double>);
	void clearFile();

};

#endif