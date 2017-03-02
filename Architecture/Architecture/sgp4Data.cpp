#include "sgp4Data.h"


using namespace std;


cTle sgp4Data::getTLE()
{
	return tleSGP4;
}

void sgp4Data::setTLE()
{
	string str1 = "SGP4 Test";
	string str2 = "1 25544U 98067A   16291.11854479  .00010689  00000-0  16758-3 0  9992";
	string str3 = "2 25544  51.6446 169.8664 0007102  80.6091  76.5051 15.54264543 23954";
	string tle[3] = { str1, str2, str3 };

	// Create a TLE object using the data above
	cTle tleSGP4(tle[0], tle[1], tle[2]);
}

void sgp4Data::setSatSGP4()
{
	//cSatellite(getTLE());
}

cSatellite sgp4Data::getSatSGP4()
{
	return satSGP4;
}

vector<cEci> sgp4Data::getVecPos()
{
	return vecPos;
}

vector<cGeo> sgp4Data::getGeoPos()
{
	return geoPos;
}

vector<cEcef> sgp4Data::getEcefPos()
{
	return ecefPos;
}
