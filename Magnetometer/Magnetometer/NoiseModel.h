#pragma once

double genRandNoiseFloor(const int gain);
double quantizeValue(double value);
void testQuantization();
void testNoiseFloor();
void saveNoiseFloor(double data[][100], int size);