#ifndef STORAGE_H
#define STORAGE_H

#include <string>

using namespace std;

class Storage
{
private:
	string fileName;
public:
	void setFileName(string);
	void storeInFile(double);
	void clearFile();

};

#endif