#pragma once

#include "stdafx.h"
using namespace std;

//void calcMagFieldMain(const cGeo geoPos, const cJulian date, vector<double>& magFieldValues);
void calculateMagField(double lat, double lon, double h, double t, vector<double>& magFieldValues);
double calculateDecimalYear(double julianDate);