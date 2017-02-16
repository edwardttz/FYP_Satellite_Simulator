#pragma once

using namespace std;

class SunSensorModel
{
private:
	/** Parameters **/

public:
	SunSensorModel();
	SunSensorModel(const SunSensorModel &s);

	double x, y, z, d;
};
