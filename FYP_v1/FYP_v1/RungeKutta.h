#ifndef RUNGEKUTTA_H
#define RUNGEKUTTA_H

#include <string>

using namespace std;
 
double rungeKutta (double, double, double, double, double, double, double);
void printValues (double, double, double, double, double, double, double); 
void storeInFile(double, string); 
void clearFile(string);

#endif