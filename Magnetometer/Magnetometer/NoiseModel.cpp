/*
	Noise Model based on HMC5883L-FDS
	Refer to page 2 of the HMC5883L-FDS.pdf for parameters
	Refer to page 8 of the HMC5883L-FDS.pdf for graph on Noise Floor
*/

#include "stdafx.h"

vector<double> genRandNoiseFloor(int gain)
{
	std::random_device rd;
	std::mt19937 e2(rd());
	std::normal_distribution<double> dist(gain, 2.15 + (gain / 11.4));
	vector<double> hist;
	double variance = 0.0;
	for (int n = 0; n < 100; n++)
	{
		hist.push_back(dist(e2));
	}
	double sum = std::accumulate(hist.begin(), hist.end(), 0.0);
	double avg = sum / hist.size();
	for (int i = 0; i < 100; i++)
	{
		variance += pow(hist[i] - avg, 2);
	}
	double stdev = sqrt(variance / 100.0);
	for (int j = 0; j < 100; j++)
	{
		hist[j] += (gain - avg);
		hist[j] /= stdev;
		hist[j] *= 2.12 + (gain / 10.4);
	}
	return hist;
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
	saveNoiseFloor();
}
void saveNoiseFloor()
{
	vector<vector<double>> data;
	for (int i = 0; i < 8; i++) {
		data.push_back(genRandNoiseFloor(i));
	}
	ofstream myfile;
	myfile.precision(12);
	myfile.open("../NoiseFloor_raw.csv", ios::trunc);
	for (int i = 0; i < 100; i++)
	{
		myfile << data[0][i] << ',' << data[1][i] << ',' << data[2][i] << ',' << data[3][i] << ','
			<< data[4][i] << ',' << data[5][i] << ',' << data[6][i] << ',' << data[7][i] << endl;
	}
	myfile.close();
}