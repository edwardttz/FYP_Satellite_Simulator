/*
	Noise Model based on HMC5883L-FDS
	Refer to page 2 of the HMC5883L-FDS.pdf for parameters
	Refer to page 8 of the HMC5883L-FDS.pdf for graph on Noise Floor
*/

#include "stdafx.h"

double genRandNoiseFloor(int gain)
{
	std::random_device rd;
	std::mt19937 e2(rd());
	std::normal_distribution<double> dist(gain, 2.15 + (gain / 11.0));

	return dist(e2);
}

double quantizeValue(double value) 
{
	double size = pow(2, 12) - 1;
	double sampling_size = 16 / size;
	// Field values ranges from 0 to 16 instead of -8 to 8
	value += 8000;
	// quantization will be rounded off to the nearest bit
	int result = (int) round(value / sampling_size);
	double result_temp = result / 1000.0;
	result = (int)round(result_temp);
	return result;
}
/*
	Testing of noise model
	quantization
	{
		testQuantization();
	}
	Noise Floor
	{
		testNoiseFloor();
		printToCSV();
	}
*/
void testQuantization()
{
	double value = 0.0;
	cout << quantizeValue(value) << endl ;
	value = -4500;
	cout << quantizeValue(value) << endl;
	value = -8000;
	cout << quantizeValue(value) << endl;
	value = 1000;
	cout << quantizeValue(value) << endl;
	value = 8000;
	cout << quantizeValue(value) << endl;
}
void testNoiseFloor() 
{
	double size = 100.0;
	double data[8][100];

	for (int i=0; i<8; i++) 
	{
		for (int j = 0; j < 100; j++) 
		{
			data[i][j] = genRandNoiseFloor(i);
		}
	}
	saveNoiseFloor(data, 100);
}
void saveNoiseFloor(double data[][100], int size)
{
	ofstream myfile;
	myfile.precision(12);
	myfile.open("../NoiseFloor_raw.csv", ios::trunc);
	for (int i = 0; i < size; i++)
	{
		myfile << data[0][i] << ',' << data[1][i] << ',' << data[2][i] << ',' << data[3][i] << ','
			<< data[4][i] << ',' << data[5][i] << ',' << data[6][i] << ',' << data[7][i] << endl;
	}
	myfile.close();
}